//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_FLOW_NODE_VARIABLE_VARIABLE_NODE_H_
#define _SNAKEOIL_FLOW_NODE_VARIABLE_VARIABLE_NODE_H_

#include "../node.h"

#include <snakeoil/variable/protos.h>
#include <snakeoil/variable/variable_set/variable_set.h>

namespace so_flow
{
    /// this node serves as a pure slot to
    /// variable exchange mechanism.
    class SNAKEOIL_FLOW_API variable_node : public node
    {
        so_this_typedefs( variable_node ) ;
           
    public:
        
        variable_node( void_t ) ;
        variable_node( this_rref_t ) ;
        virtual ~variable_node( void_t ) ;

    public:

        static this_ptr_t create( so_memory::purpose_cref_t ) ;
        static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
        static void_t destroy( this_ptr_t ) ;

    public:

        so_flow::result create_input_connections( so_var::variable_set_cref_t ) ;
        so_flow::result create_input_connection( so_flow::key_in_t, so_var::ivariable_ptr_t ) ;        

    public: // interface

        virtual bool_t on_update( void_t ) ;
        virtual void_t destroy( void_t ) ;
    };
    so_typedef( variable_node ) ;
}

#endif
