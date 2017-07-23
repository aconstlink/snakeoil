//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_API_IAPI_H_
#define _SNAKEOIL_API_IAPI_H_

#include "../protos.h"
#include "../typedefs.h"
#include "../api.h"
#include "../result.h"

#include "api_object.h"
#include "api_object_helper.h"
#include "api_types.h"

namespace so_gpu
{
    class SNAKEOIL_GPU_API iapi
    {
    public:

        virtual api_type get_type( void_t ) const = 0 ;
        virtual void_t destroy( void_t ) = 0 ;
    };
}

#endif