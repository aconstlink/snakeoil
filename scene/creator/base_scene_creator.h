//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_SCENE_CREATOR_BASE_SCENE_CREATOR_H_
#define _SNAKEOIL_SCENE_CREATOR_BASE_SCENE_CREATOR_H_

#include "../typedefs.h"
#include "../result.h"
#include "../api.h"
#include "../protos.h"

#include "scene_creator_context.h"

#include <snakeoil/imex/protos.h>
#include <snakeoil/imex/typedefs.h>

#include <snakeoil/gfx/camera/icamera.h>

#include <snakeoil/manager/protos.h>
#include <snakeoil/manager/graphics/geometry/geometry_manager.h>

#include <snakeoil/thread/protos.h>

namespace so_scene
{
    class SNAKEOIL_SCENE_API base_scene_creator
    {
        so_this_typedefs( base_scene_creator ) ;

    public:

        struct init_package
        {

            so_imex::graph_manager_ptr_t graph_mgr_ptr = nullptr ;
            so_imex::image_manager_ptr_t image_mgr_ptr = nullptr ;
            so_imex::mesh_manager_ptr_t mesh_mgr_ptr = nullptr ;
            so_imex::camera_manager_ptr_t cam_mgr_ptr = nullptr ;

            so_manager::manager_registry_ptr_t rt_mreg_ptr = nullptr ;
        };
        so_typedef( init_package ) ;

    private:

        init_package _init_pkg ;

        so_manager::key_t _default_program_key ;

        so_scene::so_node::node_ptr_t _node_ptr = nullptr ;

        // add processing heavy tasks to this graph. It will be executed at the end
        // of the execute method.
        so_thread::itask_ptr_t _task_root_ptr ;

    protected:

        init_package_ref_t ip( void_t ) { return _init_pkg ; }

    protected:

        base_scene_creator( void_t ) ;
        base_scene_creator( init_package_cref_t ) ;
        base_scene_creator( this_rref_t ) ;

    public:

        virtual ~base_scene_creator( void_t ) ;

    public:

        /// no images if not already loaded
        so_scene::so_node::node_ptr_t execute( so_imex::key_cref_t ) ;

        so_scene::so_node::node_ptr_t execute_graph_creation( so_imex::key_cref_t ) ;

        void_t set_default_program_key( so_manager::key_cref_t ) ;

        so_scene::so_node::node_ptr_t get_node( void_t ) ;

    private:

        void_t prepare_scene( so_imex::so_node::node_ptr_t,
            so_scene::scene_creator_context_ptr_t ) ;

        so_scene::so_node::node_ptr_t inspect_node( so_imex::so_node::node_ptr_t,
            so_scene::scene_creator_context_ptr_t ) ;
        so_scene::so_node::leaf_ptr_t inspect_leaf( so_imex::so_node::node_ptr_t,
            so_scene::scene_creator_context_ptr_t ) ;
        so_scene::so_node::group_ptr_t inspect_group( so_imex::so_node::node_ptr_t,
            so_scene::scene_creator_context_ptr_t ) ;
        so_scene::so_node::decorator_ptr_t inspect_decorator( so_imex::so_node::node_ptr_t,
            so_scene::scene_creator_context_ptr_t ) ;

    protected:

        virtual void_t on_prepare_scene( so_imex::so_node::node_ptr_t, so_thread::itask_ptr_t, 
            so_scene::scene_creator_context_ptr_t ) = 0 ;
        virtual so_scene::so_node::group_ptr_t on_create_renderable( so_imex::so_node::mesh_ptr_t, 
            so_scene::scene_creator_context_ptr_t ) = 0 ;

    protected:

        bool_t handle_mesh_asset( so_imex::so_node::mesh_asset_ptr_t,
            so_manager::so_graphics::geometry_manager::handle_out_t,
            so_scene::scene_creator_context_ptr_t ) ;

        so_scene::so_node::camera_ptr_t handle_camera_asset(
            so_imex::so_node::camera_asset_ptr_t,
            so_scene::scene_creator_context_ptr_t ) ;
    };
}

#endif
