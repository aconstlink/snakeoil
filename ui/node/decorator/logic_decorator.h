//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "../decorator.h"

namespace so_ui
{
    namespace so_node
    {
        class SNAKEOIL_UI_API logic_decorator : public decorator
        {
            typedef decorator base_t ;
            so_this_typedefs( logic_decorator ) ;

        public:

            logic_decorator( void_t ) ;
            logic_decorator( this_rref_t ) ;
            logic_decorator( so_ui::so_node::iparent_ptr_t ) ;
            virtual ~logic_decorator( void_t ) ;

        public:

            static this_ptr_t create( so_memory::purpose_cref_t ) ;
            static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
            static void_t destroy( this_ptr_t ) ;
    
        public:

            void_t set_decorated( so_ui::so_node::node_ptr_t ) ;

        public:

            virtual so_ui::result apply( so_ui::so_visitor::ivisitor_ptr_t ) ;
            virtual void_t destroy( void_t ) ;
        };
        so_typedef( logic_decorator ) ;
    }
}
