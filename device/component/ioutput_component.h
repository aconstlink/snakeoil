//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_DEVICE_COMPONENT_IOUTPUT_COMPONENT_H_
#define _SNAKEOIL_DEVICE_COMPONENT_IOUTPUT_COMPONENT_H_

#include "../typedefs.h"
#include "../protos.h"
#include "../result.h"
#include "../api.h"

namespace so_device
{
    namespace so_component
    {
        struct ioutput_component
        {
            virtual void_t destroy( void_t ) = 0 ;
        };
    }
}

#endif
