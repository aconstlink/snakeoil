//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "../device_system.h"

#include "../../modules/win32/midi/midi_module.h"
#include "../../modules/win32/xinput/xinput_module.h"

#include <snakeoil/log/log.h>

using namespace so_device ;

//****************************************************************************************
void_t device_system::create_default_system_modules( void_t ) 
{
    // midi
    {
        auto * midi_mod_ptr = so_device::so_win32::midi_module_t::create(
            "[so_device::device_system::create_default_system_modules] : midi module" ) ;

        auto res = this->register_module( "system", midi_mod_ptr ) ;
        so_log::log::error( so_device::no_success( res ),
            "[so_device::device_system::create_default_system_modules] : midi module" ) ;

        so_log::log::status( so_device::success( res ),
            "[so_device::device_system] : win32 midi module online" ) ;
    }

    // xinput
    {
        auto * xinput_mod_ptr = so_device::so_win32::xinput_module_t::create(
            "[so_device::device_system::create_default_system_modules] : xinput module" ) ;

        auto res = this->register_module( "system", xinput_mod_ptr ) ;
        so_log::log::error( so_device::no_success( res ),
            "[so_device::device_system::create_default_system_modules] : midi module" ) ;

        so_log::log::status( so_device::success( res ),
            "[so_device::device_system] : win32 xinput module online" ) ;
    }    
}
