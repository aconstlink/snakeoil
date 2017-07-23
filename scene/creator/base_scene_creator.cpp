//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "base_scene_creator.h"

#include "../node/node.h"
#include "../node/decorator.h"
#include "../node/group.h"
#include "../node/leaf.h"
#include "../node/leaf/logic_leaf.h"
#include "../node/group/logic_group.h"
#include "../node/transform/transform_3d.h"
#include "../node/decorator/logic_decorator.h"
#include "../node/render/renderable.h"
#include "../node/camera/camera.h"

#include "../converter/geometry_binding_converter.h"

#include <snakeoil/imex/node/mesh/mesh.h>
#include <snakeoil/imex/node/mesh/mesh_asset.h>
#include <snakeoil/imex/node/image/images.h>
#include <snakeoil/imex/node/image/image_asset.h>
#include <snakeoil/imex/node/material/material_asset.h>
#include <snakeoil/imex/node/group/logic_group.h>
#include <snakeoil/imex/node/decorator/transform_3d.h>
#include <snakeoil/imex/node/leaf.h>
#include <snakeoil/imex/node/group.h>
#include <snakeoil/imex/node/node.h>
#include <snakeoil/imex/node/camera/camera_asset.h>
#include <snakeoil/imex/manager/graph_manager.h>
#include <snakeoil/imex/manager/mesh_manager.h>
#include <snakeoil/imex/manager/image_manager.h>
#include <snakeoil/imex/camera/generic_camera.h>
#include <snakeoil/imex/camera/pinhole_lens.h>
#include <snakeoil/imex/image/image.h>

#include <snakeoil/shade/code/ascii_code.h>
#include <snakeoil/shade/material/standard_material.h>
#include <snakeoil/shade/generator/glsl/effect_generator.h>

#include <snakeoil/gfx/camera/generic_camera.h>
#include <snakeoil/gfx/camera/pinhole_lens.h>

#include <snakeoil/gpu/program/config.h>
#include <snakeoil/gpu/variable/variable_set.h>

#include <snakeoil/manager/code/code_manager.h>
#include <snakeoil/manager/camera/camera_manager.h>
#include <snakeoil/manager/graphics/geometry/geometry_manager.h>
#include <snakeoil/manager/graphics/shader/shader_manager.h>
#include <snakeoil/manager/graphics/shader/program_manager.h>
#include <snakeoil/manager/graphics/variable/variable_manager.h>
#include <snakeoil/manager/graphics/imagery/image_creator.h>
#include <snakeoil/manager/graphics/imagery/image_manager.h>
#include <snakeoil/manager/graphics/imagery/texture_manager.h>
#include <snakeoil/manager/registry/manager_registry.h>

#include <snakeoil/geometry/mesh/polygon_mesh.h>
#include <snakeoil/geometry/mesh/tri_mesh.h>
#include <snakeoil/geometry/mesh/flat_tri_mesh.h>

#include <snakeoil/variable/variable_set/variable_set.h>

#include <snakeoil/thread/task/tasks.h>
#include <snakeoil/thread/task/async_executor.h>

#include <snakeoil/log/log.h>

#include <snakeoil/math/utility/3d/look_at.hpp>

#include <snakeoil/core/macros/move.h>
#include <snakeoil/core/assert.h>
#include <snakeoil/core/break_if.hpp>

using namespace so_scene ;


//*************************************************************************************
base_scene_creator::base_scene_creator( void_t ) 
{
}

//*************************************************************************************
base_scene_creator::base_scene_creator( init_package_cref_t ip ) : _init_pkg(ip)
{

}

//*************************************************************************************
base_scene_creator::base_scene_creator( this_rref_t rhv ) 
{
    _init_pkg = std::move( rhv._init_pkg ) ;

    _default_program_key = std::move( rhv._default_program_key ) ;
    so_move_member_ptr( _node_ptr, rhv ) ;

    so_move_member_ptr( _task_root_ptr, rhv ) ;
}

//*************************************************************************************
base_scene_creator::~base_scene_creator( void_t ) 
{

}

//*************************************************************************************
so_scene::so_node::node_ptr_t base_scene_creator::get_node( void_t ) 
{
    return _node_ptr ;
}

//*************************************************************************************
so_scene::so_node::node_ptr_t base_scene_creator::execute( so_imex::key_cref_t graph_key ) 
{
    return this_t::execute_graph_creation( graph_key ) ;
}

//*************************************************************************************
so_scene::so_node::node_ptr_t base_scene_creator::execute_graph_creation( 
    so_imex::key_cref_t graph_key ) 
{
    if( this_t::ip().graph_mgr_ptr == nullptr )
        return nullptr ;

    so_imex::graph_manager_t::handle_t ghnd ;
    {
        auto const res = this_t::ip().graph_mgr_ptr->acquire( graph_key,
        "[so_scene::base_scene_creator::execute_graph_creation]", ghnd ) ;
        if( so_core::is_not(res) )
        {
            so_log::log::error( "[so_scene::base_scene_creator::execute_graph_creation] : \
                                 \r graph not found : " + graph_key ) ;
            return nullptr ;
        }
    }

    so_imex::so_node::node_ptr_t to_be_traversed = ghnd->data_ptr ;

    if( so_log::log::warning( to_be_traversed == nullptr, 
        "[so_scene::create_scene_graph::execute] : Graph does not exist with key : " + graph_key ) )
        return nullptr ;
    
    _task_root_ptr = so_thread::void_funk_task_t::create( [=]( void_t ){},
        "[base_scene_creator::execute_graph_creation] : root task" ) ;

    so_scene::scene_creator_context_t ctx ;

    this_t::prepare_scene( to_be_traversed, &ctx ) ;

    // execute parallel stuff
    {
        so_thread::async_executor ae ;
        ae.consume_and_wait( _task_root_ptr ) ;
    }

    _node_ptr = this_t::inspect_node( to_be_traversed, &ctx ) ;    

    return _node_ptr ;
}

//*************************************************************************************
void_t base_scene_creator::set_default_program_key( so_manager::key_cref_t key_in ) 
{
    _default_program_key = key_in ;
}

//*************************************************************************************
so_scene::so_node::node_ptr_t base_scene_creator::inspect_node( 
    so_imex::so_node::node_ptr_t node_in_ptr, so_scene::scene_creator_context_ptr_t ctx ) 
{
    if( node_in_ptr == nullptr ) return nullptr ;

    so_scene::so_node::node_ptr_t nout_ptr = nullptr ;

    if( so_imex::so_node::node::is_of<so_imex::so_node::decorator>(node_in_ptr) )
    {
        nout_ptr = this_t::inspect_decorator( node_in_ptr, ctx ) ;
    }
    else if( so_imex::so_node::node::is_of<so_imex::so_node::group>(node_in_ptr) )
    {
        nout_ptr = this_t::inspect_group( node_in_ptr, ctx ) ;
    }
    else if( so_imex::so_node::node::is_of<so_imex::so_node::leaf>(node_in_ptr) )
    {
        nout_ptr = this_t::inspect_leaf( node_in_ptr,ctx  ) ;
    }
    return nout_ptr ;
}

//*************************************************************************************
so_scene::so_node::group_ptr_t base_scene_creator::inspect_group( 
    so_imex::so_node::node_ptr_t node_in_ptr, so_scene::scene_creator_context_ptr_t ctx ) 
{
    if( node_in_ptr == nullptr ) return nullptr ;

    so_scene::so_node::group_ptr_t nout_ptr = nullptr  ;

    if( so_imex::so_node::node::is_of<so_imex::so_node::mesh>(node_in_ptr) )
    {
        // make render config from mesh and material
        auto * in_mesh_ptr = so_imex::so_node::node::just_cast
            <so_imex::so_node::mesh>(node_in_ptr) ;

        nout_ptr = this->on_create_renderable( in_mesh_ptr, ctx ) ;
    }
    else if( so_imex::so_node::node::is_of<so_imex::so_node::logic_group>(node_in_ptr) )
    {
        auto * in_group_ptr = so_imex::so_node::node::just_cast
            <so_imex::so_node::logic_group>(node_in_ptr) ;

        auto * new_group_ptr = so_scene::so_node::logic_group_t::create(
            "[so_scene::base_scene_creator::inspect_group] : logic group") ;

        for( size_t i=0; i<in_group_ptr->get_num_children(); ++i )
        {
            auto * new_node_ptr = this_t::inspect_node( in_group_ptr->get_child(i), ctx ) ;
            if( so_log::log::warning(new_node_ptr == nullptr,
                "[so_scene::base_scene_creator::inspect_group] : node inspection failed") )
                continue ;

            new_group_ptr->add_child( new_node_ptr ) ;
        }

        if( so_log::log::warning( new_group_ptr->get_num_children() == 0,
            "[so_scene::base_scene_creator::inspect_group] : no children in group => will be omitted") )
        {
            new_group_ptr->destroy() ;
            new_group_ptr = nullptr ;
        }
        nout_ptr = new_group_ptr ;
    }

    return nout_ptr ;
}

//*************************************************************************************
so_scene::so_node::decorator_ptr_t base_scene_creator::inspect_decorator( 
    so_imex::so_node::node_ptr_t node_in_ptr, so_scene::scene_creator_context_ptr_t ctx ) 
{
    if( node_in_ptr == nullptr ) return nullptr ;

    so_scene::so_node::decorator_ptr_t nout_ptr = nullptr ;

    /*
    if( so_scene::so_node::node::is_of<so_scene::so_node::render_state>(node_in_ptr) )
    {
        auto * in_deco_ptr = so_scene::so_node::node::just_cast
            <so_scene::so_node::render_state>(node_in_ptr) ;

        auto * decorated_ptr = this_t::inspect_node( in_deco_ptr->get_decorated() ) ;

        if( decorated_ptr != nullptr )
        {
            auto * new_deco_ptr = so_scene::so_node::render_state::create( 
            so_scene::so_node::render_state(*in_deco_ptr),
            "[so_scene::base_scene_creator::inspect_decorator] : render_state" ) ;

            new_deco_ptr->set_decorated( decorated_ptr ) ;

            nout_ptr = new_deco_ptr ;
        }
    }
    else*/ if( so_imex::so_node::node::is_of<so_imex::so_node::transform_3d>(node_in_ptr) )
    {
        auto * in_deco_ptr = so_imex::so_node::node::just_cast
            <so_imex::so_node::transform_3d>(node_in_ptr) ;

        auto * decorated_ptr = this_t::inspect_node( in_deco_ptr->get_decorated(), ctx ) ;
        
        if( decorated_ptr != nullptr )
        {
            so_math::so_3d::trafof_t other_trafo = in_deco_ptr->get_transformation() ;

            auto * new_deco_ptr = so_scene::so_node::transform_3d_t::create( 
            so_scene::so_node::transform_3d_t(other_trafo),
            "[so_scene::base_scene_creator::inspect_decorator] : transform_3d_t" ) ;

            new_deco_ptr->set_decorated( decorated_ptr ) ;

            nout_ptr = new_deco_ptr ;
        }
    }
    else
    {
        auto * in_deco_ptr = so_imex::so_node::node::just_cast
            <so_imex::so_node::decorator_t>(node_in_ptr) ;

        auto * decorated_ptr = this_t::inspect_node( in_deco_ptr->get_decorated(), ctx ) ;
        if( decorated_ptr != nullptr )
        {
            auto * new_deco_ptr = so_scene::so_node::logic_decorator_t::create( 
                "[so_scene::base_scene_creator::inspect_decorator] : logic_decorator" ) ;

            new_deco_ptr->set_decorated( decorated_ptr ) ;
            nout_ptr = new_deco_ptr ;
        }
    }

    return nout_ptr ;
}

//*************************************************************************************
so_scene::so_node::leaf_ptr_t base_scene_creator::inspect_leaf( 
    so_imex::so_node::node_ptr_t node_in_ptr, so_scene::scene_creator_context_ptr_t ctx ) 
{
    if( node_in_ptr == nullptr ) return nullptr ;

    so_scene::so_node::leaf_ptr_t nout_ptr = nullptr ;

    if( so_imex::so_node::node::is_of<so_imex::so_node::mesh_asset_t>(node_in_ptr) )
    {
        nout_ptr = nullptr ;
    }
    else if( so_imex::so_node::node::is_of<so_imex::so_node::camera_asset_t>(node_in_ptr) )
    {
        nout_ptr = this_t::handle_camera_asset( 
            so_imex::so_node::node::just_cast<so_imex::so_node::camera_asset_t>(node_in_ptr),
            ctx) ;
    }
    return nout_ptr ;
}

//*************************************************************************************
bool_t base_scene_creator::handle_mesh_asset( so_imex::so_node::mesh_asset_ptr_t mnode_ptr,
    so_manager::so_graphics::geometry_manager::handle_out_t hnd_out,
    so_scene::scene_creator_context_ptr_t) 
{
    if( so_log::log::error( mnode_ptr == nullptr, 
        "[base_scene_creator::handle_mesh_asset] : nullptr" ) ) 
        return false ;
    
    auto const res = _init_pkg.rt_mreg_ptr->gpu_geometry_manager()->acquire( 
        mnode_ptr->get_key(), "[so_scene::base_scene_creator::handle_mesh_asset]", hnd_out ) ;

    if( so_log::log::error( so_core::is_not(res), 
        "[base_scene_creator::handle_mesh_asset]" ) )
        return false ;

    return true ;
}

//*************************************************************************************
so_scene::so_node::camera_ptr_t base_scene_creator::handle_camera_asset( 
    so_imex::so_node::camera_asset_ptr_t cam_in_ptr,
    so_scene::scene_creator_context_ptr_t) 
{
    auto * in_cam_node_ptr = so_imex::so_node::node::just_cast
        <so_imex::so_node::camera_asset_t>(cam_in_ptr) ;

    so_imex::icamera_ptr_t icam_ptr = in_cam_node_ptr->get_camera() ;
    if( so_log::log::warning( icam_ptr == nullptr, 
        "[so_imex::base_scene_creator::inspect_leaf] : no camera in camera node" ) )
        return nullptr ;
        
    so_scene::so_node::camera_ptr_t cam_node_out_ptr = nullptr ;

    if( dynamic_cast<so_imex::generic_camera_ptr_t>(icam_ptr) != nullptr )
    {
        so_gfx::generic_camera_ptr_t gfx_cam = so_gfx::generic_camera::create(
            "[so_scene::base_scene_creator::handle_camera_asset] : generic_camera") ;

        auto * cam_ptr = static_cast<so_imex::generic_camera_ptr_t>(icam_ptr) ;    
        for( size_t i=0; i<cam_ptr->get_num_lenses(); ++i )
        {
            if( dynamic_cast<so_imex::pinhole_lens_cptr_t>( cam_ptr->get_lens(i) ) != nullptr )
            {
                auto * imex_lens = static_cast<so_imex::pinhole_lens_cptr_t>(cam_ptr->get_lens(i)) ;

                so_gfx::pinhole_lens_ptr_t phole_lens_ptr = so_gfx::pinhole_lens::create(
                    "[so_scene::base_scene_creator::handle_camera_asset] : pinhole_lens" ) ;

                // camera matrix
                // do not look at a particular point.
                {
                    so_math::mat4f_t mat ;
                    so_math::vec3f_t pos ;
                    so_math::vec3f_t right = cam_ptr->get_right() ;
                    so_math::vec3f_t up = cam_ptr->get_up_vector() ;
                    so_math::vec3f_t look = cam_ptr->get_look() ;

                    so_math::so_3d::create_lookat_from_vec( pos, look, up, right, mat ) ;
                    phole_lens_ptr->set_camera_matrix( mat ) ;
                }

                // projection
                phole_lens_ptr->create_perspective_fov( 
                     imex_lens->get_fovv(), imex_lens->get_aspect(), 
                     imex_lens->get_near(), imex_lens->get_far() ) ;

                 gfx_cam->add_lens( phole_lens_ptr ) ;
            }
            else
            {
                so_log::log::warning( "[so_scene::base_scene_creator::handle_camera_asset] : \
                                        unknown camera lens" ) ;
            }
        }

        {
            so_imex::key_t key ;
            if( cam_in_ptr->get_key(key) )
            {
                so_manager::camera_manager_t::manage_params_t mp ;
                mp.data_ptr = gfx_cam ;

                auto res = _init_pkg.rt_mreg_ptr->camera_manager()->insert( key, mp ) ;
                so_log::log::error( so_manager::no_success( res ),
                    "[so_scene::base_scene_creator::handle_camera_asset] : manage camera" ) ;
            }            
        }

        cam_node_out_ptr = so_scene::so_node::camera::create( 
            so_scene::so_node::camera( gfx_cam ), 
            "[base_scene_creator::handle_camera_asset] : camera node" ) ;
    }        

    return cam_node_out_ptr ;
}

void_t base_scene_creator::prepare_scene( so_imex::so_node::node_ptr_t node_in_ptr,
    so_scene::scene_creator_context_ptr_t ctx ) 
{
    
    this->on_prepare_scene( node_in_ptr, _task_root_ptr, ctx ) ;

    if( so_imex::so_node::node::is_of<so_imex::so_node::mesh>(node_in_ptr) )
    {
        so_imex::so_node::mesh_ptr_t mesh_ptr =
            static_cast<so_imex::so_node::mesh_ptr_t>( node_in_ptr ) ;

        so_imex::so_node::mesh_asset_ptr_t mnode_ptr = mesh_ptr->get_asset() ;

        so_geo::imesh_cptr_t m_ptr = mnode_ptr->get_mesh() ;

        if( dynamic_cast<so_geo::polygon_mesh_cptr_t>( m_ptr ) != nullptr )
        {
            so_thread::void_funk_task_ptr_t t = so_thread::void_funk_task_t::create(
                [=]( void_t )
            {
                so_geo::polygon_mesh_cptr_t poly_mesh_ptr =
                    static_cast<so_geo::polygon_mesh_cptr_t>( m_ptr ) ;

                so_geo::flat_tri_mesh_t ftm ;
                poly_mesh_ptr->flatten( ftm ) ;

                auto const rres = _init_pkg.rt_mreg_ptr->gpu_geometry_manager()->construct_from( ftm, mnode_ptr->get_key() ) ;
                if( rres == so_manager::ok )
                    _init_pkg.rt_mreg_ptr->gpu_geometry_manager()->upload( mnode_ptr->get_key() ) ;

            }, "[base_scene_creator::handle_mesh_asset] : handle polygon_mesh" ) ;

            _task_root_ptr->then( t ) ;
        }
        else if( dynamic_cast<so_geo::tri_mesh_cptr_t>( m_ptr ) != nullptr )
        {
            so_thread::void_funk_task_ptr_t t = so_thread::void_funk_task_t::create(
                [=]( void_t )
            {
                so_geo::tri_mesh_cptr_t tri_mesh_ptr =
                    static_cast<so_geo::tri_mesh_cptr_t>( m_ptr ) ;

                so_geo::flat_tri_mesh_t ftm ;
                tri_mesh_ptr->flatten( ftm ) ;

                auto const rres = _init_pkg.rt_mreg_ptr->gpu_geometry_manager()->construct_from( ftm, mnode_ptr->get_key() ) ;
                
                if( rres == so_manager::ok )
                    _init_pkg.rt_mreg_ptr->gpu_geometry_manager()->upload( mnode_ptr->get_key() ) ;
            }, "[base_scene_creator::handle_mesh_asset] : handle tri_mesh_cptr_t" ) ;

            _task_root_ptr->then( t ) ;
        }
        else if( dynamic_cast<so_geo::flat_tri_mesh_cptr_t>( m_ptr ) != nullptr )
        {
            so_thread::void_funk_task_ptr_t t = so_thread::void_funk_task_t::create(
                [=]( void_t )
            {
                so_geo::flat_tri_mesh_cptr_t tri_mesh_ptr =
                    static_cast<so_geo::flat_tri_mesh_cptr_t>( m_ptr ) ;

                auto const rres = _init_pkg.rt_mreg_ptr->gpu_geometry_manager()->construct_from( *tri_mesh_ptr, mnode_ptr->get_key() ) ;
                if( rres == so_manager::ok )
                    _init_pkg.rt_mreg_ptr->gpu_geometry_manager()->upload( mnode_ptr->get_key() ) ;
            }, "[base_scene_creator::handle_mesh_asset] : handle flat_tri_mesh_cptr_t" ) ;

            _task_root_ptr->then( t ) ;                
        }
    }
    else if( so_imex::so_node::node::is_of<so_imex::so_node::decorator>( node_in_ptr ) )
    {
        so_imex::so_node::decorator_ptr_t deco = 
            static_cast<so_imex::so_node::decorator_ptr_t>(node_in_ptr) ;
        this_t::prepare_scene( deco->get_decorated(), ctx ) ;
    }
    else if( so_imex::so_node::node::is_of<so_imex::so_node::group>( node_in_ptr ) )
    {
        so_imex::so_node::group_ptr_t group =
            static_cast<so_imex::so_node::group_ptr_t>( node_in_ptr ) ;
        for( size_t i=0; i<group->get_num_children(); ++i )
        {
            this_t::prepare_scene( group->get_child(i), ctx ) ;
        }
    }
}
