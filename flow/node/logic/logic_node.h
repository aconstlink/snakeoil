//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_FLOW_NODE_LOGIC_LOGIC_NODE_H_
#define _SNAKEOIL_FLOW_NODE_LOGIC_LOGIC_NODE_H_

#include "../node.h"


namespace so_flow
{
    class SNAKEOIL_FLOW_API logic_node : public node
    {
        so_this_typedefs( logic_node ) ;

    private:

        so_flow::ilogic_ptr_t _logic_ptr = nullptr ;

    public:

        logic_node( so_flow::ilogic_ptr_t ) ;
        logic_node( this_rref_t ) ;
        virtual ~logic_node( void_t ) ;

    public:

        static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
        static void_t destroy( this_ptr_t ) ;

    public: 

        virtual bool_t on_trigger( void_t ) ;
        virtual bool_t on_update( void_t ) ;
        virtual void_t destroy( void_t ) ;
    };
}

#endif
