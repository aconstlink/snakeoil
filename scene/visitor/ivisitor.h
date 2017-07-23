//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_SCENE_VISITOR_IVISITOR_H_
#define _SNAKEOIL_SCENE_VISITOR_IVISITOR_H_

#include "../result.h"
#include "../protos.h"
#include "../typedefs.h"
#include "../api.h"

namespace so_scene
{
    namespace so_visitor
    {
        class SNAKEOIL_SCENE_API ivisitor
        {
        public: // group / decorator

            virtual so_scene::result visit( so_scene::so_node::node_ptr_t ) = 0 ;
            virtual so_scene::result post_visit( so_scene::so_node::node_ptr_t ) = 0 ;

            virtual so_scene::result visit( so_scene::so_node::group_ptr_t ) = 0 ;
            virtual so_scene::result post_visit( so_scene::so_node::group_ptr_t ) = 0 ;

            virtual so_scene::result visit( so_scene::so_node::transform_3d_ptr_t ) = 0 ;
            virtual so_scene::result post_visit( so_scene::so_node::transform_3d_ptr_t ) = 0 ;

            virtual so_scene::result visit( so_scene::so_node::render_state_ptr_t ) = 0 ;
            virtual so_scene::result post_visit( so_scene::so_node::render_state_ptr_t ) = 0 ;

            virtual so_scene::result visit( so_scene::so_node::decorator_ptr_t ) = 0 ;
            virtual so_scene::result post_visit( so_scene::so_node::decorator_ptr_t ) = 0 ;

        public: // leaf

            virtual so_scene::result visit( so_scene::so_node::leaf_ptr_t ) = 0 ;
            virtual so_scene::result visit( so_scene::so_node::renderable_ptr_t ) = 0 ;
            virtual so_scene::result visit( so_scene::so_node::camera_ptr_t ) = 0 ;

        public:

            virtual void_t destroy( void_t ) = 0 ;
        };
    }
}

#endif
