//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_DEVICE_SYSTEM_IDEVICE_SYSTEM_H_
#define _SNAKEOIL_DEVICE_SYSTEM_IDEVICE_SYSTEM_H_

#include "../typedefs.h"
#include "../protos.h"
#include "../result.h"
#include "../api.h"

#include <snakeoil/std/container/vector.hpp>

namespace so_device
{
    class SNAKEOIL_DEVICE_API idevice_system
    {
    public:

        virtual so_device::result register_api( so_device::iapi_ptr_t ) = 0 ;
        virtual so_device::result register_module( so_device::imodule_ptr_t ) = 0 ;

    public: // midi

        virtual void_t install_midi_notify( so_device::imidi_notify_ptr_t ) = 0 ;

        /// returns all available midi device names
        virtual bool_t get_midi_device_names( so_std::vector< so_std::string_t > & names ) = 0 ;

        /// finds a midi device with passed name
        virtual so_device::midi_device_ptr_t find_midi_device( so_std::string_cref_t ) = 0 ;
        
        /// finds the first midi device
        virtual so_device::midi_device_ptr_t find_midi_device( void_t ) = 0 ;
        

    public: // gamepad

        virtual void_t install_gamepad_notify( so_device::igamepad_notify_ptr_t ) = 0 ;

        /// finds the first gamepad device
        virtual so_device::gamepad_device_ptr_t find_gamepad_device( void_t ) = 0 ;
        virtual so_device::so_vgamepad::xbox_360_ptr_t find_xbox360_device( size_t const ) = 0 ;

    public: // keyboard

        /// finds the first keyboard device
        virtual so_device::ascii_keyboard_ptr_t find_ascii_keyboard( void_t ) = 0 ;

    public: // mouse

        /// finds the first mouse device
        virtual so_device::three_button_mouse_ptr_t find_three_button_mouse( void_t ) = 0 ;

    public:

        virtual so_device::result update( void_t ) = 0 ;

    public:

        virtual void_t destroy( void_t ) = 0 ;
    };
}

#endif
