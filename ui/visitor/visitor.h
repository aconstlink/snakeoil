//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "ivisitor.h"

#include "../node/leaf.h"

namespace so_ui
{
    namespace so_visitor
    {
        /// implements the ivisitor interface with default
        /// behavior. The default behavior is to do nothing.
        class SNAKEOIL_UI_API visitor : public ivisitor
        {
        public:

            virtual so_ui::result visit( so_ui::so_node::node_ptr_t )
                { return so_ui::ok ; }

            virtual so_ui::result post_visit( so_ui::so_node::node_ptr_t )
                { return so_ui::ok ; }

            virtual so_ui::result visit( so_ui::so_node::group_ptr_t gptr ) 
            {
                return this->visit( so_ui::so_node::node_ptr_t( gptr ) ) ;
            }

            virtual so_ui::result post_visit( so_ui::so_node::group_ptr_t gptr )
            {
                return this->post_visit( so_ui::so_node::node_ptr_t( gptr ) ) ;
            }

            virtual so_ui::result visit( so_ui::so_node::decorator_ptr_t dptr ) 
            {
                return this->visit( so_ui::so_node::node_ptr_t( dptr ) ) ;
            }

            virtual so_ui::result post_visit( so_ui::so_node::decorator_ptr_t dptr ) 
            {
                return this->post_visit( so_ui::so_node::node_ptr_t( dptr ) ) ;
            }

            virtual so_ui::result visit( so_ui::so_node::leaf_ptr_t lptr )
            {
                return this->visit( so_ui::so_node::node_ptr_t( lptr ) ) ;
            }

            virtual so_ui::result post_visit( so_ui::so_node::leaf_ptr_t lptr ) 
            {
                return this->post_visit( so_ui::so_node::node_ptr_t( lptr ) ) ;
            }
        };
    }
}