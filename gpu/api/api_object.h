//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_API_OBJECT_H_
#define _SNAKEOIL_GPU_API_OBJECT_H_

#include "../typedefs.h"

namespace so_gpu
{
    /// more like a carrier object for vanishing the 
    /// real driver object which only belongs to the driver.
    class api_object
    { 
        virtual void_t dummy( void_t ) const {}
    } ;
}
    
#endif

