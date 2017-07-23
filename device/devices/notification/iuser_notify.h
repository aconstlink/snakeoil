//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_DEVICE_DEVICES_NOTIFICATION_IUSER_NOTIFY_H_
#define _SNAKEOIL_DEVICE_DEVICES_NOTIFICATION_IUSER_NOTIFY_H_

#include "../../typedefs.h"
#include "../../protos.h"
#include "../../api.h"

namespace so_device
{
    class SNAKEOIL_DEVICE_API iuser_notify
    {
    public:

        virtual void_t on_change( so_device::idevice_ptr_t, so_std::string_cref_t name, 
            so_device::so_component::iinput_component_ptr_t ) = 0 ;

        virtual void_t destroy( void_t ) = 0 ;
    };
}
 
#endif
