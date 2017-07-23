//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "midi_module.h"
#include "../../../devices/midi/midi_device.h"

#include <snakeoil/log/log.h>
#include <snakeoil/core/macros/move.h>

#include <algorithm>
#include <windows.h>

using namespace so_device ;
using namespace so_device::so_win32 ;

//****************************************************************************************
void CALLBACK midi_in_proc(
    HMIDIIN   hMidiIn,
    UINT      wMsg,
    DWORD_PTR dwInstance,
    DWORD_PTR dwParam1,
    DWORD_PTR dwParam2
    )
{
    so_device::so_win32::midi_module_t::global_self_ptr_t gptr = 
        (so_device::so_win32::midi_module_t::global_self_ptr_t)(dwInstance) ;

    if( so_core::is_nullptr(gptr) )
    {
        so_log::log::error( "[midi_in_proc] : nullptr " ) ;
        return ;
    }

    so_device::so_win32::midi_module_ptr_t _midi_mod_ptr = 
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

//****************************************************************************************
midi_module::midi_module( void_t ) 
{
    _global_self_ptr = so_device::memory::alloc<global_self_t>(
        "[so_device::midi_module::midi_module] : global self") ;
}

//****************************************************************************************
midi_module::midi_module( this_rref_t rhv ) 
{
    _devices = std::move( rhv._devices ) ;
    _ins = std::move( rhv._ins ) ;

    so_move_member_ptr( _global_self_ptr, rhv ) ;
    _global_self_ptr->self_ptr = this ;
    
}

//****************************************************************************************
midi_module::~midi_module( void_t ) 
{
    for( auto & item : _devices )
    {
        
        so_device::midi_device::destroy( item.dev_ptr ) ;

        {
            auto res = this_t::unregister_device( item ) ;
            so_log::log::error( so_device::no_success(res), 
                "[midi_module::~midi_module] : unregister_device" ) ;
        }
                
        /*if( item.mdata.inh != NULL )
            midiInClose( item.mdata.inh ) ;

        if( item.mdata.outh != NULL )
            midiOutClose( item.mdata.outh ) ;*/
    }

    so_device::memory::dealloc( _global_self_ptr ) ;
}

//****************************************************************************************
midi_module::this_ptr_t midi_module::create( so_memory::purpose_cref_t p ) 
{
    return so_device::memory::alloc( this_t(), p ) ;
}

//****************************************************************************************
midi_module::this_ptr_t midi_module::create( this_rref_t rhv, so_memory::purpose_cref_t p ) 
{
    return so_device::memory::alloc( std::move(rhv), p ) ;
}

//****************************************************************************************
void_t midi_module::destroy( this_ptr_t ptr ) 
{
    so_device::memory::dealloc( ptr ) ;
}

//****************************************************************************************
so_device::result midi_module::register_device( so_device::key_cref_t key, 
    so_device::midi_device_ptr_t dptr ) 
{        
    so_thread::lock_guard_t lk( _mtx ) ;

    {
        auto iter = std::find_if( _devices.begin(), _devices.end(), 
            [&]( this_t::store_data_cref_t item )
            {
                return key == item.key ;
            } ) ;

        if( iter != _devices.end() )
        {
            so_log::log::error( "[so_device::so_win32::midi_module::register_device] : \
                                 key already in use :" + key ) ;
            return so_device::invalid_argument ;
        }
    }

    {
        auto iter = std::find_if( _devices.begin(), _devices.end(), 
            [&]( this_t::store_data_cref_t item )
            {
                return item.dev_ptr == dptr ;
            } ) ;

        if( iter != _devices.end() )
        {
            so_log::log::error( "[so_device::so_win32::midi_module::register_device] : \
                                    device already registered : " + iter->key ) ;
            return so_device::ok ;
        }
    }

    this_t::store_data sd ;
    sd.key = key ;
    sd.dev_ptr = dptr ;
    _devices.push_back( sd ) ;    

    check_handle_for_device( _devices.size()-1 ) ;
    
    return so_device::ok ;
}

//****************************************************************************************
so_device::result midi_module::unregister_device( so_device::key_cref_t key ) 
{
    this_t::store_data_t item ;

    {
        so_thread::lock_guard_t lk( _mtx ) ;

        auto iter = std::find_if( _devices.begin(), _devices.end(),
            [&]( this_t::store_data_cref_t item )
        {
            return item.key == key ;
        } ) ;

        if( iter == _devices.end() )
        {
            so_log::log::error( "[so_device::so_win32::midi_module::unregister_device] : \
                                                              invalid key : " + key ) ;
            return so_device::invalid_argument ;
        }

        item = *iter ;

        _devices.erase( iter ) ;
    }

    return this_t::unregister_device( item ) ;
}

//****************************************************************************************
so_device::midi_device_ptr_t midi_module::find_device( so_device::key_cref_t key ) 
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
so_device::result midi_module::unregister_device( store_data_ref_t item ) 
{
    if( item.mdata.inh != NULL )
    {
        // reset according to msdn. Otherwise midiInClose will fail due to
        // midiInAddBuffer used here.
        {
            auto res = midiInReset( item.mdata.inh ) ;
            so_log::log::error( res != MMSYSERR_NOERROR,
                "[so_device::so_win32::midi_module::unregister_device] : midiInReset" ) ;
        }

        {
            MIDIHDR mhdr ;
            ZeroMemory( &mhdr, sizeof(MIDIHDR) ) ;

            auto res = midiInUnprepareHeader( item.mdata.inh, &mhdr, sizeof(MIDIHDR) ) ;
            so_log::log::error( res != MMSYSERR_NOERROR,
                "[so_device::so_win32::midi_module::unregister_device] : midiInUnprepareHeader" ) ;

            so_device::memory::dealloc_raw( item.mdata.in_buffer ) ;
        }
        {
            auto res = midiInClose( item.mdata.inh ) ;
            so_log::log::error( res != MMSYSERR_NOERROR,
                "[so_device::so_win32::midi_module::unregister_device] : midiInClose" ) ;
        }
        item.mdata.inh = NULL ;
    }

    if( item.mdata.outh != NULL )
    {
        {
            auto res = midiOutShortMsg( item.mdata.outh, byte_t(255) ) ;
            so_log::log::error( res != MMSYSERR_NOERROR,
                "[so_device::so_win32::midi_module::unregister_device] : sending -1 failed" ) ;
        }

        auto res = midiOutClose( item.mdata.outh ) ;
        so_log::log::error( res != MMSYSERR_NOERROR,
            "[so_device::so_win32::midi_module::unregister_device] : midiInClose" ) ;
        item.mdata.outh = NULL ;
    }
    
    return so_device::ok ;
}

//****************************************************************************************
void_t midi_module::update( void_t ) 
{
    // time interval
    // check periodically, so devices can be 
    // connected afterwards
    {
        check_for_new_devices() ;
    }

    for( auto & item : _devices )
    {
        if( item.mdata.inh == NULL || item.mdata.outh == NULL )
            continue ;

        so_device::midi_device_t::midi_messages_t msgs ;
        item.dev_ptr->update( msgs ) ;

        for( auto & msg : msgs )
        {
            MMRESULT res = midiOutShortMsg( item.mdata.outh, (DWORD) midi_message::to_32bit( msg ) ) ;
            so_log::log::error( res != MMSYSERR_NOERROR,
                "[midi_module::transmit_message] : midiOutShortMsg " ) ;
        }
     
        {
            auto res = midiInReset( item.mdata.inh ) ;
            so_log::log::error( res != MMSYSERR_NOERROR,
                "[so_device::so_win32::midi_module::update] : midiInReset" ) ;
        }

        {
            auto res = midiInStart( item.mdata.inh ) ;
            so_log::log::error( res != MMSYSERR_NOERROR,
                "[so_device::so_win32::midi_module::update] : midiInStart" ) ;
        }

        //midiInStart( item.mdata.inh ) ;
        //midiInReset( item.mdata.inh ) ;
    }
}

//****************************************************************************************
void_t midi_module::destroy( void_t ) 
{
    this_t::destroy( this ) ;
}

//****************************************************************************************
void_t midi_module::handle_message( HMIDIIN hin, so_device::midi_message_cref_t msg ) 
{
    for( auto & item : _devices )
    {
        if( item.mdata.inh == hin )
            item.dev_ptr->receive_message( msg ) ;
    }
}

//****************************************************************************************
void_t midi_module::check_handle_for_device( size_t slot ) 
{
    if( slot >= _devices.size() ) 
        return ;
    
    auto & data = _devices[slot] ;
    
    auto * dptr = data.dev_ptr ;
    this_t::midi_data & md = data.mdata ;

    auto di = dptr->get_device_info() ;

    if( md.inh != NULL )
        return ;

    if( md.outh != NULL )
        return ;

    uint_t index_in = uint_t(-1) ;
    for( uint_t i = 0; i < midiInGetNumDevs(); ++i )
    {
        MIDIINCAPS caps ;
        ZeroMemory( &caps, sizeof(MIDIINCAPS) ) ;

        MMRESULT res = midiInGetDevCaps( i, &caps, sizeof(MIDIINCAPS) ) ;
        if( res != MMSYSERR_NOERROR )
        {
            so_log::log::warning( "[so_device::midi_module::check_handle_for_device] : \
                                   unable to retrieve device caps" ) ;
            continue ;
        }

        if( di.device_name == so_std::string_t(caps.szPname) )
        {
            index_in = i ;
            break ;
        }
    }

    if( so_log::log::warning( index_in == uint_t(-1), 
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

        MMRESULT res = midiOutGetDevCaps( i, &caps, sizeof( MIDIOUTCAPS ) ) ;
        if(res != MMSYSERR_NOERROR)
        {
            so_log::log::warning( "[so_device::midi_module::check_handle_for_device] : \
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

        so_log::log::warning( res != MMSYSERR_NOERROR, 
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
            so_log::log::error( prep_res != MMSYSERR_NOERROR, 
                "[so_device::midi_module::check_handle_for_device] : midiInPrepareHeader" ) ;

            auto const add_res = midiInAddBuffer( hmin, &midihdr, sizeof( MIDIHDR ) ) ;
            so_log::log::error( prep_res != MMSYSERR_NOERROR, 
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

            so_log::log::error( res != MMSYSERR_NOERROR, 
                "[so_device::midi_module::check_handle_for_device] : \
                Output MIDI device can not be opended" ) ;
            if( res == MMSYSERR_NOERROR )
            {
                md.outh = hmout ;
            }
        }
    }
}

//****************************************************************************************
void_t midi_module::check_for_new_devices( void_t ) 
{

}
