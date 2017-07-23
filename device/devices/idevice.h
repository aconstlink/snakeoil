//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_DEVICE_DEVICES_IDEVICE_H_
#define _SNAKEOIL_DEVICE_DEVICES_IDEVICE_H_

#include "../typedefs.h"
#include "../result.h"
#include "../api.h"
#include "../protos.h"

namespace so_device
{
    class SNAKEOIL_DEVICE_API idevice
    {
    public:

        virtual void_t destroy( void_t ) = 0 ;
    };
}

#endif
