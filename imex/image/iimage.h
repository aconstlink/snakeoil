//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_IMEX_IMAGE_IIMAGE_H_
#define _SNAKEOIL_IMEX_IMAGE_IIMAGE_H_

#include "../typedefs.h"
#include "../result.h"
#include "../api.h"
#include "../protos.h"

namespace so_imex
{
    class SNAKEOIL_IMEX_API iimage
    {
    public:

        virtual void_t destroy( void_t ) = 0 ;
    };
    so_typedef( iimage )  ;

}

#endif
