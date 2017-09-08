//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_DEVICE_MODULES_IGAMEPAD_MODULE_H_
#define _SNAKEOIL_DEVICE_MODULES_IGAMEPAD_MODULE_H_

#include "imodule.h"

namespace so_device
{
    class SNAKEOIL_DEVICE_API igamepad_module : public so_device::imodule
    {
    public:

        virtual so_device::midi_device_ptr_t create_gamepad_device( so_std::string_cref_t ) = 0 ;
    };
    so_typedef( igamepad_module ) ;
}

#endif