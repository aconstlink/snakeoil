//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPX_SERVICE_IUSER_DATA_H_
#define _SNAKEOIL_GPX_SERVICE_IUSER_DATA_H_

#include "../api.h"
#include "../protos.h"
#include "../typedefs.h"

namespace so_gpx
{
    class SNAKEOIL_GPX_API iuser_data
    {
    public:

        virtual void_t destroy( void_t ) = 0 ;
    };
}

#endif