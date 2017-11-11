//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "../result.h"
#include "../protos.h"
#include "../typedefs.h"
#include "../api.h"

namespace so_ui
{
    namespace so_visitor
    {
        class SNAKEOIL_UI_API ivisitor
        {
        public: // group / decorator

            virtual so_ui::result visit( so_ui::so_node::node_ptr_t ) = 0 ;
            virtual so_ui::result post_visit( so_ui::so_node::node_ptr_t ) = 0 ;

            virtual so_ui::result visit( so_ui::so_node::group_ptr_t ) = 0 ;
            virtual so_ui::result post_visit( so_ui::so_node::group_ptr_t ) = 0 ;

            virtual so_ui::result visit( so_ui::so_node::transform_2d_ptr_t ) = 0 ;
            virtual so_ui::result post_visit( so_ui::so_node::transform_2d_ptr_t ) = 0 ;

            virtual so_ui::result visit( so_ui::so_node::render_state_ptr_t ) = 0 ;
            virtual so_ui::result post_visit( so_ui::so_node::render_state_ptr_t ) = 0 ;

            virtual so_ui::result visit( so_ui::so_node::decorator_ptr_t ) = 0 ;
            virtual so_ui::result post_visit( so_ui::so_node::decorator_ptr_t ) = 0 ;

            virtual so_ui::result visit( so_ui::so_node::row_layout_ptr_t ) = 0 ;
            virtual so_ui::result post_visit( so_ui::so_node::row_layout_ptr_t ) = 0 ;

            virtual so_ui::result visit( so_ui::so_node::column_layout_ptr_t ) = 0 ;
            virtual so_ui::result post_visit( so_ui::so_node::column_layout_ptr_t ) = 0 ;

            virtual so_ui::result visit( so_ui::so_node::area_2d_ptr_t ) = 0 ;
            virtual so_ui::result post_visit( so_ui::so_node::area_2d_ptr_t ) = 0 ;

            virtual so_ui::result visit( so_ui::so_node::input_event_ptr_t ) = 0 ;
            virtual so_ui::result post_visit( so_ui::so_node::input_event_ptr_t ) = 0 ;

            virtual so_ui::result visit( so_ui::so_node::layer_2d_ptr_t ) = 0 ;
            virtual so_ui::result post_visit( so_ui::so_node::layer_2d_ptr_t ) = 0 ;

        public: // leaf

            virtual so_ui::result visit( so_ui::so_node::leaf_ptr_t ) = 0 ;
            virtual so_ui::result visit( so_ui::so_node::renderable_ptr_t ) = 0 ;

        public:

            virtual void_t destroy( void_t ) = 0 ;
        };
    }
}
