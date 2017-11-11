//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "../../typedefs.h"
#include "../../api.h"
#include "../../protos.h"
#include "../../result.h"

#include <snakeoil/flow/protos.h>

namespace so_ui
{
    namespace so_node
    {
        class SNAKEOIL_UI_API variable_node_component
        {
            so_this_typedefs( variable_node_component ) ;

        private:

            /// will be viewed as shared resource
            so_flow::variable_set_ptr_t _var_set_ptr = nullptr ;
            so_flow::variable_node_ptr_t _var_node_ptr = nullptr ;

        public:

            variable_node_component( void_t ) ;
            variable_node_component( this_rref_t ) ;
            variable_node_component( this_cref_t ) = delete ;

            ~variable_node_component( void_t ) ;

        public:

            bool_t reconnect_flow_node( void_t ) ;

            /// allows to connect slots
            so_flow::variable_node_ptr_t get_flow_node( void_t ) ;

            bool_t bind_variable( so_std::string_in_t, so_flow::ivariable_ptr_t ) ;


        public:
            
            this_ref_t operator = ( this_cref_t ) = delete ;
            this_ref_t operator = ( this_rref_t ) ;

        };
        so_typedef( variable_node_component ) ;
    }
}
