//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_DEVICE_VDEV_IVDEV_H_
#define _SNAKEOIL_DEVICE_VDEV_IVDEV_H_

#include "../typedefs.h"
#include "../protos.h"
#include "../api.h"
#include "../result.h"

namespace so_device
{
    namespace so_vdev
    {
        class SNAKEOIL_DEVICE_API ivdev
        {
        public:

            virtual void_t reset( void_t ) = 0 ;
            virtual void_t destroy( void_t ) = 0 ;
        };
    }
}

#endif
