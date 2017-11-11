//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "ivisitor.h"

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

            virtual so_ui::result visit( so_ui::so_node::group_ptr_t ) 
                { return so_ui::ok ; }

            virtual so_ui::result post_visit( so_ui::so_node::group_ptr_t )
                { return so_ui::ok ; }

            virtual so_ui::result visit( so_ui::so_node::decorator_ptr_t ) 
                { return so_ui::ok ; }

            virtual so_ui::result post_visit( so_ui::so_node::decorator_ptr_t ) 
                { return so_ui::ok ; }

            virtual so_ui::result visit( so_ui::so_node::leaf_ptr_t )
                { return so_ui::ok ; }

            virtual so_ui::result post_visit( so_ui::so_node::leaf_ptr_t ) 
                { return so_ui::ok ; }

            virtual so_ui::result visit( so_ui::so_node::transform_2d_ptr_t )
                { return so_ui::ok ; }

            virtual so_ui::result post_visit( so_ui::so_node::transform_2d_ptr_t )
                { return so_ui::ok ; }

            virtual so_ui::result visit( so_ui::so_node::render_state_ptr_t ) 
                { return so_ui::ok ; }

            virtual so_ui::result post_visit( so_ui::so_node::render_state_ptr_t ) 
                { return so_ui::ok ; }

            virtual so_ui::result visit( so_ui::so_node::renderable_ptr_t )
                { return so_ui::ok ; }

            virtual so_ui::result visit( so_ui::so_node::row_layout_ptr_t ) 
                { return so_ui::ok ; }
            virtual so_ui::result post_visit( so_ui::so_node::row_layout_ptr_t ) 
                { return so_ui::ok ; }

            virtual so_ui::result visit( so_ui::so_node::column_layout_ptr_t ) 
                { return so_ui::ok ; }

            virtual so_ui::result post_visit( so_ui::so_node::column_layout_ptr_t ) 
                { return so_ui::ok ; }

            virtual so_ui::result visit( so_ui::so_node::area_2d_ptr_t )
                { return so_ui::ok ; }

            virtual so_ui::result post_visit( so_ui::so_node::area_2d_ptr_t )
                { return so_ui::ok ; }

            virtual so_ui::result visit( so_ui::so_node::input_event_ptr_t )
                { return so_ui::ok ; }

            virtual so_ui::result post_visit( so_ui::so_node::input_event_ptr_t )
                { return so_ui::ok ; }

            virtual so_ui::result visit( so_ui::so_node::layer_2d_ptr_t ) 
                { return so_ui::ok ; }

            virtual so_ui::result post_visit( so_ui::so_node::layer_2d_ptr_t ) 
                { return so_ui::ok ; }
        };
    }
}