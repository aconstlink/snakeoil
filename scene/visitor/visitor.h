//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "ivisitor.h"

namespace so_scene
{
    namespace so_visitor
    {
        /// implements the ivisitor interface with default
        /// behavior. The default behavior is to do nothing.
        class SNAKEOIL_SCENE_API visitor : public ivisitor
        {
        public:

            virtual so_scene::result visit( so_scene::so_node::node_ptr_t )
                { return so_scene::ok ; }

            virtual so_scene::result post_visit( so_scene::so_node::node_ptr_t )
                { return so_scene::ok ; }

            virtual so_scene::result visit( so_scene::so_node::group_ptr_t ) 
                { return so_scene::ok ; }

            virtual so_scene::result post_visit( so_scene::so_node::group_ptr_t )
                { return so_scene::ok ; }

            virtual so_scene::result visit( so_scene::so_node::decorator_ptr_t ) 
                { return so_scene::ok ; }

            virtual so_scene::result post_visit( so_scene::so_node::decorator_ptr_t ) 
                { return so_scene::ok ; }

            virtual so_scene::result visit( so_scene::so_node::leaf_ptr_t )
                { return so_scene::ok ; }

            virtual so_scene::result post_visit( so_scene::so_node::leaf_ptr_t ) 
                { return so_scene::ok ; }

            virtual so_scene::result visit( so_scene::so_node::transform_3d_ptr_t )
                { return so_scene::ok ; }

            virtual so_scene::result post_visit( so_scene::so_node::transform_3d_ptr_t )
                { return so_scene::ok ; }

            virtual so_scene::result visit( so_scene::so_node::render_state_ptr_t ) 
                { return so_scene::ok ; }

            virtual so_scene::result post_visit( so_scene::so_node::render_state_ptr_t ) 
                { return so_scene::ok ; }

            virtual so_scene::result visit( so_scene::so_node::renderable_ptr_t )
                { return so_scene::ok ; }

            virtual so_scene::result visit( so_scene::so_node::camera_ptr_t ) 
                { return so_scene::ok ; }
        };
    }
}
