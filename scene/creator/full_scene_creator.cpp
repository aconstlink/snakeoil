//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "full_scene_creator.h"

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
#include <snakeoil/shade/material/effect_material.h>
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

#include <snakeoil/log/log.h>

#include <snakeoil/core/execute_if.hpp>
#include <snakeoil/core/macros/move.h>
#include <snakeoil/core/assert.h>
#include <snakeoil/core/break_if.hpp>

using namespace so_scene ;


//*************************************************************************************
full_scene_creator::full_scene_creator( void_t ) 
{}

//*************************************************************************************
full_scene_creator::full_scene_creator( init_package_cref_t mgr_pkg ) : base_scene_creator( mgr_pkg )
{
}

//*************************************************************************************
full_scene_creator::full_scene_creator( this_rref_t rhv ) 
{}

//*************************************************************************************
full_scene_creator::~full_scene_creator( void_t ) 
{}

//*************************************************************************************
full_scene_creator::this_ptr_t full_scene_creator::create( so_memory::purpose_cref_t purp ) 
{
    return so_scene::memory::alloc( this_t(), purp ) ;
}

//************************************************************************************* 
full_scene_creator::this_ptr_t full_scene_creator::create( this_rref_t rhv, so_memory::purpose_cref_t purp ) 
{
    return so_scene::memory::alloc( std::move(rhv), purp ) ;
}

//*************************************************************************************
void_t full_scene_creator::destroy( this_ptr_t ptr ) 
{
    so_scene::memory::dealloc( ptr ) ;
}

//*************************************************************************************
void_t full_scene_creator::on_prepare_scene( 
    so_imex::so_node::node_ptr_t nptr, so_thread::itask_ptr_t task_ptr,
    so_scene::scene_creator_context_ptr_t ctx ) 
{
    if( so_imex::so_node::node::is_of<so_imex::so_node::mesh>( nptr ) )
    {
        so_thread::itask_ptr_t t = so_thread::void_funk_task_t::create( [=]( void_t )
        {
            so_imex::so_node::mesh_ptr_t mn_ptr =
                static_cast<so_imex::so_node::mesh_ptr_t>( nptr ) ;

            so_imex::so_node::mesh_asset_ptr_t mesh_asset_ptr = mn_ptr->get_asset() ;
            so_imex::so_node::material_asset_ptr_t mat_asset_ptr = mn_ptr->get_material() ;            

            if( so_log::log::warning( so_core::is_nullptr( mat_asset_ptr ),
                "[so_scene::base_scene_creator::inspect_group] : no material for mesh : " +
                mesh_asset_ptr->get_key() ) )
                return ;

            so_manager::key_t const mesh_key = mesh_asset_ptr->get_key() ;
            so_imex::key_t const material_key = mat_asset_ptr->get_key() ;
            
            if( so_core::is_not( ctx->reserve_effect(material_key) ) )
            {
                // do not process effects that are already added to the context
                return ;
            }

            so_shade::so_shader::effect_t effect ;

            so_shade::so_material::imaterial_ptr_t mat_ptr = mat_asset_ptr->get_material() ;
            if( so_core::can_cast<so_shade::so_material::standard_material_ptr_t>( mat_ptr ) )
            {
                so_shade::so_material::standard_material_ptr_t sm_ptr = 
                    static_cast<so_shade::so_material::standard_material_ptr_t>( mat_ptr ) ;

                so_shade::so_generator::so_glsl::generator_properties_t gp ;
                so_shade::so_generator::so_glsl::effect_generator eg( gp ) ;

                auto const res = eg.generate_effect_from( sm_ptr, effect ) ;
                if( so_shade::no_success( res ) )
                {
                    so_log::log::error( "[full_scene_creator::inspect_group] : "
                        "unable to generate effect from material" ) ;
                }
            }
            else if( so_core::can_cast<so_shade::so_material::effect_material_ptr_t>( mat_ptr ) )
            {
                auto * eff_mat_ptr = static_cast<so_shade::so_material::effect_material_ptr_t>(mat_ptr) ;
                effect = *eff_mat_ptr->get_effect() ;
            }
            else
            {
                so_log::log::warning( "[so_scene::full_scene_creator::inspect_group] : "
                    "unknown material type: " + material_key ) ;
                return ;
            }

            so_manager::key_list_t vs_keys ;
            so_manager::key_list_t ps_keys ;

            
            // handle vertex shader
            {
                so_std::string_t loc_key = material_key + ".vs" ;
                if( so_core::is_not( this_t::ip().rt_mreg_ptr->code_manager()->has_code(loc_key) ) )
                {
                    auto const ires = this_t::ip().rt_mreg_ptr->code_manager()->add_code(
                        loc_key, effect.get_vertex_shader()->get_code().get_code() ) ;

                    so_log::log::error( so_manager::no_success( ires ),
                        "[so_scene::full_scene_creator::inspect_group] : "
                        "can not rt manage vertex shader : " + loc_key ) ;
                }
                

                vs_keys.push_back( loc_key ) ;
            }

            // handle pixel shader
            {
                so_std::string_t loc_key = material_key + ".ps" ;
                if( so_core::is_not(this_t::ip().rt_mreg_ptr->code_manager()->has_code( loc_key )) )
                {
                    auto const ires = this_t::ip().rt_mreg_ptr->code_manager()->add_code(
                        loc_key, effect.get_pixel_shader()->get_code().get_code() ) ;

                    so_log::log::error( so_manager::no_success( ires ),
                        "[so_scene::full_scene_creator::inspect_group] : "
                        "can not rt manage vertex shader : " + loc_key ) ;
                }

                ps_keys.push_back( loc_key ) ;
            }

            bool_t vs_compilation = false ;
            bool_t ps_compilation = false ;

            // 2. define shaders by it's code and compile
            {
                if( so_core::is_nullptr( this_t::ip().rt_mreg_ptr->gpu_shader_manager()->
                    get_vertex_shader( material_key ) ) )
                {
                    this_t::ip().rt_mreg_ptr->gpu_shader_manager()->compose_and_compile_shader(
                        so_gpu::shader_type::vertex_shader, material_key,
                        std::move( vs_keys ) ) ;

                    vs_compilation = true ;
                }

                if( ps_keys.size() != 0 )
                {
                    if( so_core::is_nullptr( this_t::ip().rt_mreg_ptr->gpu_shader_manager()->
                        get_pixel_shader( material_key ) ) )
                    {
                        this_t::ip().rt_mreg_ptr->gpu_shader_manager()->compose_and_compile_shader(
                            so_gpu::shader_type::pixel_shader, material_key,
                            std::move( ps_keys ) ) ;

                        ps_compilation = true ;
                    }
                }
            }

            if( vs_compilation )
            {
                auto const ires = this_t::ip().rt_mreg_ptr->gpu_shader_manager()->
                    wait_for_compilation( so_gpu::shader_type::vertex_shader, material_key ) ;

                so_log::log::error( so_gpu::no_success( ires ),
                    "[so_scene::full_scene_creator::inspect_group] : vertex shader compilation failed" ) ;
            }

            if( ps_compilation )
            {
                auto const ires = this_t::ip().rt_mreg_ptr->gpu_shader_manager()->
                    wait_for_compilation( so_gpu::shader_type::pixel_shader, material_key ) ;

                so_log::log::error( so_gpu::no_success( ires ),
                    "[so_scene::full_scene_creator::inspect_group] : pixel shader compilation failed" ) ;
            }

            // 3. define the program with it's shaders
            if( so_core::is_nullptr( this_t::ip().rt_mreg_ptr->gpu_program_manager()->
                get_program( material_key ) ) )
            {
                so_manager::so_graphics::iprogram_manager::shader_keys skeys ;
                skeys.vs_key = material_key ;

                if( ps_keys.size() != 0 )
                    skeys.ps_key = material_key ;

                this_t::ip().rt_mreg_ptr->gpu_program_manager()->compose_and_link_program(
                    material_key, skeys ) ;

                auto const ires = this_t::ip().rt_mreg_ptr->gpu_program_manager()->wait_for_link(
                    material_key ) ;

                so_log::log::error( so_gpu::no_success( ires ),
                    "[so_scene::full_scene_creator::inspect_group] : gpu program link failed" ) ;
            }

            
            {
                so_shade::so_shader::effect_ptr_t eptr = so_shade::so_shader::effect_t::create(
                    std::move( effect ), "[so_scene::full_scene_creator::inspect_group] : effect" ) ;

                if( so_core::is_not(ctx->set_reserved_effect( material_key, eptr ) ) )
                {
                    so_shade::so_shader::effect_t::destroy( eptr ) ;
                    so_log::log::error("[full_scene_creator::on_prepare_scene] : " 
                        "failed to set reserved effect") ;
                }
            }

        }, "[full_scene_creator::on_prepare_scene]" ) ;

        task_ptr->then( t ) ;
    }
}

//*************************************************************************************
so_scene::so_node::group_ptr_t full_scene_creator::on_create_renderable( 
    so_imex::so_node::mesh_ptr_t in_mesh_ptr, so_scene::scene_creator_context_ptr_t ctx ) 
{
    
    so_scene::so_node::renderable_ptr_t renderable_to_be_created_ptr = nullptr ;

    so_imex::so_node::mesh_asset_ptr_t mesh_asset_ptr = in_mesh_ptr->get_asset() ;
    so_imex::so_node::material_asset_ptr_t mat_asset_ptr = in_mesh_ptr->get_material() ;

    so_manager::key_t const mesh_key = mesh_asset_ptr->get_key() ;
    so_imex::key_t const material_key = mat_asset_ptr->get_key() ;

    // handle mesh asset
    {
        so_manager::so_graphics::geometry_manager::handle_t hnd ;
        if( so_core::is_not( this_t::handle_mesh_asset( mesh_asset_ptr, hnd, ctx ) ) )
        {
            // use dummy geometry
            so_log::log::error( "[so_scene::base_scene_creator::inspect_group] : "
                                "geometry does not exist : " + mesh_key ) ;
        }

        {
            so_shade::so_shader::effect_ptr_t effect_ptr ;
            if( so_core::is_not( ctx->get_effect( material_key, effect_ptr ) ) )
            {
                so_log::log::error( "[so_scene::full_scene_creator::on_create_renderable] : "
                    "effect not found : " + material_key ) ;

                return nullptr ;
            }
            
            so_scene::so_node::renderable_t renderable ;
            
            so_gpu::config_ptr_t config_ptr = nullptr ;

            // if no material was specified, use the default shader program
            so_manager::key_t prog_key = material_key ;

            so_manager::key_t config_key =
                this_t::ip().rt_mreg_ptr->gpu_program_manager()->
                compose_unique_config_key( prog_key ) ;

            if( so_manager::success( this_t::ip().rt_mreg_ptr->gpu_program_manager()->
                compose_config( config_key, prog_key ) ) )
            {
                config_ptr = this_t::ip().rt_mreg_ptr->gpu_program_manager()->
                    get_config( config_key ) ;
                config_ptr->bind( hnd->pt, hnd->vb, hnd->ib ) ;
                
                {
                    so_shade::so_shader::position_binding pb ;
                    if( effect_ptr->get_vertex_shader()->get( pb ) )
                    {
                        config_ptr->bind( so_gpu::vertex_attribute::position, pb.var.name ) ;
                    }
                }

                {
                    so_shade::so_shader::normal_binding pb ;
                    if( effect_ptr->get_vertex_shader()->get( pb ) )
                    {
                        config_ptr->bind( so_gpu::vertex_attribute::normal, pb.var.name ) ;
                    }
                }

                {
                    size_t const num = effect_ptr->get_vertex_shader()->get_num_texcood_bindings() ;
                    for( size_t i = 0; i < num; ++i )
                    {
                        so_shade::so_shader::texcoord_binding tb ;
                        if( effect_ptr->get_vertex_shader()->get_texcoord_binding( i, tb ) )
                        {
                            auto va = so_gpu::texcoord_vertex_attribute_by( i ) ;
                            config_ptr->bind( va, tb.var.name ) ;
                        }
                    }
                }

                renderable.set_config( config_ptr ) ;

                
            }

            so_gpu::variable_set_ptr_t var_set_ptr = nullptr ;

            //
            // create variable sets
            //
            so_core::execute_if( true, [&]( void_t )
            {
                so_std::string_t const var_set_key = config_key ;

                auto const res = this_t::ip().rt_mreg_ptr->gpu_variable_manager()->
                    create_variable_set( var_set_key,
                    "[so_scene::base_scene_creator::inspect_group] : varialbe_set" ) ;

                so_log::log::warning( so_manager::no_success( res ),
                    "[so_scene::base_scene_creator::inspect_group] : varialbe_set with key : " +
                    var_set_key ) ;

                if( so_manager::no_success( res ) )
                    return false ;
                
                var_set_ptr = this_t::ip().rt_mreg_ptr->gpu_variable_manager()->
                    get_variable_set( var_set_key ) ;

                // object bindings
                {
                    size_t const num = effect_ptr->get_vertex_shader()->get_num_object_bindings() ;
                    for( size_t i = 0; i < num; ++i )
                    {
                        so_shade::so_shader::object_binding ob ;
                        if( effect_ptr->get_vertex_shader()->get_object_binding( i, ob ) )
                        {
                            switch( ob.binding_point )
                            {
                            case so_shade::so_shader::object_binding_point::object_matrix:
                                {
                                    auto * vptr = so_var::variable<so_math::mat4f_t>::create( 
                                        so_math::mat4f_t().identity(),
                                        "[full_scene_creator::inspect_group] : renderable varialbe object matrix" ) ;

                                    var_set_ptr->bind_data( ob.var.name, vptr->get_data_ptr() ) ;
                                    renderable.bind_object_matrix( ob.var.name, vptr ) ;
                                }
                                break ;

                            case so_shade::so_shader::object_binding_point::world_matrix:
                                {
                                    auto * vptr = so_var::variable<so_math::mat4f_t>::create( 
                                        so_math::mat4f_t().identity(),
                                        "[full_scene_creator::inspect_group] : renderable varialbe world matrix" ) ;

                                    var_set_ptr->bind_data( ob.var.name, vptr->get_data_ptr() ) ;
                                    renderable.bind_world_matrix( ob.var.name, vptr ) ;
                                }
                                break ;
                            default:
                                so_log::log::warning(
                                    "[full_scene_creator::inspect_group] : missed object binding " +
                                    ob.var.name ) ;
                                break ;
                            }
                        }
                    }
                }

                // camera bindings
                {
                    size_t const num = effect_ptr->get_vertex_shader()->get_num_camera_bindings() ;
                    for( size_t i = 0; i < num; ++i )
                    {
                        so_shade::so_shader::camera_binding cb ;
                        if( effect_ptr->get_vertex_shader()->get_camera_binding( i, cb ) )
                        {
                            switch( cb.binding_point )
                            {
                            case so_shade::so_shader::camera_binding_point::view_matrix:
                                {
                                    auto * vptr = so_var::variable<so_math::mat4f_t>::create( 
                                        so_math::mat4f_t(),
                                        "[full_scene_creator::inspect_group] : renderable varialbe view matrix" ) ;

                                    var_set_ptr->bind_data( cb.var.name, vptr->get_data_ptr() ) ;
                                    renderable.bind_view_matrix( cb.var.name, vptr ) ;
                                }
                                break ;
                            case so_shade::so_shader::camera_binding_point::proj_matrix:
                                {
                                    auto * vptr = so_var::variable<so_math::mat4f_t>::create( 
                                        so_math::mat4f_t(),
                                        "[full_scene_creator::inspect_group] : renderable varialbe proj matrix" ) ;

                                    var_set_ptr->bind_data( cb.var.name, vptr->get_data_ptr() ) ;
                                    renderable.bind_proj_matrix( cb.var.name, vptr ) ;
                                }
                                break ;
                            case so_shade::so_shader::camera_binding_point::mvp_matrix:
                                {
                                    auto * vptr = so_var::variable<so_math::mat4f_t>::create(
                                        so_math::mat4f_t(),
                                        "[full_scene_creator::inspect_group] : renderable varialbe mvp matrix" ) ;

                                    var_set_ptr->bind_data( cb.var.name, vptr->get_data_ptr() ) ;
                                    renderable.bind_mvp_matrix( cb.var.name, vptr ) ;
                                }
                                break ;

                            case so_shade::so_shader::camera_binding_point::proj_info_0:
                                {
                                    auto * vptr = so_var::variable<so_math::vec4f_t>::create( 
                                        so_math::vec4f_t(),
                                        "[full_scene_creator::inspect_group] : renderable variable proj info" ) ;

                                    var_set_ptr->bind_data( cb.var.name, vptr->get_data_ptr() ) ;
                                    renderable.bind_proj_param0( cb.var.name, vptr ) ;
                                }
                                break ;

                            default:
                                so_log::log::warning(
                                    "[full_scene_creator::inspect_group] : missed camera binding " +
                                    cb.var.name ) ;
                                break ;
                            }
                        }
                    }
                }

                // custom variables with default values
                {
                }

                // image variables
                {
                    size_t const num = effect_ptr->get_pixel_shader()->get_num_texture_bindings() ;
                    for( size_t i = 0; i < num; ++i )
                    {
                        so_shade::so_shader::texture_binding_t tb ;
                        if( so_core::is_not(effect_ptr->get_pixel_shader()->get_texture_binding( i, tb )) )
                            continue ;

                        so_std::string_t binding_name ;
                        if( so_core::is_not(effect_ptr->get_pixel_shader_binding( tb.var, binding_name )) )
                            continue ;

                        // get image from mesh_ptr
                        so_imex::so_node::image_asset_ptr_t image_ass_ptr ;
                        if( so_core::is_not(in_mesh_ptr->get_image( binding_name, image_ass_ptr )) )
                        {
                            so_log::log::warning( so_std::string_t(
                                "[so_scene::full_scene_creator::on_create_renderable] : "
                                "no image in mesh" ) + binding_name ) ;
                        }

                        so_manager::so_graphics::image_manager_t::i2d_handle_t i2d_hnd ;

                        if( so_core::is_not(this_t::ip().rt_mreg_ptr->gpu_image_manager()->
                            acquire( image_ass_ptr->get_key(),
                                "[so_scene::full_scene_creator::on_create_renderable]", i2d_hnd )) )
                        {
                            so_imex::iimage_ptr_t imex_iimage_ptr =
                                image_ass_ptr->get_image();

                            so_log::log::error_and_exit(
                                dynamic_cast<so_imex::image_ptr_t>(imex_iimage_ptr) == nullptr,
                                so_std::string_t( "[so_scene::full_scene_creator::on_create_renderable] : "
                                    "image type not supported : " ) + image_ass_ptr->get_key() ) ;

                            auto * imex_img_ptr = dynamic_cast<so_imex::image_ptr_t>(imex_iimage_ptr);
                            so_manager::so_graphics::image_creator img_creator( imex_img_ptr );
                            so_gpu::iimage_2d_ptr_t gpu_img_ptr = img_creator.move_image_2d();

                            so_log::log::error_and_exit( gpu_img_ptr == nullptr,
                                so_std::string_t( "[so_scene::full_scene_creator::on_create_renderable] : "
                                    "image type not supported : " ) + image_ass_ptr->get_key() ) ;


                            so_manager::so_graphics::image_manager_t::i2d_manage_params_t i2d_mp ;
                            i2d_mp.obj_ptr = gpu_img_ptr ;
                            this_t::ip().rt_mreg_ptr->gpu_image_manager()->insert(
                                image_ass_ptr->get_key(), i2d_mp );

                            this_t::ip().rt_mreg_ptr->gpu_image_manager()->
                                acquire( image_ass_ptr->get_key(),
                                    "[so_scene::full_scene_creator::on_create_renderable]", i2d_hnd ) ;

                            {
                                so_gpu::texture_2d_ptr_t tex_ptr = so_gpu::texture_2d::create(
                                    so_gpu::texture_2d_t( gpu_img_ptr ),
                                    "[so_scene::full_scene_creator::on_create_renderable] : texture 2d" );

                                tex_ptr->set_texture_wrap( so_gpu::texture_wrap_type::repeat,
                                    so_gpu::texture_wrap_type::repeat ) ;

                                so_manager::so_graphics::texture_manager_t::t2d_manage_params_t mp ;
                                mp.obj_ptr = tex_ptr ;

                                this_t::ip().rt_mreg_ptr->gpu_texture_manager()->insert(
                                    image_ass_ptr->get_key(), mp );

                                auto const res_upload = this_t::ip().rt_mreg_ptr->gpu_image_manager()->
                                    alloc_and_upload_image_2d( i2d_hnd );
                                so_core::break_if( so_manager::no_success( res_upload ) );
                            }
                        }

                        // bind image
                        {
                            so_manager::so_graphics::texture_manager_t::t2d_handle_t ihnd ;

                            this_t::ip().rt_mreg_ptr->gpu_texture_manager()->
                                acquire( image_ass_ptr->get_key(),
                                    "[so_scene::base_scene_creator::on_create_renderable]", ihnd ) ;
                            var_set_ptr->bind_texture( tb.var.name, ihnd->obj_ptr ) ;
                        }

                    }
                }
                
                renderable.add_variable_set( var_set_ptr ) ;

                return true ;
            } ) ;

            renderable_to_be_created_ptr = so_scene::so_node::renderable::create(
                std::move(renderable), "[so_scene::base_scene_creator::inspect_group] : renderable" ) ;
        }

        
#if 0
            // create image
            if( so_core::is_not_nullptr( mat_ptr ) &&
                so_core::is_not_nullptr( mat_ptr->get_image( 0 ) ) )
            {
                auto * image_ass_ptr = mat_ptr->get_image( 0 ) ;

                so_imex::key_t key = image_ass_ptr->get_key() ;

                if( so_manager::no_success( this_t::ip().rt_mreg_ptr->gpu_image_manager()->
                    has_image_2d( key ) ) )
                {
                    so_imex::iimage_ptr_t imex_iimage_ptr =
                        image_ass_ptr->get_image( this_t::ip().image_mgr_ptr );

                    so_assert( dynamic_cast<so_imex::image_ptr_t>( imex_iimage_ptr ) != nullptr );

                    auto * imex_img_ptr = dynamic_cast<so_imex::image_ptr_t>( imex_iimage_ptr );
                    so_manager::so_graphics::image_creator img_creator( imex_img_ptr );
                    so_gpu::iimage_2d_ptr_t gpu_img_ptr = img_creator.move_image_2d();

                    so_assert( gpu_img_ptr != nullptr );

                    this_t::ip().rt_mreg_ptr->gpu_image_manager()->manage( key, gpu_img_ptr );

                    {
                        so_gpu::texture_2d_ptr_t tex_ptr = so_gpu::texture_2d::create(
                            so_gpu::texture_2d_t( gpu_img_ptr ), "[] : texture 2d" );

                        this_t::ip().rt_mreg_ptr->gpu_texture_manager()->manage( key, tex_ptr );
                        auto const res_upload = this_t::ip().rt_mreg_ptr->gpu_image_manager()->
                            upload_image_2d( key );
                        so_core::break_if( so_manager::no_success( res_upload ) );
                    }
                }

                // bind image
                {
                    so_manager::so_graphics::itexture_manager::texture_2d_info_t ti ;
                    this_t::ip().rt_mreg_ptr->gpu_texture_manager()->get_texture_2d( key, ti ) ;
                    var_set_ptr->bind_texture( "u_smp", ti.obj_ptr ) ;
                }

            }
#endif
    }

    auto * group_out_ptr = so_scene::so_node::logic_group::create(
        "[so_scene::base_scene_creator::inspect_group] : logic_group" ) ;

    group_out_ptr->add_child( renderable_to_be_created_ptr ) ;

    return group_out_ptr ;
}
