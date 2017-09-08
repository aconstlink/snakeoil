//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_DEVICE_MODULES_MODULE_REGISTRY_H_
#define _SNAKEOIL_DEVICE_MODULES_MODULE_REGISTRY_H_

#include "../typedefs.h"
#include "../protos.h"
#include "../api.h"
#include "../result.h"

#include <snakeoil/thread/mutex.h>
#include <snakeoil/std/container/vector.hpp>

namespace so_device
{
    class SNAKEOIL_DEVICE_API module_registry
    {
        so_this_typedefs( module_registry ) ;

    private:

        so_typedefs( so_std::vector< so_device::imidi_module_ptr_t >, midi_modules ) ;
        so_typedefs( so_std::vector< so_device::igamepad_module_ptr_t >, gamepad_modules ) ;
        so_typedefs( so_std::vector< so_device::ikeyboard_module_ptr_t >, keyboard_modules ) ;
        so_typedefs( so_std::vector< so_device::imouse_module_ptr_t >, mouse_modules ) ;
        so_typedefs( so_std::vector< so_device::imodule_ptr_t >, modules ) ;

        so_thread::mutex_t _mtx_midi ;
        midi_modules_t _midis ;

        so_thread::mutex_t _mtx_gamepad ;
        gamepad_modules_t _gamepads ;

        so_thread::mutex_t _mtx_keyboard ;
        keyboard_modules_t _keyboards ;

        so_thread::mutex_t _mtx_mouses ;
        mouse_modules_t _mouses ;

        so_thread::mutex_t _mtx_mods ;
        modules_t _mods ;

    public:

        module_registry( void_t ) ;
        module_registry( this_cref_t ) = delete ;
        module_registry( this_rref_t ) ;
        ~module_registry( void_t ) ;

    public:

        static this_ptr_t create( so_memory::purpose_cref_t ) ;
        static void_t destroy( this_ptr_t ) ;

    public:

        so_device::result register_module( so_device::imodule_ptr_t ) ;

    public:

        so_device::midi_device_ptr_t create_midi_device( so_std::string_cref_t ) ;

    public:

        void_t create_devices( so_device::imidi_api_ptr_t ) ;

    };
    so_typedef( module_registry ) ;
}

#endif