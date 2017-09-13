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
        so_typedefs( so_std::vector<so_device::imouse_api_ptr_t>, mouse_apis ) ;
        so_typedefs( so_std::vector<so_device::ikeyboard_api_ptr_t>, keyboard_apis ) ;
        so_typedefs( so_std::vector<so_device::iapi_ptr_t>, apis ) ;

    private:

        so_thread::mutex_t _midi_mtx ;
        midi_apis_t _midis ;

        so_thread::mutex_t _gamepad_mtx ;
        gamepad_apis_t _gamepads ;

        so_thread::mutex_t _mouse_mtx ;
        mouse_apis_t _mouses ;

        so_thread::mutex_t _keyboard_mtx ;
        keyboard_apis_t _keyboards ;

        so_thread::mutex_t _api_mtx ;
        apis_t _apis ;

        module_registry_ptr_t _mod_reg ;

    public:

        device_system( void_t ) ;
        device_system( this_rref_t ) ;
        ~device_system( void_t ) ;

    public:

        static this_ptr_t create( so_memory::purpose_cref_t ) ;
        static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
        static void_t destroy( this_ptr_t ) ;

    public: // interface

        virtual so_device::result register_api( so_device::iapi_ptr_t ) ;
        virtual so_device::result register_module( so_device::imodule_ptr_t ) ;

        virtual void_t install_midi_notify( so_device::imidi_notify_ptr_t ) ;
        virtual bool_t get_midi_device_names( so_std::vector< so_std::string_t > & names ) ;
        virtual so_device::midi_device_ptr_t find_midi_device( so_std::string_cref_t ) ;
        virtual so_device::midi_device_ptr_t find_midi_device( void_t ) ;
        virtual so_device::gamepad_device_ptr_t find_gamepad_device( void_t ) ;
        virtual so_device::ascii_keyboard_ptr_t find_ascii_keyboard( void_t ) ;
        virtual so_device::three_button_mouse_ptr_t find_three_button_mouse( void_t ) ;

        virtual so_device::result update( void_t ) ;

        virtual void_t destroy( void_t ) ;

    private:

        void_t create_default_system_modules( void_t ) ;
    };
    so_typedef( device_system ) ;
}

#endif
