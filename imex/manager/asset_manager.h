//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_IMEX_MANAGER_ASSET_MANAGER_H_
#define _SNAKEOIL_IMEX_MANAGER_ASSET_MANAGER_H_

#include "../result.h"
#include "../typedefs.h"
#include "../api.h"
#include "../protos.h"

namespace so_imex
{
    class SNAKEOIL_IMEX_API asset_manager
    {
    public:

        virtual void_t destroy( void_t ) = 0 ;
    };
}

#endif
