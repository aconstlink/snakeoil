//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_IMEX_CAMERA_ILENS_H_
#define _SNAKEOIL_IMEX_CAMERA_ILENS_H_

#include "../api.h"
#include "../result.h"
#include "../typedefs.h"
#include "../protos.h"

namespace so_imex
{
    class SNAKEOIL_IMEX_API ilens
    {
    public:

        virtual void_t destroy( void_t ) = 0 ;
    };
};

#endif
