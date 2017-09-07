//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_DEVICE_SYSTEM_DEVICE_SYSTEM_H_
#define _SNAKEOIL_DEVICE_SYSTEM_DEVICE_SYSTEM_H_

#include "idevice_system.h"

#include <snakeoil/std/container/map.hpp>
#include <snakeoil/std/string/string.hpp>
#include <snakeoil/thread/mutex.h>

namespace so_device
{
    class SNAKEOIL_DEVICE_API device_system : public idevice_system
    {
        so_this_typedefs( device_system ) ;
        
        so_typedefs( so_std::vector<so_device::imidi_api_ptr_t>, midi_apis ) ;
        so_typedefs( so_std::vector<so_device::igamepad_api_ptr_t>, gamepad_apis ) ;

    private:

        so_thread::mutex_t _midi_mtx ;
        midi_apis_t _midis ;

        so_thread::mutex_t _gamepad_mtx ;
        gamepad_apis_t _gamepads ;

        module_registry_ptr_t _mod_reg ;

    public:

        device_system( void_t ) ;
        device_system( this_rref_t ) ;
        ~device_system( void_t ) ;

    public:

        static this_ptr_t create( so_memory::purpose_cref_t ) ;
        static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
        static void_t destroy( this_ptr_t ) ;

    public:

        so_device::result register_api( so_device::imidi_api_ptr_t ) ;
        so_device::result register_api( so_device::igamepad_api_ptr_t ) ;

    public: // interface

        virtual void_t install_midi_notify( so_device::imidi_notify_ptr_t ) ;
        virtual bool_t get_midi_device_names( so_std::vector< so_std::string_t > & names ) ;
        virtual so_device::midi_device_ptr_t find_midi_device( so_std::string_cref_t ) ;
        virtual so_device::midi_device_ptr_t find_midi_device( void_t ) ;
        virtual so_device::gamepad_device_ptr_t find_gamepad_device( void_t ) ;
        virtual so_device::keyboard_device_ptr_t find_keyboard_device( void_t ) ;
        virtual so_device::mouse_device_ptr_t find_mouse_device( void_t ) ;

        virtual so_device::result update( void_t ) ;

        virtual void_t destroy( void_t ) ;

    private:

        void_t create_default_system_modules( void_t ) ;
    };
    so_typedef( device_system ) ;
}

#endif
