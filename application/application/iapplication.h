//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_APPLICATION_APPLICATION_IAPPLICATION_H_
#define _SNAKEOIL_APPLICATION_APPLICATION_IAPPLICATION_H_

#include "../api.h"
#include "../protos.h"
#include "../result.h"
#include "../typedefs.h"

namespace so_app
{
    class SNAKEOIL_APPLICATION_API iapplication
    {
    public:

        virtual so_app::result destroy( void_t ) = 0 ;
        virtual so_app::result exec( void_t ) = 0 ;
    };
}

#endif

