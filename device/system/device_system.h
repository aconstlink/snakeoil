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

        typedef so_std::map< so_std::string_t, so_device::imidi_module_ptr_t > __midi_modules_t ;
        so_typedefs( __midi_modules_t, midi_modules ) ;

        typedef so_std::map< so_std::string_t, so_device::igamepad_module_ptr_t > __gamepad_modules_t ;
        so_typedefs( __gamepad_modules_t, gamepad_modules ) ;

    private:

        so_thread::mutex_t _midi_mtx ;
        midi_modules_t _midis ;

        so_thread::mutex_t _gamepad_mtx ;
        gamepad_modules_t _gamepads ;

    public:

        device_system( void_t ) ;
        device_system( this_rref_t ) ;
        ~device_system( void_t ) ;

    public:

        static this_ptr_t create( so_memory::purpose_cref_t ) ;
        static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
        static void_t destroy( this_ptr_t ) ;

    public:

        virtual so_device::result register_module( so_std::string_cref_t, so_device::imidi_module_ptr_t ) ;
        virtual so_device::result register_module( so_std::string_cref_t, so_device::igamepad_module_ptr_t ) ;

        virtual so_device::result update( void_t ) ;

    public:

        virtual so_device::imidi_module_ptr_t get_midi_module( so_std::string_cref_t ) ;
        virtual so_device::igamepad_module_ptr_t get_gamepad_module( so_std::string_cref_t ) ;

    public: 

        virtual void_t destroy( void_t ) ;

    private:

        void_t create_default_system_modules( void_t ) ;
    };
    so_typedef( device_system ) ;
}

#endif
