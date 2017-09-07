//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "../device_system.h"

#include "../../api/win32/midi/win32_midi_api.h"
#include "../../api/win32/xinput/xinput_api.h"

#include <snakeoil/log/log.h>

using namespace so_device ;

//****************************************************************************************
void_t device_system::create_default_system_modules( void_t ) 
{
    // midi
    {
        auto * midi_mod_ptr = so_device::so_win32::win32_midi_api_t::create(
            "[so_device::device_system::create_default_system_modules] : midi module" ) ;

        auto res = this->register_api( midi_mod_ptr ) ;
        so_log::log::error( so_device::no_success( res ),
            "[so_device::device_system::create_default_system_modules] : midi module" ) ;

        so_log::log::status( so_device::success( res ),
            "[so_device::device_system] : win32 midi module online" ) ;
    }

    // xinput
    {
        auto * xinput_mod_ptr = so_device::so_win32::xinput_api_t::create(
            "[so_device::device_system::create_default_system_modules] : xinput module" ) ;

        auto res = this->register_api( xinput_mod_ptr ) ;
        so_log::log::error( so_device::no_success( res ),
            "[so_device::device_system::create_default_system_modules] : midi module" ) ;

        so_log::log::status( so_device::success( res ),
            "[so_device::device_system] : win32 xinput module online" ) ;
    }    
}
