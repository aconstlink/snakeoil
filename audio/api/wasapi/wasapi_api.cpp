//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "wasapi_api.h"

#include "../../buffer/loopback_buffer.h"
#include "../../buffer/pcm/pcm_buffer.hpp"

#include <snakeoil/log/log.h>

#include <mmdeviceapi.h>
#include <audioclient.h>

#include <thread>


using namespace so_audio ;
using namespace so_audio::so_win32 ;

const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);
const IID IID_IAudioClient = __uuidof(IAudioClient);
const IID IID_IAudioCaptureClient = __uuidof(IAudioCaptureClient);

#define REFTIMES_PER_SEC  10000000
#define REFTIMES_PER_MILLISEC  10000

using namespace so_audio ;
using namespace so_audio::so_win32 ;

struct wasapi_api::loopback_device
{
    so_this_typedefs( loopback_device ) ;

public:

    loopback_device( void_t ) {}
    loopback_device( this_cref_t ) = delete ;
    loopback_device( this_rref_t rhv )
    {
        so_move_member_ptr( def_device_ptr, rhv ) ;
        so_move_member_ptr( audio_client_ptr, rhv ) ;
        so_move_member_ptr( wav_format_ptr, rhv ) ;
        so_move_member_ptr( capture_client_ptr, rhv ) ;

    }
    ~loopback_device( void_t ) {}

public:

    IMMDevice * def_device_ptr = NULL ;
    IAudioClient *audio_client_ptr = NULL;
    IAudioCaptureClient * capture_client_ptr = NULL ;

    // format see: mmreg.h
    WAVEFORMATEX * wav_format_ptr = NULL ;
};

//*************************************************************************************
wasapi_api::wasapi_api( void_t ) 
{
    _lbptr = so_audio::memory::alloc( loopback_data_t(),
        "[wasapi_engine::wasapi_engine] : loopback_data_t" ) ;

    auto * cap_ptr = _lbptr ;

    _lbptr->_lbt = std::thread( [cap_ptr]( void_t )
    {
        IMMDeviceEnumerator *pEnumerator = NULL ;
        IMMDevice *pDevice = NULL ;

        // format see: mmreg.h
        WAVEFORMATEX *pwfx = NULL ;
        IAudioClient *pAudioClient = NULL;
        UINT32 bufferFrameCount;
        REFERENCE_TIME hnsRequestedDuration = REFTIMES_PER_SEC;
        REFERENCE_TIME hnsActualDuration;
        IAudioCaptureClient *pCaptureClient = NULL;

        WAVEFORMATEXTENSIBLE * fmt_ext = NULL ;

        bool_t is_float = false ;

        so_audio::ipcm_buffer_ptr_t the_buffer = nullptr ;

        {
            auto const res = CoInitializeEx( NULL, COINIT_MULTITHREADED ) ;
            if( res != S_OK )
            {
                so_log::log::error( "[wasapi_engine::initialize] : CoInitializeEx" ) ;
                return cap_ptr->so_init.set_and_signal() ;
            }
        }

        {
            HRESULT const hr = CoCreateInstance(
                CLSID_MMDeviceEnumerator, NULL,
                CLSCTX_ALL, IID_IMMDeviceEnumerator,
                (void**)&pEnumerator ) ;

            if( hr != S_OK )
            {
                so_log::log::error( "[wasapi_engine::initialize] : CoCreateInstance" ) ;
                return cap_ptr->so_init.set_and_signal() ;
            }
        }

        {
            auto const res = pEnumerator->GetDefaultAudioEndpoint(
                eRender, eConsole, &pDevice ) ;

            if( res != S_OK )
            {
                so_log::log::error( "[wasapi_engine::initialize] : GetDefaultAudioEndpoint" ) ;
                return cap_ptr->so_init.set_and_signal() ;
            }
        }

        {
            auto const res = pDevice->Activate( IID_IAudioClient, CLSCTX_ALL,
                NULL, (void**)&pAudioClient );

            if( res != S_OK )
            {
                so_log::log::error( "[wasapi_engine::initialize] : Device Activate" ) ;
                return cap_ptr->so_init.set_and_signal() ;
            }
        }

        {
            auto const res = pAudioClient->GetMixFormat( &pwfx ) ;
            if( res != S_OK )
            {
                so_log::log::error( "[wasapi_engine::initialize] : GetMixFormat" ) ;
                return cap_ptr->so_init.set_and_signal() ;
            }

            if( pwfx->wFormatTag == WAVE_FORMAT_EXTENSIBLE )
            {
                fmt_ext = (WAVEFORMATEXTENSIBLE*)pwfx ;

                if( fmt_ext->SubFormat == KSDATAFORMAT_SUBTYPE_PCM )
                {
                    int const bp = 0 ;

                    // create integral type buffer
                    if( pwfx->wBitsPerSample == 8 )
                    {
                        so_audio::pcm_buffer_int8_t::init_data id ;
                        id.num_channels = size_t( pwfx->nChannels ) ;
                        id.num_samples_per_sec = size_t( pwfx->nSamplesPerSec ) ;

                        the_buffer = so_audio::pcm_buffer_int8_t::create(
                            so_audio::pcm_buffer_int8_t( id ),
                            "[wasapi_engine::initialize] : pcm_buffer_int8_t" ) ;
                    }
                    else if( pwfx->wBitsPerSample == 16 )
                    {
                        so_audio::pcm_buffer_int16_t::init_data id ;
                        id.num_channels = size_t( pwfx->nChannels ) ;
                        id.num_samples_per_sec = size_t( pwfx->nSamplesPerSec ) ;

                        the_buffer = so_audio::pcm_buffer_int16_t::create(
                            so_audio::pcm_buffer_int16_t( id ),
                            "[wasapi_engine::initialize] : pcm_buffer_int16_t" ) ;
                    }
                    else if( pwfx->wBitsPerSample == 32 )
                    {
                        so_audio::pcm_buffer_int32_t::init_data id ;
                        id.num_channels = size_t( pwfx->nChannels ) ;
                        id.num_samples_per_sec = size_t( pwfx->nSamplesPerSec ) ;

                        the_buffer = so_audio::pcm_buffer_int32_t::create(
                            so_audio::pcm_buffer_int32_t( id ),
                            "[wasapi_engine::initialize] : pcm_buffer_int32_t" ) ;
                    }

                }
                else if( fmt_ext->SubFormat == KSDATAFORMAT_SUBTYPE_IEEE_FLOAT )
                {
                    is_float = true ;

                    so_audio::pcm_buffer_float_t::init_data id ;
                    id.num_channels = size_t( pwfx->nChannels ) ;
                    id.num_samples_per_sec = size_t( pwfx->nSamplesPerSec ) ;

                    the_buffer = so_audio::pcm_buffer_float_t::create(
                        so_audio::pcm_buffer_float_t( id ),
                        "[wasapi_engine::initialize] : pcm_buffer_float_t" ) ;
                }
            }
        }

        {
            auto const res = pAudioClient->Initialize( AUDCLNT_SHAREMODE_SHARED,
                AUDCLNT_STREAMFLAGS_LOOPBACK, hnsRequestedDuration,
                0, pwfx, NULL ) ;

            if( res != S_OK )
            {
                so_log::log::error( "[wasapi_engine::initialize] : AudioClient Initialize" ) ;
                return cap_ptr->so_init.set_and_signal() ;
            }
        }

        {
            auto const res = pAudioClient->GetBufferSize( &bufferFrameCount ) ;
            if( res != S_OK )
            {
                so_log::log::error( "[wasapi_engine::initialize] : GetBufferSize" ) ;
                return cap_ptr->so_init.set_and_signal() ;
            }
        }

        {
            auto const res = pAudioClient->GetService( IID_IAudioCaptureClient, (void**)&pCaptureClient ) ;
            if( res != S_OK )
            {
                so_log::log::error( "[wasapi_engine::initialize] : AudioClient GetService" ) ;
                return cap_ptr->so_init.set_and_signal() ;
            }
        }

        {
            hnsActualDuration = REFTIMES_PER_SEC * bufferFrameCount / pwfx->nSamplesPerSec;
        }

        {
            auto const res = pAudioClient->Start() ;
            if( res != S_OK )
            {
                so_log::log::error( "[wasapi_engine::initialize] : AudioClient GetService" ) ;
                return cap_ptr->so_init.set_and_signal() ;
            }
        }

        cap_ptr->so_init.set_and_signal() ;


        size_t const num_bits_per_sample = pwfx->wBitsPerSample ;

        loopback_data_t::handlers_t hnds ;

#if 0
        so_audio::pcm_buffer_int32_t::init_data_t id ;
        id.num_channels = size_t( pwfx->nChannels ) ;
        id.num_samples_per_sec = size_t( pwfx->nSamplesPerSec ) ;

        so_audio::pcm_buffer_int32_t pcmb( id ) ;
#endif   

        while( so_core::is_not( cap_ptr->is_done ) )
        {
            size_t const wait_ms = 20 ;
            std::this_thread::sleep_for( std::chrono::milliseconds( wait_ms ) ) ;

            // check for new handlers and push to local array
            if( cap_ptr->handler_added )
            {
                so_thread::lock_guard_t lk( cap_ptr->mtx_handlers ) ;
                for( auto const & hnd : cap_ptr->hnds )
                    hnds.push_back( hnd ) ;

                cap_ptr->handler_added = false ;
            }

            // if no handlers, this thread sleeps
            if( hnds.size() == 0 )
            {
                so_thread::lock_t lk( cap_ptr->mtx_handlers ) ;
                while( so_core::is_not( cap_ptr->handler_added ) && so_core::is_not( cap_ptr->is_done ) )
                    cap_ptr->cv_handlers.wait( lk ) ;
            }

            if( cap_ptr->is_done ) break ;

            UINT32 packetLength = 0 ;

            // read available data
            {
                auto const res = pCaptureClient->GetNextPacketSize( &packetLength ) ;
                if( res != S_OK )
                {
                    so_log::log::error( "[wasapi_engine] : GetNextPacketSize" ) ;
                    break ;
                }
            }

            while( packetLength != 0 )
            {
                // get loopback data
                BYTE *data_ptr ;
                UINT32 num_frames_available ;
                DWORD flags ;

                {
                    auto const res = pCaptureClient->GetBuffer( &data_ptr,
                        &num_frames_available, &flags, NULL, NULL ) ;

                    if( res != S_OK )
                    {
                        so_log::log::error( "[wasapi_engine] : unable to get audio buffer."
                            "Will cancel thread." ) ;
                        break ;
                    }
                }

#if 0
                // copy to local buffer
                if( num_bits_per_sample == 8 )
                {
                    pcmb.copy_from<uint8_t>( data_ptr, size_t( num_frames_available ) ) ;
                }
                else if( num_bits_per_sample == 16 )
                {
                    pcmb.copy_from<uint16_t>( data_ptr, size_t( num_frames_available ) ) ;
                }
                else if( num_bits_per_sample == 32 )
                {
                    if( is_float )
                        pcmb.copy_from<float_t>( data_ptr, size_t( num_frames_available ) ) ;
                    else
                        pcmb.copy_from<uint32_t>( data_ptr, size_t( num_frames_available ) ) ;
                }
#else
                // use : source type == destination type
                the_buffer->copy_from( data_ptr, size_t( num_frames_available ) ) ;
#endif
                // release wasapi buffer
                {
                    auto const res = pCaptureClient->ReleaseBuffer( num_frames_available ) ;
                    if( res != S_OK )
                    {
                        so_log::log::error( "[wasapi_engine] : ReleaseBuffer" ) ;
                        break ;
                    }
                }

                // call loopback handler
                for( auto & hnd : hnds )
                {
                    hnd( *the_buffer ) ;
                }

                // read available data
                {
                    auto const res = pCaptureClient->GetNextPacketSize( &packetLength ) ;
                    if( res != S_OK )
                    {
                        so_log::log::error( "[wasapi_engine] : GetNextPacketSize" ) ;
                        break ;
                    }
                }
            }

            
        }
    } ) ;

    _lbptr->so_init.wait() ;
}

//*************************************************************************************
wasapi_api::wasapi_api( this_rref_t rhv )
{
    so_move_member_ptr( _lbptr, rhv ) ;
}

//*************************************************************************************
wasapi_api::~wasapi_api( void_t )
{
    if( so_core::is_not_nullptr( _lbptr ) )
    {
        _lbptr->is_done = true ;
        _lbptr->cv_handlers.notify_all() ;

        if( _lbptr->_lbt.joinable() )
            _lbptr->_lbt.join() ;

        so_audio::memory::dealloc( _lbptr ) ;
    }
}

//*************************************************************************************
wasapi_api::this_ptr_t wasapi_api::create( this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_audio::memory::alloc( std::move( rhv ), p ) ;
}


//*************************************************************************************
bool_t wasapi_api::install_handler( so_audio::loopback_handler_t handler, handler_id_out_t id_out )
{
    {
        so_thread::lock_guard_t( _lbptr->mtx_handlers ) ;
        _lbptr->handler_added = true ;
        _lbptr->hnds.push_back( handler ) ;
    }

    _lbptr->cv_handlers.notify_all() ;

    return true ;
}

//*************************************************************************************
void_t wasapi_api::destroy( this_ptr_t ptr )
{
    so_audio::memory::dealloc( ptr ) ;
}

//*************************************************************************************
bool_t wasapi_api::initialize( void_t ) 
{
    {
        auto const res = CoInitializeEx( NULL, COINIT_MULTITHREADED ) ;
        if( res != S_OK )
        {
            so_log::log::error( "[wasapi_api::initialize] : CoInitializeEx" ) ;
            return false ;
        }
    }

    IMMDeviceEnumerator *pEnumerator = NULL ;
    {
        HRESULT const hr = CoCreateInstance(
            CLSID_MMDeviceEnumerator, NULL,
            CLSCTX_ALL, IID_IMMDeviceEnumerator,
            (void**)&pEnumerator ) ;

        if( hr != S_OK )
        {
            so_log::log::error( "[wasapi_api::initialize] : CoCreateInstance" ) ;
            return false ;
        }
    }

    IMMDevice *pDevice = NULL ;
    WAVEFORMATEX *pwfx = NULL ;
    IAudioClient *pAudioClient = NULL;
    UINT32 bufferFrameCount;
    REFERENCE_TIME hnsRequestedDuration = REFTIMES_PER_SEC;
    REFERENCE_TIME hnsActualDuration;
    IAudioCaptureClient *pCaptureClient = NULL;

    this_t::loopback_device lpdev ;
    

    {
        auto const res = pEnumerator->GetDefaultAudioEndpoint( eRender, eConsole, &pDevice ) ;

        if( res != S_OK )
        {
            so_log::log::error( "[wasapi_api::initialize] : GetDefaultAudioEndpoint" ) ;
            return false ;
        }
        lpdev.def_device_ptr = pDevice ;
    }

    {
        auto const res = pDevice->Activate( IID_IAudioClient, CLSCTX_ALL,
            NULL, (void**)&pAudioClient ) ;

        if( res != S_OK )
        {
            so_log::log::error( "[wasapi_api::initialize] : Device Activate" ) ;
            return false ;
        }
        lpdev.audio_client_ptr = pAudioClient ;
    }

    {
        auto const res = pAudioClient->GetMixFormat( &pwfx ) ;
        if( res != S_OK )
        {
            so_log::log::error( "[wasapi_api::initialize] : GetMixFormat" ) ;
            return false ;
        }

        lpdev.wav_format_ptr = pwfx ;
    }

    {
        auto const res = pAudioClient->Initialize( AUDCLNT_SHAREMODE_SHARED,
            AUDCLNT_STREAMFLAGS_LOOPBACK, hnsRequestedDuration, 0, pwfx, NULL ) ;

        if( res != S_OK )
        {
            so_log::log::error( "[wasapi_engine::initialize] : AudioClient Initialize" ) ;
            return false ;
        }
    }

    {
        auto const res = pAudioClient->GetBufferSize( &bufferFrameCount ) ;
        if( res != S_OK )
        {
            so_log::log::error( "[wasapi_engine::initialize] : GetBufferSize" ) ;
            return false ;
        }
    }

    {
        auto const res = pAudioClient->GetService( IID_IAudioCaptureClient, (void**)&pCaptureClient ) ;
        if( res != S_OK )
        {
            so_log::log::error( "[wasapi_engine::initialize] : AudioClient GetService" ) ;
            return false ;
        }
        lpdev.capture_client_ptr = pCaptureClient ;
    }

    {
        hnsActualDuration = REFTIMES_PER_SEC * bufferFrameCount / pwfx->nSamplesPerSec;
    }

    {
        auto const res = pAudioClient->Start() ;
        if( res != S_OK )
        {
            so_log::log::error( "[wasapi_engine::initialize] : AudioClient GetService" ) ;
            return false ;
        }
    }

    _lp_dev_ptr = so_audio::memory::alloc( std::move(lpdev), 
        "[wasapi_api::initialize] : loopback_device" ) ;

    return true ;
}

//*************************************************************************************
so_audio::result wasapi_api::create( loopback_buffer_ptr_t lb_ptr ) 
{
    if( so_core::is_nullptr( lb_ptr ) )
        return so_audio::invalid_argument ;

    WAVEFORMATEX * pwfx = _lp_dev_ptr->wav_format_ptr ;

    if( pwfx->wFormatTag == WAVE_FORMAT_EXTENSIBLE )
    {
        so_audio::ipcm_buffer_ptr_t the_buffer = nullptr ;

        WAVEFORMATEXTENSIBLE * fmt_ext = (WAVEFORMATEXTENSIBLE*)pwfx ;

        if( fmt_ext->SubFormat == KSDATAFORMAT_SUBTYPE_PCM )
        {
            // create integral type buffer
            if( pwfx->wBitsPerSample == 8 )
            {
                so_audio::pcm_buffer_int8_t::init_data id ;
                id.num_channels = size_t( pwfx->nChannels ) ;
                id.num_samples_per_sec = size_t( pwfx->nSamplesPerSec ) ;

                the_buffer = so_audio::pcm_buffer_int8_t::create(
                    so_audio::pcm_buffer_int8_t( id ),
                    "[wasapi_api::create] : pcm_buffer_int8_t" ) ;
            }
            else if( pwfx->wBitsPerSample == 16 )
            {
                so_audio::pcm_buffer_int16_t::init_data id ;
                id.num_channels = size_t( pwfx->nChannels ) ;
                id.num_samples_per_sec = size_t( pwfx->nSamplesPerSec ) ;

                the_buffer = so_audio::pcm_buffer_int16_t::create(
                    so_audio::pcm_buffer_int16_t( id ),
                    "[wasapi_api::create] : pcm_buffer_int16_t" ) ;
            }
            else if( pwfx->wBitsPerSample == 32 )
            {
                so_audio::pcm_buffer_int32_t::init_data id ;
                id.num_channels = size_t( pwfx->nChannels ) ;
                id.num_samples_per_sec = size_t( pwfx->nSamplesPerSec ) ;

                the_buffer = so_audio::pcm_buffer_int32_t::create(
                    so_audio::pcm_buffer_int32_t( id ),
                    "[wasapi_api::create] : pcm_buffer_int32_t" ) ;
            }
        }
        else if( fmt_ext->SubFormat == KSDATAFORMAT_SUBTYPE_IEEE_FLOAT )
        {
            so_audio::pcm_buffer_float_t::init_data id ;
            id.num_channels = size_t( pwfx->nChannels ) ;
            id.num_samples_per_sec = size_t( pwfx->nSamplesPerSec ) ;

            the_buffer = so_audio::pcm_buffer_float_t::create(
                so_audio::pcm_buffer_float_t( id ),
                "[wasapi_api::create] : pcm_buffer_float_t" ) ;
        }

        lb_ptr->set_pcm_buffer( the_buffer ) ;

        return so_audio::ok ;
    }

    return so_audio::not_implemented ;
}

//*************************************************************************************
so_audio::result wasapi_api::release( loopback_buffer_ptr_t lb_ptr ) 
{
    if( so_core::is_nullptr(lb_ptr) ) 
        return so_audio::invalid_argument ;

    lb_ptr->set_pcm_buffer( nullptr ) ;

    return so_audio::ok ;
}

//*************************************************************************************
so_audio::result wasapi_api::store( loopback_buffer_ptr_t lb_ptr ) 
{
    if( so_core::is_nullptr( lb_ptr ) )
        return so_audio::invalid_argument ;

    UINT32 packetLength = 0 ;

    auto * the_buffer = lb_ptr->get_pcm_buffer() ;
    if( so_core::is_nullptr( the_buffer ) )
        return so_audio::invalid_argument ;

    // read available data
    {
        auto const res = _lp_dev_ptr->capture_client_ptr->GetNextPacketSize( &packetLength ) ;
        if( res != S_OK )
        {
            so_log::log::error( "[wasapi_api::store] : GetNextPacketSize" ) ;
            return so_audio::failed ;
        }
    }

    while( packetLength != 0 )
    {
        BYTE * data_ptr ;
        UINT32 num_frames_available ;
        DWORD flags ;

        {
            auto const res = _lp_dev_ptr->capture_client_ptr->GetBuffer( &data_ptr,
                &num_frames_available, &flags, NULL, NULL ) ;

            if( res != S_OK )
            {
                so_log::log::error( "[wasapi_api::store] : unable to get audio buffer."
                    "Will cancel thread." ) ;
                return so_audio::failed ;
            }
        }


        // use : source type == destination type
        the_buffer->copy_from( data_ptr, size_t( num_frames_available ) ) ;

        // release wasapi buffer
        {
            auto const res = _lp_dev_ptr->capture_client_ptr->ReleaseBuffer( num_frames_available ) ;
            if( res != S_OK )
            {
                so_log::log::error( "[wasapi_api::store] : ReleaseBuffer" ) ;
                return so_audio::failed ;
            }
        }

        // read available data
        {
            auto const res = _lp_dev_ptr->capture_client_ptr->GetNextPacketSize( &packetLength ) ;
            if( res != S_OK )
            {
                so_log::log::error( "[wasapi_api::store] : GetNextPacketSize" ) ;
                return so_audio::failed ;
            }
        }
    }

    return so_audio::ok ;
}

//*************************************************************************************
void_t wasapi_api::release( void_t ) 
{
}

//*************************************************************************************
api_type wasapi_api::get_type( void_t ) const
{
    return so_audio::api_type::wasapi ;
}

//*************************************************************************************
void_t wasapi_api::destroy( void_t )
{
    this_t::destroy( this ) ;
}

//*************************************************************************************