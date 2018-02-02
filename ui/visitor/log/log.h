//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "../visitor.h"

namespace so_ui
{
    namespace so_visitor
    {
        /// implements the ivisitor interface with default
        /// behavior. The default behavior is to do nothing.
        class SNAKEOIL_UI_API log : public visitor
        {
            so_this_typedefs( log ) ;

        public:

            static this_ptr_t create( void_t ) ;
            static void_t destroy( this_ptr_t ) ;

        public:

            virtual so_ui::result visit( so_ui::so_node::node_ptr_t ) ;

            virtual so_ui::result post_visit( so_ui::so_node::node_ptr_t );

            virtual so_ui::result visit( so_ui::so_node::group_ptr_t );

            virtual so_ui::result post_visit( so_ui::so_node::group_ptr_t );

            virtual so_ui::result visit( so_ui::so_node::decorator_ptr_t );

            virtual so_ui::result post_visit( so_ui::so_node::decorator_ptr_t );

            virtual so_ui::result visit( so_ui::so_node::leaf_ptr_t );

            virtual so_ui::result post_visit( so_ui::so_node::leaf_ptr_t );

            virtual void_t destroy( void_t ) ;
        };
        so_typedef( log ) ;
    }
}