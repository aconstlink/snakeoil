//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_IMEX_SPLINE_ISPLINE_H_
#define _SNAKEOIL_IMEX_SPLINE_ISPLINE_H_

#include "../typedefs.h"
#include "../result.h"
#include "../protos.h"
#include "../api.h"

namespace so_imex
{
    class SNAKEOIL_IMEX_API ispline
    {
    public:

        virtual void_t destroy( void_t ) = 0 ;
    };
}

#endif
