//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_DEVICE_API_IGAMEPAD_API_H_
#define _SNAKEOIL_DEVICE_API_IGAMEPAD_API_H_

#include "iapi.h"

#include "../devices/gamepad/gamepad_message.h"

namespace so_device
{
    class SNAKEOIL_DEVICE_API igamepad_api : public iapi
    {
    public:

        /// create all pluged-in devices that are registered
        virtual void_t create_devices( so_device::igamepad_module_ptr_t ) = 0 ;

    public:

        virtual so_device::result register_device( so_device::key_cref_t,
            so_device::gamepad_device_ptr_t ) = 0 ;
        virtual so_device::result unregister_device( so_device::key_cref_t ) = 0 ;

        virtual so_device::gamepad_device_ptr_t find_any_device( void_t ) = 0 ;
        virtual so_device::so_vgamepad::xbox_360_ptr_t find_device( size_t const ) = 0 ;
        
        /// register a passed virtual device for any(first found) gamepad device
        /// if the virtual device is not unregistered, it will be destroyed on 
        /// module destruction time.
        virtual bool_t register_for_any_device( so_device::so_vdev::ivdev_ptr_t ) = 0 ;
        virtual bool_t register_device( size_t const, so_device::so_vdev::ivdev_ptr_t ) = 0 ;
        virtual bool_t unregister_virtual_device( so_device::so_vdev::ivdev_ptr_t ) = 0 ;

        virtual void_t install_gamepad_notify( so_device::igamepad_notify_ptr_t ) = 0 ;

    public:

        virtual void_t update_gamepad( void_t ) = 0 ;
    };
}

#endif
