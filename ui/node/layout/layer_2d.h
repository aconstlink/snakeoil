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
        class SNAKEOIL_UI_API layer_2d : public decorator
        {
            typedef decorator base_t ;
            so_this_typedefs( layer_2d ) ;

        private:

            size_t _layer = size_t( -1 )  ;

        public:

            layer_2d( void_t ) ;
            layer_2d( this_rref_t ) ;
            layer_2d( so_ui::so_node::iparent_ptr_t ) ;
            virtual ~layer_2d( void_t ) ;

        public:

            static this_ptr_t create( so_memory::purpose_cref_t ) ;
            static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
            static void_t destroy( this_ptr_t ) ;

        public:

            void_t set_decorated( so_ui::so_node::node_ptr_t ) ;

            size_t get_layer( void_t ) const { return _layer ; }

        public:

            virtual so_ui::result apply( so_ui::so_visitor::ivisitor_ptr_t ) ;
            virtual void_t destroy( void_t ) ;
        };
        so_typedef( layer_2d ) ;
    }
}
