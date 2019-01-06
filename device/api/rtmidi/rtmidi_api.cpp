//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "rtmidi_api.h"
#include "../../devices/midi/midi_device.h"

#include "../midi_notify.h"

#include "../../modules/imidi_module.h" 

#include <snakeoil/log/global.h>
#include <snakeoil/core/macros/move.h>

#include <algorithm>

using namespace so_device ;
using namespace so_device::so_win32 ;

//****************************************************************************************
#if 0
void CALLBACK midi_in_proc(
    HMIDIIN   hMidiIn,
    UINT      wMsg,
    DWORD_PTR dwInstance,
    DWORD_PTR dwParam1,
    DWORD_PTR dwParam2
    )
{
    so_device::so_win32::rtmidi_api_t::global_self_ptr_t gptr = 
        (so_device::so_win32::rtmidi_api_t::global_self_ptr_t)(dwInstance) ;

    if( so_core::is_nullptr(gptr) )
    {
        so_log::global::error( "[midi_in_proc] : nullptr " ) ;
        return ;
    }

    so_device::so_win32::rtmidi_api_ptr_t _midi_mod_ptr = 
        gptr->self_ptr ;

    if( wMsg == MIM_OPEN )
    {}
    else if( wMsg == MIM_CLOSE )
    {}
    else if( wMsg == MIM_DATA )
    {
        WORD const hiword = HIWORD( dwParam1 ) ;
        WORD const loword = LOWORD( dwParam1 ) ;

        BYTE const status = LOBYTE( loword ) ;
        BYTE const byte1 = HIBYTE( loword ) ;
        BYTE const byte2 = LOBYTE( hiword ) ;
        BYTE const byte3 = HIBYTE( hiword ) ;

        _midi_mod_ptr->handle_message( hMidiIn, 
            so_device::midi_message( status, byte1, byte2, byte3 ) ) ;
    }
    else if( wMsg == MIM_LONGDATA )
    {}
    else if( wMsg == MIM_ERROR )
    {}
    else if( wMsg == MIM_LONGERROR )
    {}
    else if( wMsg == MIM_MOREDATA )
    {}
}

//****************************************************************************************
void CALLBACK midi_out_proc(
    HMIDIOUT   hmo,
    UINT      wMsg,
    DWORD_PTR dwInstance,
    DWORD_PTR dwParam1,
    DWORD_PTR dwParam2
    )
{
}
#endif

//****************************************************************************************
rtmidi_api::rtmidi_api( void_t ) 
{
    _global_self_ptr = so_device::memory::alloc<global_self_t>(
        "[so_device::midi_module::midi_module] : global self") ;
}

//****************************************************************************************
rtmidi_api::rtmidi_api( this_rref_t rhv ) 
{
    _devices = std::move( rhv._devices ) ;
    _ins = std::move( rhv._ins ) ;

    so_move_member_ptr( _global_self_ptr, rhv ) ;
    _global_self_ptr->self_ptr = this ;

    _midi_notifies = std::move( rhv._midi_notifies ) ;
}

//****************************************************************************************
rtmidi_api::~rtmidi_api( void_t ) 
{
    for( auto & item : _devices )
    {
        
        so_device::midi_device::destroy( item.dev_ptr ) ;

        {
            auto res = this_t::unregister_device( item ) ;
            so_log::global::error( so_device::no_success(res), 
                "[midi_module::~midi_module] : unregister_device" ) ;
        }
                
        /*if( item.mdata.inh != NULL )
            midiInClose( item.mdata.inh ) ;

        if( item.mdata.outh != NULL )
            midiOutClose( item.mdata.outh ) ;*/
    }

    for( auto * ptr : _midi_notifies )
    {
        ptr->destroy() ;
    }

    so_device::memory::dealloc( _global_self_ptr ) ;
}

//****************************************************************************************
rtmidi_api::this_ptr_t rtmidi_api::create( so_memory::purpose_cref_t p ) 
{
    return so_device::memory::alloc( this_t(), p ) ;
}

//****************************************************************************************
rtmidi_api::this_ptr_t rtmidi_api::create( this_rref_t rhv, so_memory::purpose_cref_t p ) 
{
    return so_device::memory::alloc( std::move(rhv), p ) ;
}

//****************************************************************************************
void_t rtmidi_api::destroy( this_ptr_t ptr ) 
{
    so_device::memory::dealloc( ptr ) ;
}

//****************************************************************************************
void_t rtmidi_api::install_midi_notify( so_device::imidi_notify_ptr_t nptr )
{
    so_thread::lock_guard_t lk( _mtx_notifies ) ;
    
    {
        auto const iter = std::find( _midi_notifies.begin(), _midi_notifies.end(), nptr ) ;
        if( iter != _midi_notifies.end() )
            return ;
    }

    _midi_notifies.push_back( nptr ) ;
}

//****************************************************************************************
void_t rtmidi_api::create_devices( so_device::imidi_module_ptr_t mptr )
{
    so_std::vector< so_std::string_t > names ;
    this_t::get_device_names( names ) ;

    for( auto const & name : names )
    {
        auto * mdev = mptr->create_midi_device( name ) ;
        if( so_core::is_nullptr( mdev ) ) continue ;

        auto iter = std::find_if( _devices.begin(), _devices.end(), 
            [&]( this_t::store_data_cref_t item ) 
        { 
            return item.key == name ;
        } ) ;
        
        // if the device is already created, 
        // only exchange the components
        if( iter != _devices.end() )
        {
            auto * midi_ptr = iter->dev_ptr ;
            ( *midi_ptr ) = std::move( *mdev ) ;
            mdev->destroy() ;
        }
        // otherwise, create a new one.
        else
        {
            this_t::store_data sd ;
            sd.key = name ;
            sd.dev_ptr = mdev ;
            _devices.push_back( sd ) ;

            check_handle_for_device( _devices.size() - 1 ) ;
        }
    }
}

//****************************************************************************************
void_t rtmidi_api::get_device_names( so_std::vector< so_std::string_t > & names_out ) const
{
    #if 0
    for( uint_t i = 0; i < midiInGetNumDevs(); ++i )
    {
        MIDIINCAPS caps ;
        ZeroMemory( &caps, sizeof( MIDIINCAPS ) ) ;

        MMRESULT const res = midiInGetDevCaps( i, &caps, sizeof( MIDIINCAPS ) ) ;
        if( res != MMSYSERR_NOERROR )
        {
            so_log::global::warning( "[so_device::rtmidi_api::get_device_names] : "
                                   "unable to retrieve device caps" ) ;
            continue ;
        }

        names_out.push_back( so_std::string_t( caps.szPname ) ) ;
    }
    #endif
}

//****************************************************************************************
so_device::midi_device_ptr_t rtmidi_api::find_midi_device( so_device::key_cref_t key ) 
{
    so_thread::lock_guard_t lk(_mtx) ;

    auto iter = std::find_if( _devices.begin(), _devices.end(), 
        [&]( this_t::store_data_cref_t item )
    {
        return item.key == key ;
    } ) ;

    if( iter == _devices.end() )
    {
        return nullptr ;
    }

    return iter->dev_ptr;
}

//****************************************************************************************
so_device::midi_device_ptr_t rtmidi_api::find_any_midi_device( void_t )
{
    so_thread::lock_guard_t lk( _mtx ) ;

    if( _devices.size() > 0 )
        return _devices[ 0 ].dev_ptr ;

    return nullptr ;
}

//****************************************************************************************
so_device::result rtmidi_api::unregister_device( store_data_ref_t item ) 
{
    #if 0
    if( item.mdata.inh != NULL )
    {
        // reset according to msdn. Otherwise midiInClose will fail due to
        // midiInAddBuffer used here.
        {
            auto res = midiInReset( item.mdata.inh ) ;
            so_log::global::error( res != MMSYSERR_NOERROR,
                "[so_device::so_win32::midi_module::unregister_device] : midiInReset" ) ;
        }

        {
            MIDIHDR mhdr ;
            ZeroMemory( &mhdr, sizeof(MIDIHDR) ) ;

            auto res = midiInUnprepareHeader( item.mdata.inh, &mhdr, sizeof(MIDIHDR) ) ;
            so_log::global::error( res != MMSYSERR_NOERROR,
                "[so_device::so_win32::midi_module::unregister_device] : midiInUnprepareHeader" ) ;

            so_device::memory::dealloc_raw( item.mdata.in_buffer ) ;
        }
        {
            auto res = midiInClose( item.mdata.inh ) ;
            so_log::global::error( res != MMSYSERR_NOERROR,
                "[so_device::so_win32::midi_module::unregister_device] : midiInClose" ) ;
        }
        item.mdata.inh = NULL ;
    }

    if( item.mdata.outh != NULL )
    {
        {
            auto res = midiOutShortMsg( item.mdata.outh, byte_t(255) ) ;
            so_log::global::error( res != MMSYSERR_NOERROR,
                "[so_device::so_win32::midi_module::unregister_device] : sending -1 failed" ) ;
        }

        auto res = midiOutClose( item.mdata.outh ) ;
        so_log::global::error( res != MMSYSERR_NOERROR,
            "[so_device::so_win32::midi_module::unregister_device] : midiInClose" ) ;
        item.mdata.outh = NULL ;
    }
    #endif
    return so_device::ok ;
}

//****************************************************************************************
void_t rtmidi_api::update_midi( void_t )
{
    // time interval
    // check periodically, so devices can be 
    // connected afterwards
    {
        check_for_new_devices() ;
    }

    #if 0
    for( auto & item : _devices )
    {
        if( item.mdata.inh == NULL || item.mdata.outh == NULL )
            continue ;

        so_device::midi_device_t::midi_messages_t msgs ;
        item.dev_ptr->update( msgs ) ;

        for( auto & msg : msgs )
        {
            MMRESULT res = midiOutShortMsg( item.mdata.outh, (DWORD) midi_message::to_32bit( msg ) ) ;
            so_log::global::error( res != MMSYSERR_NOERROR,
                "[midi_module::transmit_message] : midiOutShortMsg " ) ;
        }
     
        {
            auto res = midiInReset( item.mdata.inh ) ;
            so_log::global::error( res != MMSYSERR_NOERROR,
                "[so_device::so_win32::midi_module::update] : midiInReset" ) ;
        }

        {
            auto res = midiInStart( item.mdata.inh ) ;
            so_log::global::error( res != MMSYSERR_NOERROR,
                "[so_device::so_win32::midi_module::update] : midiInStart" ) ;
        }

        //midiInStart( item.mdata.inh ) ;
        //midiInReset( item.mdata.inh ) ;
    }
    #endif
}

//****************************************************************************************
void_t rtmidi_api::destroy( void_t ) 
{
    this_t::destroy( this ) ;
}

#if 0
//****************************************************************************************
void_t rtmidi_api::handle_message( HMIDIIN hin, so_device::midi_message_cref_t msg ) 
{
    for( auto & item : _devices )
    {
        if( item.mdata.inh == hin )
        {
            for( auto * ptr : _midi_notifies )
            {
                ptr->on_message( item.dev_ptr->get_device_info().device_name, msg ) ;
            }

            item.dev_ptr->receive_message( msg ) ;
        }
    }
}
#endif

//****************************************************************************************
void_t rtmidi_api::check_handle_for_device( size_t slot ) 
{
    if( slot >= _devices.size() ) 
        return ;
    
    auto & data = _devices[slot] ;
    
    auto * dptr = data.dev_ptr ;
    this_t::midi_data & md = data.mdata ;

    auto di = dptr->get_device_info() ;

    #if 0
    if( md.inh != NULL )
        return ;

    if( md.outh != NULL )
        return ;    

    uint_t index_in = uint_t(-1) ;
    for( uint_t i = 0; i < midiInGetNumDevs(); ++i )
    {
        MIDIINCAPS caps ;
        ZeroMemory( &caps, sizeof(MIDIINCAPS) ) ;

        MMRESULT const res = midiInGetDevCaps( i, &caps, sizeof(MIDIINCAPS) ) ;
        if( res != MMSYSERR_NOERROR )
        {
            so_log::global::warning( "[so_device::midi_module::check_handle_for_device] : \
                                   unable to retrieve device caps" ) ;
            continue ;
        }

        if( di.device_name == so_std::string_t(caps.szPname) )
        {
            index_in = i ;
            break ;
        }
    }

    if( so_log::global::warning( index_in == uint_t(-1), 
        "[so_device::midi_module::check_handle_for_device] : \
        Could not find midi device" ) )
    {
        return ;
    }
    
    uint_t index_out = uint_t( -1 ) ;
    for(uint_t i = 0; i < midiOutGetNumDevs(); ++i)
    {
        MIDIOUTCAPS caps ;
        ZeroMemory( &caps, sizeof( MIDIOUTCAPS ) ) ;

        MMRESULT const res = midiOutGetDevCaps( i, &caps, sizeof( MIDIOUTCAPS ) ) ;
        if(res != MMSYSERR_NOERROR)
        {
            so_log::global::warning( "[so_device::midi_module::check_handle_for_device] : \
                                        unable to retrieve midi out caps" ) ;
            continue ;
        }

        if( di.device_name == so_std::string_t( caps.szPname ) )
        {
            index_out = i ;
            break ;
        }
    }

    // midi-in handle
    {
        HMIDIIN hmin ;
        
        auto const res = midiInOpen( &hmin, index_in, (DWORD_PTR) midi_in_proc, 
            (DWORD_PTR)_global_self_ptr, CALLBACK_FUNCTION | MIDI_IO_STATUS ) ;

        so_log::global::warning( res != MMSYSERR_NOERROR, 
            "[so_device::midi_module::check_handle_for_device] : \
            Input MIDI device can not be opended" ) ;
        
        if( res == MMSYSERR_NOERROR )
        {
            MIDIHDR midihdr ;
            memset( &midihdr, 0, sizeof(MIDIHDR) );

            midihdr.dwBufferLength = 10 ;
            midihdr.lpData = (LPSTR)so_device::memory::alloc_raw<uchar_t>(10, 
                "[so_device::so_win32::midi_module::check_handle_for_device] : MIDIHDR.lpData" ) ;
            midihdr.dwFlags = 0 ;
            
            md.in_buffer = midihdr.lpData ;

            auto const prep_res = midiInPrepareHeader( hmin, &midihdr, sizeof( MIDIHDR ) ) ;
            so_log::global::error( prep_res != MMSYSERR_NOERROR, 
                "[so_device::midi_module::check_handle_for_device] : midiInPrepareHeader" ) ;

            auto const add_res = midiInAddBuffer( hmin, &midihdr, sizeof( MIDIHDR ) ) ;
            so_log::global::error( prep_res != MMSYSERR_NOERROR, 
                "[so_device::midi_module::check_handle_for_device] : midiInAddBuffer" ) ;
            
            if( prep_res == MMSYSERR_NOERROR && add_res == MMSYSERR_NOERROR )
            {
                midiInStart( hmin ) ;
                md.inh = hmin ;
            }
        }        
    }

    // midi-out handle
    if( index_out != uint_t(-1)) 
    {
        HMIDIOUT hmout ;
        {
            auto const res = midiOutOpen( &hmout, index_out, (DWORD_PTR)midi_out_proc, 
                (DWORD_PTR)_global_self_ptr, CALLBACK_FUNCTION ) ;

            so_log::global::error( res != MMSYSERR_NOERROR, 
                "[so_device::midi_module::check_handle_for_device] : \
                Output MIDI device can not be opended" ) ;
            if( res == MMSYSERR_NOERROR )
            {
                md.outh = hmout ;
            }
        }
    }
    #endif
}

//****************************************************************************************
void_t rtmidi_api::check_for_new_devices( void_t ) 
{

}
