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


namespace so_device
{
    class SNAKEOIL_DEVICE_API idevice_system
    {
    public:

        virtual so_device::result register_module( so_std::string_cref_t, 
            so_device::imidi_module_ptr_t ) = 0 ;

        virtual so_device::result register_module( so_std::string_cref_t, 
            so_device::igamepad_module_ptr_t ) = 0 ;

    public:

        virtual so_device::imidi_module_ptr_t get_midi_module( so_std::string_cref_t ) = 0 ;
        virtual so_device::igamepad_module_ptr_t get_gamepad_module( so_std::string_cref_t ) = 0 ;

        virtual so_device::result update( void_t ) = 0 ;

    public:

        virtual void_t destroy( void_t ) = 0 ;
    };
}

#endif
