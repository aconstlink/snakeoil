//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "gl33_vignette_post_plug.h"
#include "gfx_config.h"

#include <snakeoil/gpu/api/gl/igl_33_api.h>
#include <snakeoil/gpu/framebuffer/framebuffer_2d.h>
#include <snakeoil/gpu/image/color_image_2d.hpp>
#include <snakeoil/gpu/image/depth_image_2d.hpp>
#include <snakeoil/gpu/texture/texture_2d.h>
#include <snakeoil/gpu/texture/depth_texture_2d.h>
#include <snakeoil/gpu/manager/gpu_manager.h>
#include <snakeoil/gpu/image/image_creator.h>
#include <snakeoil/gpu/viewport/viewport_2d.h>
#include <snakeoil/gpu/buffer/vertex_buffer.hpp>
#include <snakeoil/gpu/buffer/index_buffer.hpp>
#include <snakeoil/gpu/shader/vertex_shader.h>
#include <snakeoil/gpu/shader/pixel_shader.h>
#include <snakeoil/gpu/program/program.h>
#include <snakeoil/gpu/program/config.h>
#include <snakeoil/gpu/variable/variable_set.h>

#include <snakeoil/io/global.h>
#include <snakeoil/log/global.h>

using namespace so_gfx ;
using namespace so_gfx::so_gl33 ;

//*************************************************************************************
gl33_vignette_post_plug::gl33_vignette_post_plug( predef_post_t::shared_data_ptr_t sd_ptr,
    so_gpu::so_gl::igl_33_api_ptr_t aptr ) : this_base_t( sd_ptr, aptr )
{
    this_base_t::_vs_file_name = "gl33_vignette_post.vs.glsl" ;
    this_base_t::_ps_file_name = "gl33_vignette_post.ps.glsl" ;
}

//*************************************************************************************
gl33_vignette_post_plug::gl33_vignette_post_plug( this_rref_t rhv ) :
    this_base_t( std::move( rhv ) )
{}

//*************************************************************************************
gl33_vignette_post_plug::~gl33_vignette_post_plug( void_t )
{}

//*************************************************************************************
gl33_vignette_post_plug::this_ptr_t gl33_vignette_post_plug::create(
    this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_gfx::memory::alloc( std::move( rhv ), p ) ;
}

//*************************************************************************************
void_t gl33_vignette_post_plug::destroy( this_ptr_t ptr )
{
    so_gfx::memory::dealloc( ptr ) ;
}

//*************************************************************************************
void_t gl33_vignette_post_plug::destroy( void_t )
{
    this_t::destroy( this ) ;
}

//*************************************************************************************
void_t gl33_vignette_post_plug::reconnect_variables( init_info_cref_t ii, so_gpu::variable_set_ptr_t vars ) 
{
    {
        so_gpu::texture_ptr_t color_ptr ;

        so_std::string_t name ;

        {
            so_flow::variable< so_std::string_t > * var ;
            auto const res = _sd_ptr->vs.get_variable( "source_color_tx", var ) ;
            if( so_core::is_not( res ) )
            {
                so_log::global_t::error( "[gl33_vignette_post_plug::reconnect_variables] : "
                    "missing source color texture name in variable set" ) ;
                return ;
            }

            name = var->get_data() ;
        }
        {
            so_gpu::gpu_manager::tx2d_manager_t::handle_t hnd ;
            auto const res = ii.mgr->get_tx2d_mgr()->acquire(
                name, "[gl33_vignette_post_plug::on_initialize]", hnd ) ;
            if( so_core::is_not( res ) )
            {
                so_log::global::error( "[gl33_vignette_post_plug::on_initialize] : "
                    "color texture name incorrect" ) ;
                return ;
            }

            color_ptr = hnd.get_ptr() ;

            so_gpu::iimage_2d_ptr_t ptr = dynamic_cast< so_gpu::iimage_2d_ptr_t >( color_ptr->get_image() );
            if( so_core::is_not_nullptr( ptr ) )
            {
                _bb_dims = so_math::vec2f_t( float_t( ptr->get_width() ), float_t( ptr->get_height() ) ) ;
            }
            else
            {
                so_log::global::error( "[gl33_vignette_post_plug::on_initialize] : "
                    "color texture is not a 2d image" ) ;
            }

        }
        
        vars->bind_texture( "u_smp_color", color_ptr ) ;
        vars->bind_data( "u_bb_dims", &_bb_dims ) ;
        vars->bind_data( "u_fb_dims", &_fb_dims ) ;
    }


    {
        auto const res = _sd_ptr->vs.get_variable( "dest_rect", _var_fb_dims ) ;
        if( res )
        {
            _fb_dims = _var_fb_dims->get_data().zw();
        }
        else
        {
            _fb_dims = _bb_dims ;
        }
    }
}

//*************************************************************************************
void_t gl33_vignette_post_plug::before_execute( void_t ) 
{
    _fb_dims = _var_fb_dims->get_data().zw() ;

    this_t::api()->use_framebuffer( nullptr ) ;
    this_t::api()->set_viewport( so_gpu::viewport_2d_t( 0, 0,
        size_t( _fb_dims.x() ), size_t( _fb_dims.y() ) ) ) ;
}

//*************************************************************************************
void_t gl33_vignette_post_plug::after_execute( void_t ) 
{
}