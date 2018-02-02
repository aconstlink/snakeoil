//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "xaudio2_api.h"
#include "objects/audio_buffer.h"

#include "../../buffer/audio_buffer.h"
#include "../../buffer/pcm/pcm_buffer.hpp"

#include <snakeoil/log/global.h>

using namespace so_audio ;
using namespace so_audio::so_win32 ;


//*************************************************************************************
xaudio2_api::xaudio2_api( void_t ) 
{
    
}

//*************************************************************************************
xaudio2_api::xaudio2_api( this_rref_t rhv )
{
    so_move_member_ptr( _xeng_ptr, rhv ) ;
    so_move_member_ptr( _dest_ptr, rhv ) ;
}

//*************************************************************************************
xaudio2_api::~xaudio2_api( void_t )
{
    so_log::global_t::error_and_exit( so_core::is_not_nullptr(_xeng_ptr), 
        "[xaudio2_api::~xaudio2_api] : xaudio2 not released" ) ;
}

//*************************************************************************************
xaudio2_api::this_ptr_t xaudio2_api::create( this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_audio::memory::alloc( std::move(rhv), p ) ;
}

//*************************************************************************************
void_t xaudio2_api::destroy( this_ptr_t ptr )
{
    so_audio::memory::dealloc( ptr ) ;
}

//*************************************************************************************
bool_t xaudio2_api::initialize( void_t ) 
{
    UINT32 flags = 0 ;

#if defined( SNAKEOIL_TARGET_OS_WIN7 )
    {
        auto const res = CoInitializeEx( NULL, COINIT_MULTITHREADED );
        if( res != S_OK )
        {
            so_log::global_t::error( "[xaudio2_api::initialize] : CoInitializeEx" ) ;
            return false ;
        }

#if defined( SNAKEOIL_DEBUG )
        flags |= XAUDIO2_DEBUG_ENGINE ;
#endif
    }

#endif

    {
        auto const res = XAudio2Create( &_xeng_ptr, flags, XAUDIO2_DEFAULT_PROCESSOR ) ;
        so_log::global_t::error( res != S_OK, "[xaudio2_engine::xaudio2_engine] : XAudio2Create" ) ;

        if( res != S_OK ) return false ;
    }

    {
        auto const res = _xeng_ptr->CreateMasteringVoice( &_dest_ptr ) ;
        so_log::global_t::error( res != S_OK, "[xaudio2_engine::xaudio2_engine] : CreateMasteringVoice" ) ;
        if( res != S_OK ) return false ;
    }

    
    return true ;
}

//*************************************************************************************
void_t xaudio2_api::release( void_t ) 
{
    if( so_core::is_not_nullptr( _xeng_ptr ) )
    {
        _xeng_ptr->Release() ;
        _xeng_ptr = nullptr ;
    }
}

//*************************************************************************************
api_type xaudio2_api::get_type( void_t ) const
{
    return so_audio::api_type::xaudio2 ;
}

//*************************************************************************************
void_t xaudio2_api::destroy( void_t )
{
    this_t::destroy( this ) ;
}

//*************************************************************************************
so_audio::result xaudio2_api::create( so_audio::audio_buffer_ptr_t pb, so_xaudio2::pcm_info_cref_t info )
{
    if( so_core::is_nullptr( pb ) )
        return so_audio::invalid_argument ;
    
    if( so_core::is_not_nullptr( pb->get_driver_object() ) )
        return so_audio::ok ;

    WAVEFORMATEX fmt ;
    memset( &fmt, 0, sizeof( WAVEFORMATEX ) ) ;

    so_audio::ipcm_buffer_ptr_t bptr = nullptr ;

    {
        size_t bits_per_sample = 0 ;
        size_t sib = 0 ;

        if( info.type == so_xaudio2::pcm_info_t::sample_type::int8 )
        {
            fmt.wFormatTag = WAVE_FORMAT_PCM ;
         
            {
                so_typedefs( so_audio::pcm_buffer<int8_t>, buffer ) ;

                buffer_t::init_data_t id ;
                id.num_channels = info.channels ;
                id.num_samples_per_sec = info.samples_per_sec ;
                id.num_second_cache = info.seconds ;

                bptr = buffer_t::create( buffer_t( id ), "[xaudio2_api::create] : pcm buffer" ) ;
            }

            bits_per_sample = 8 ;
            sib = 1 ;
        }
        else if( info.type == so_xaudio2::pcm_info_t::sample_type::int16 )
        {
            fmt.wFormatTag = WAVE_FORMAT_PCM ;

            {
                so_typedefs( so_audio::pcm_buffer<int16_t>, buffer ) ;

                buffer_t::init_data_t id ;
                id.num_channels = info.channels ;
                id.num_samples_per_sec = info.samples_per_sec ;
                id.num_second_cache = info.seconds ;

                bptr = buffer_t::create( buffer_t( id ), "[xaudio2_api::create] : pcm buffer" ) ;
            }

            bits_per_sample = 16 ;
            sib = 2 ;
        }
        else if( info.type == so_xaudio2::pcm_info_t::sample_type::int32 )
        {
            fmt.wFormatTag = WAVE_FORMAT_PCM ;
            
            {
                so_typedefs( so_audio::pcm_buffer<int32_t>, buffer ) ;

                buffer_t::init_data_t id ;
                id.num_channels = info.channels ;
                id.num_samples_per_sec = info.samples_per_sec ;
                id.num_second_cache = info.seconds ;

                bptr = buffer_t::create( buffer_t( id ), "[xaudio2_api::create] : pcm buffer" ) ;
            }

            bits_per_sample = 32 ;
            sib = 4 ;
        }
        else if( info.type == so_xaudio2::pcm_info_t::sample_type::ieee_float )
        {
            fmt.wFormatTag = WAVE_FORMAT_IEEE_FLOAT ;
            
            {
                so_typedefs( so_audio::pcm_buffer<float_t>, buffer ) ;

                buffer_t::init_data_t id ;
                id.num_channels = info.channels ;
                id.num_samples_per_sec = info.samples_per_sec ;
                id.num_second_cache = info.seconds ;

                bptr = buffer_t::create( buffer_t( id ), "[xaudio2_api::create] : pcm buffer" ) ;
            }

            bits_per_sample = 32 ;
            sib = 4 ;
        }

        fmt.nChannels = ( WORD ) info.channels ;
        fmt.nSamplesPerSec = ( DWORD ) info.samples_per_sec ;
        fmt.nAvgBytesPerSec = ( DWORD ) ( info.channels * sib * info.samples_per_sec ) ;
        fmt.wBitsPerSample = ( WORD ) bits_per_sample ;

        fmt.nBlockAlign = ( WORD ) ( ( info.channels*bits_per_sample ) / 8 ) ;
    }

    IXAudio2SourceVoice * srcv ;
    {
        auto const res = _xeng_ptr->CreateSourceVoice( &srcv, ( WAVEFORMATEX* ) &fmt ) ;
        if( res != S_OK )
        {
            so_log::global_t::error( "[xaudio2_api::create] : CreateSourceVoice" ) ;
            return so_audio::failed ;
        }
    }

    {
        auto * drv_ptr = so_audio::memory::alloc( so_audio::so_xaudio2::audio_buffer_t( srcv ),
            "[xaudio2_api::create] : so_audio::so_win32::play_buffer_t" ) ;

        drv_ptr->num_channels = info.channels ;
        drv_ptr->samples_per_sec = info.samples_per_sec ;

        *pb = so_audio::audio_buffer_t( drv_ptr, bptr ) ;
    }

    return so_audio::ok ;
}

//*************************************************************************************
so_audio::result xaudio2_api::release( so_audio::audio_buffer_ptr_t pb )
{
    if( so_core::is_nullptr( pb ) )
        return so_audio::invalid_argument ;

    if( so_core::is_nullptr( pb->get_driver_object() ) )
        return so_audio::ok ;

    so_audio::so_xaudio2::audio_buffer_ptr_t apio = dynamic_cast<so_audio::so_xaudio2::audio_buffer_ptr_t>( 
        pb->get_driver_object() ) ;

    if( so_core::is_nullptr( apio ) )
    {
        so_log::global_t::error( "[xaudio2_api::release] : play buffer has the wrong api object" ) ;
        return so_audio::failed ;
    }


    so_audio::memory::dealloc( apio ) ;
    pb->set_driver_object( nullptr ) ;

    return so_audio::ok ;
}

//*************************************************************************************
so_audio::result xaudio2_api::play( so_audio::audio_buffer_ptr_t ab )
{
    if( so_core::is_nullptr( ab ) )
        return so_audio::invalid_argument ;

    if( so_core::is_nullptr( ab->get_driver_object() ) )
        return so_audio::ok ;

    so_audio::so_xaudio2::audio_buffer_ptr_t apio = static_cast< so_audio::so_xaudio2::audio_buffer_ptr_t >(
        ab->get_driver_object() ) ;

    auto const res = apio->src_ptr->Start( 0 ) ;
    if( res != S_OK )
    {
        so_log::global_t::error( "[xaudio2_api::play] : Start" ) ;
        return so_audio::failed ;
    }

    return so_audio::ok ;
}

//*************************************************************************************
so_audio::result xaudio2_api::stop( so_audio::audio_buffer_ptr_t ab )
{
    if( so_core::is_nullptr( ab ) )
        return so_audio::invalid_argument ;

    if( so_core::is_nullptr( ab->get_driver_object() ) )
        return so_audio::ok ;

    so_audio::so_xaudio2::audio_buffer_ptr_t apio = static_cast< so_audio::so_xaudio2::audio_buffer_ptr_t >(
        ab->get_driver_object() ) ;

    auto const res = apio->src_ptr->Stop() ;
    if( res != S_OK )
    {
        so_log::global_t::error( "[xaudio2_api::play] : Start" ) ;
        return so_audio::failed ;
    }

    return so_audio::ok ;
}

//*************************************************************************************
so_audio::result xaudio2_api::submit( so_audio::audio_buffer_ptr_t ab, so_audio::range_cref_t r_in )
{
    if( so_core::is_nullptr( ab ) )
        return so_audio::invalid_argument ;

    if( so_core::is_nullptr( ab->get_driver_object() ) )
        return so_audio::ok ;

    so_audio::so_xaudio2::audio_buffer_ptr_t apio = static_cast< so_audio::so_xaudio2::audio_buffer_ptr_t >(
        ab->get_driver_object() ) ;

    IXAudio2SourceVoice * source_voice_ptr = apio->src_ptr ;

    size_t const sps = apio->samples_per_sec ;
    size_t const nc = apio->num_channels ;

    if( r_in.first > r_in.second )
    {
        // start -> end of buffer
        {
            so_audio::ipcm_buffer_ptr_t pcm_ptr = ab->get_pcm_buffer() ;
            size_t const num_elems = pcm_ptr->get_num_samples() - r_in.first ;

            byte_cptr_t buffer = byte_cptr_t( pcm_ptr->get_buffer_ptr() ) +
                r_in.first * so_audio::size_of( pcm_ptr->get_buffer_type() ) ;

            XAUDIO2_BUFFER xbuffer;
            memset( &xbuffer, 0, sizeof( XAUDIO2_BUFFER ) );

            xbuffer.AudioBytes = UINT32( num_elems * so_audio::size_of( pcm_ptr->get_buffer_type() ) );
            xbuffer.pAudioData = buffer ;
            xbuffer.PlayBegin = 0 ;
            xbuffer.PlayLength = 0 ;

            auto const res = source_voice_ptr->SubmitSourceBuffer( &xbuffer );
            if( res != S_OK )
            {
                so_log::global_t::error( "submit source buffer failed" ) ;
                return so_audio::failed ;
            }
        }

        // zero -> end, only if end > 0
        if( r_in.second > 0 )
        {
            so_audio::ipcm_buffer_ptr_t pcm_ptr = ab->get_pcm_buffer() ;

            size_t const num_elems = r_in.second ;

            // start at zero
            byte_cptr_t buffer = byte_cptr_t( pcm_ptr->get_buffer_ptr() ) ;

            XAUDIO2_BUFFER xbuffer;
            memset( &xbuffer, 0, sizeof( XAUDIO2_BUFFER ) );

            xbuffer.AudioBytes = UINT32( num_elems * so_audio::size_of( pcm_ptr->get_buffer_type() ) );
            xbuffer.pAudioData = buffer ;
            xbuffer.PlayBegin = 0 ;
            xbuffer.PlayLength = 0 ;

            auto const res = source_voice_ptr->SubmitSourceBuffer( &xbuffer );
            if( res != S_OK )
            {
                so_log::global_t::error( "submit source buffer failed" ) ;
                return so_audio::failed ;
            }
        }
    }
    else
    {
        so_audio::ipcm_buffer_ptr_t pcm_ptr = ab->get_pcm_buffer() ;

        size_t const num_elems = r_in.second - r_in.first ;

        if( num_elems > 0 )
        {
            byte_cptr_t buffer = byte_cptr_t( pcm_ptr->get_buffer_ptr() ) +
                r_in.first * so_audio::size_of( pcm_ptr->get_buffer_type() ) ;

            XAUDIO2_BUFFER xbuffer;
            memset( &xbuffer, 0, sizeof( XAUDIO2_BUFFER ) );

            xbuffer.AudioBytes = UINT32( num_elems * so_audio::size_of( pcm_ptr->get_buffer_type() ) );
            xbuffer.pAudioData = buffer ;
            xbuffer.PlayBegin = UINT32( 0 ) ;
            xbuffer.PlayLength = UINT32( 0 ) ;

            auto const res = source_voice_ptr->SubmitSourceBuffer( &xbuffer );
            if( res != S_OK )
            {
                XAUDIO2_VOICE_STATE state;
                source_voice_ptr->GetState( &state );

                so_log::global_t::error( "submit source buffer failed" ) ;
                return so_audio::failed ;
            }
        }
    }

    return so_audio::ok ;
}