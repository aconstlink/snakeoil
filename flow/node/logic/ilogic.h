//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_FLOW_NODE_LOGIC_ILOGIC_H_
#define _SNAKEOIL_FLOW_NODE_LOGIC_ILOGIC_H_

#include "../../typedefs.h"
#include "../../api.h"
#include "../../protos.h"

namespace so_flow
{
    class SNAKEOIL_FLOW_API ilogic
    {
    public:

        virtual void_t destroy( void_t ) = 0 ;
        virtual bool_t on_trigger( so_flow::inode_ptr_t ) = 0 ;
        virtual bool_t on_update( so_flow::inode_ptr_t ) = 0 ;

    };

}

#endif
