//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "../device_system.h"

#include "../../api/rtmidi/rtmidi_api.h"
#include "../../api/win32/xinput/xinput_api.h"
#include "../../api/win32/midi/win32_midi_api.h"

#include <snakeoil/log/global.h>

using namespace so_device ;

//****************************************************************************************
void_t device_system::create_default_system_modules( void_t ) 
{
    #if 0
    // midi
    {
        auto * midi_mod_ptr = so_device::so_win32::win32_midi_api::create(
            "[so_device::device_system::create_default_system_modules] : win32_midi_api module" ) ;

        auto res = this->register_api( midi_mod_ptr ) ;
        so_log::global::error( so_device::no_success( res ),
            "[so_device::device_system::create_default_system_modules] : win32_midi_api module" ) ;

        so_log::global::status( so_device::success( res ),
            "[so_device::device_system] : win32_midi_api module online" ) ;
    }
    #endif
    
    // midi
    {
        auto * midi_mod_ptr = so_device::rtmidi_api_t::create(
            "[so_device::device_system::create_default_system_modules] : rtmidi module" ) ;

        auto res = this->register_api( midi_mod_ptr ) ;
        so_log::global::error( so_device::no_success( res ),
            "[so_device::device_system::create_default_system_modules] : rtmidi module" ) ;

        so_log::global::status( so_device::success( res ),
            "[so_device::device_system] : rtmidi module online" ) ;
    }
    

    // xinput
    {
        auto * xinput_mod_ptr = so_device::so_win32::xinput_api_t::create(
            "[so_device::device_system::create_default_system_modules] : xinput module" ) ;

        auto res = this->register_api( xinput_mod_ptr ) ;
        so_log::global::error( so_device::no_success( res ),
            "[so_device::device_system::create_default_system_modules] : midi module" ) ;

        so_log::global::status( so_device::success( res ),
            "[so_device::device_system] : win32 xinput module online" ) ;
    }    
}
