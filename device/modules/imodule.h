//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_DEVICE_MODULES_IMODULE_H_
#define _SNAKEOIL_DEVICE_MODULES_IMODULE_H_

#include "../protos.h"
#include "../typedefs.h"
#include "../api.h"
#include "../result.h"

namespace so_device
{
    class SNAKEOIL_DEVICE_API imodule
    {
    public:

        virtual void_t destroy( void_t ) = 0 ;
    };
}

#endif