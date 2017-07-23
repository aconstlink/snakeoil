//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_FLOW_WALKER_INODE_WALKER_H_
#define _SNAKEOIL_FLOW_WALKER_INODE_WALKER_H_

#include "../typedefs.h"
#include "../protos.h"
#include "../api.h"

#include "../node/inode.h"

namespace so_flow
{
    class SNAKEOIL_FLOW_API inode_walker
    {
    public:

        virtual void_t walk( so_flow::inode::nodes_cref_t ) = 0 ;
        virtual void_t walk( so_flow::inode::nodes_rref_t ) = 0 ;
        virtual void_t destroy( void_t ) = 0 ;
    };

}

#endif
