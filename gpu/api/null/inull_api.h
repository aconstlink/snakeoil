//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_API_NULL_INULL_API_H_
#define _SNAKEOIL_GPU_API_NULL_INULL_API_H_

#include "../iapi.h"

namespace so_gpu
{
    namespace so_null
    {
        class SNAKEOIL_GPU_API inull_api : public iapi
        {};
        so_typedef( inull_api ) ;
    }
}

#endif