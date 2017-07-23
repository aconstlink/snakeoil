//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_FLOW_NODE_GENERIC_GENERIC_NODE_H_
#define _SNAKEOIL_FLOW_NODE_GENERIC_GENERIC_NODE_H_

#include "../node.h"

namespace so_flow
{
    class SNAKEOIL_FLOW_API generic_node : public node
    {
        so_this_typedefs( generic_node ) ;

    public:

        typedef std::function< bool_t ( void_t ) > trigger_funk_t ;
        typedef std::function< void_t ( void_t ) > update_funk_t ;

    private:        

        trigger_funk_t _trigger ;
        update_funk_t _update ;

    public:

        generic_node( trigger_funk_t, update_funk_t ) ;
        generic_node( this_rref_t ) ;
        virtual ~generic_node( void_t ) ;

    public:

        static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
        static void_t destroy( this_ptr_t ) ;

    public:

        so_flow::result add_slot( so_flow::key_cref_t, so_flow::iinput_slot_ptr_t ) ;
        so_flow::result add_slot( so_flow::key_cref_t, so_flow::ioutput_slot_ptr_t ) ;

    public:

        virtual bool_t on_trigger( void_t ) ;
        virtual bool_t on_update( void_t ) ;
    };
    so_typedef( generic_node ) ;
}

#endif
