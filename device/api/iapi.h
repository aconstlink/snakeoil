//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_DEVICE_API_IAPI_H_
#define _SNAKEOIL_DEVICE_API_IAPI_H_

#include "../typedefs.h"
#include "../protos.h"
#include "../result.h"
#include "../api.h"

namespace so_device
{
    class SNAKEOIL_DEVICE_API iapi
    {
    public:

        virtual void_t destroy( void_t ) = 0 ;
    };
    so_typedef( iapi ) ;
}

#endif
