//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_FLOW_DATA_IDATA_H_
#define _SNAKEOIL_FLOW_DATA_IDATA_H_

#include "../protos.h"
#include "../api.h"
#include "../typedefs.h"
#include "../result.h"

namespace so_flow
{
    class SNAKEOIL_FLOW_API idata
    {

    public:

        virtual ~idata( void_t ) ;

    };
    so_typedef( idata ) ;
}

#endif
