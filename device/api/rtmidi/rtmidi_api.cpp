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

#include <RtMidi.h>

#include <algorithm>

using namespace so_device ;
using namespace so_device::so_win32 ;

struct rtmidi_api::data_pimpl 
{
    so_this_typedefs( data_pimpl ) ;

    uint_t pid = uint_t( -1 );
    RtMidiIn * midiin = nullptr ;
    RtMidiOut * midiout = nullptr ;

    data_pimpl( uint_t const pid_ ) : pid( pid_ )
    {
        try
        {
            midiin = new RtMidiIn()  ;             
            midiout = new RtMidiOut() ;
        
        } catch( RtMidiError & err )
        {
            so_log::global_t::error( "[rtmidi_api::data_pimpl] " + so_std::string_t( err.what() ) ) ;
        }
    }

    data_pimpl( this_cref_t ) = delete ;

    data_pimpl( this_rref_t rhv )
    {
        pid = rhv.pid ;
        so_move_member_ptr( midiin, rhv ) ;
        so_move_member_ptr( midiout, rhv ) ;
    }

    ~data_pimpl( void_t )
    {
        delete midiin ;
        delete midiout ;
    }
} ;

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

    so_move_member_ptr( _global_self_ptr, rhv ) ;
    _global_self_ptr->self_ptr = this ;

    _midi_notifies = std::move( rhv._midi_notifies ) ;
}

//****************************************************************************************
rtmidi_api::~rtmidi_api( void_t ) 
{
    for( auto * ptr : _midi_notifies )
    {
        ptr->destroy() ;
    }

    for( auto & item : _devices )
    {
        so_memory::global_t::dealloc( item.mptr ) ;
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
            this_t::store_data_t sd ;
            sd.key = name ;
            sd.dev_ptr = mdev ;
            sd.mptr = so_memory::global_t::alloc( std::move( data_pimpl(uint_t(-1)) ), 
                "[rtmidi_api::create_devices] : pimpl" ) ;

            {
                auto mi = RtMidiIn() ;
                for( uint_t pid = 0; pid < mi.getPortCount(); ++pid )
                {
                    if( mi.getPortName(pid) == name )
                    {
                        sd.mptr->midiin->openPort( pid ) ;
                        break ;
                    }
                }
            }

            {
                auto mo = RtMidiOut() ;
                for( uint_t pid = 0; pid < mo.getPortCount(); ++pid )
                {
                    if( mo.getPortName(pid) == name )
                    {
                        sd.mptr->midiout->openPort( pid ) ;
                        break ;
                    }
                }
            }
            _devices.push_back( std::move( sd )  ) ;

            //check_handle_for_device( _devices.size() - 1 ) ;
        }
    }
}

//****************************************************************************************
void_t rtmidi_api::get_device_names( so_std::vector< so_std::string_t > & names_out ) const
{
    auto midiin = RtMidiIn() ;
    

    for( uint_t i = 0; i < midiin.getPortCount(); ++i )
    {
        names_out.push_back( midiin.getPortName(i) ) ;
    }
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

    for( auto & item : _devices )
    {
        so_device::midi_device_t::midi_messages_t msgs ;

        while( true )
        {
            std::vector< uchar_t > message ;
            double_t const ts = item.mptr->midiin->getMessage( &message ) ;


            if( message.size() == 0 ) break ;

            if( message.size() == 3 )
            {
                message.push_back( 0 ) ;
            }

            if( message.size() != 4 ) 
            {
                so_log::global_t::error( "[rtmidi_api::update_midi] : size muste be 4 at the moment" ) ;
                continue ;
            }

            auto const midi_msg = so_device::midi_message( ( byte_t ) message[ 0 ], ( byte_t ) message[ 1 ],
                ( byte_t ) message[ 2 ], ( byte_t ) message[ 3 ] ) ;

            //msgs.push_back( so_device::midi_device_t::midi_message() )
            for( auto * ptr : _midi_notifies )
            {
                ptr->on_message( item.key, midi_msg ) ;
            }

            item.dev_ptr->receive_message( midi_msg ) ;
        }

        {
            so_device::midi_device_t::midi_messages_t msgs_out ;
            item.dev_ptr->update( msgs_out ) ;

            for( auto const msg_out : msgs_out )
            {
                std::vector< uchar_t > message( 3 ) ;
                message[ 0 ] = msg_out.status ;
                message[ 1 ] = msg_out.byte1 ;
                message[ 2 ] = msg_out.byte2 ;
                //message[ 3 ] = msg_out.byte3 ;
                item.mptr->midiout->sendMessage( &message ) ;
            }
        }
    }
}

//****************************************************************************************
void_t rtmidi_api::destroy( void_t ) 
{
    this_t::destroy( this ) ;
}

//****************************************************************************************
void_t rtmidi_api::check_for_new_devices( void_t ) 
{

}
