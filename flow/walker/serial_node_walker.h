//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_FLOW_WALKER_SERIAL_NODE_WALKER_H_
#define _SNAKEOIL_FLOW_WALKER_SERIAL_NODE_WALKER_H_

#include "inode_walker.h"

namespace so_flow
{
    class SNAKEOIL_FLOW_API serial_node_walker : public inode_walker
    {
        so_this_typedefs( serial_node_walker ) ;

    private:

        serial_node_walker( this_cref_t ) {}

    public:

        serial_node_walker( void_t ) ;
        serial_node_walker( this_rref_t ) ;
        virtual ~serial_node_walker( void_t ) ;

    public:

        static this_ptr_t create( so_memory::purpose_cref_t ) ;
        static void_t destroy( this_ptr_t ) ;

    public:

        virtual void_t walk( so_flow::inode::nodes_cref_t ) ;
        virtual void_t walk( so_flow::inode::nodes_rref_t ) ;
        virtual void_t destroy( void_t ) ;
    };
    so_typedef( serial_node_walker ) ;
}

#endif
