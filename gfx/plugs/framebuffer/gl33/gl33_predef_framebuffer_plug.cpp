//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "gl33_predef_framebuffer_plug.h"

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
gl33_predef_framebuffer_plug::gl33_predef_framebuffer_plug( predef_framebuffer_t::shared_data_ptr_t sd,
    so_gpu::so_gl::igl_33_api_ptr_t aptr ) : this_base_t( aptr )
{
    _sd = sd ;

    _fb_ptr = so_gpu::framebuffer_2d_t::create( "[gl_33_init_plug] : framebuffer" ) ;
}

//*************************************************************************************
gl33_predef_framebuffer_plug::gl33_predef_framebuffer_plug( this_rref_t rhv ) : 
    this_base_t( std::move(rhv) )
{
    _pfb_type = rhv._pfb_type ;
    so_move_member_ptr( _sd, rhv ) ;
    so_move_member_ptr( _fb_ptr, rhv ) ;
    so_move_member_ptr( _image_ptr, rhv ) ;
    so_move_member_ptr( _depth_ptr, rhv ) ;
    so_move_member_ptr( _tx_ptr, rhv ) ;
    so_move_member_ptr( _dtx_ptr, rhv ) ;
    so_move_member_ptr( _vp_window, rhv ) ;
    so_move_member_ptr( _vp_back, rhv ) ;

    _color = rhv._color ;
}

//*************************************************************************************
gl33_predef_framebuffer_plug::~gl33_predef_framebuffer_plug( void_t )
{
    if( so_core::is_not_nullptr( _fb_ptr ) )
        _fb_ptr->destroy() ;

    if( so_core::is_not_nullptr( _image_ptr ) )
        _image_ptr->destroy() ;

    if( so_core::is_not_nullptr( _depth_ptr ) )
        _depth_ptr->destroy() ;

    if( so_core::is_not_nullptr( _tx_ptr ) )
        _tx_ptr->destroy() ;

    so_gpu::depth_texture_2d_t::destroy( _dtx_ptr ) ;
}

//*************************************************************************************
gl33_predef_framebuffer_plug::this_ptr_t gl33_predef_framebuffer_plug::create( 
    this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_gfx::memory::alloc( std::move( rhv ), p ) ;
}

//*************************************************************************************
void_t gl33_predef_framebuffer_plug::destroy( this_ptr_t ptr )
{
    so_gfx::memory::dealloc( ptr ) ;
}

//*************************************************************************************
so_gpx::plug_result gl33_predef_framebuffer_plug::on_load( load_info_cref_t )
{
    return so_gpx::plug_result::ok ;
}

//*************************************************************************************
so_gpx::plug_result gl33_predef_framebuffer_plug::on_unload( void_t )
{
    return so_gpx::plug_result::ok ;
}

//*************************************************************************************
so_gpx::plug_result gl33_predef_framebuffer_plug::on_initialize( init_info_cref_t ii )
{
    _viewport = _sd->vp ;

    bool_t const with_color = _sd->type == so_gfx::predef_framebuffer_type::color888_alpha8_depth32 ||
        _sd->type == so_gfx::predef_framebuffer_type::color888_alpha8 ;

    bool_t const with_depth = _sd->type == so_gfx::predef_framebuffer_type::color888_alpha8_depth32 ||
        _sd->type == so_gfx::predef_framebuffer_type::color888_alpha8_depth32 ;    

    size_t const width = _viewport.get_width<size_t>() ;
    size_t const height = _viewport.get_height<size_t>() ;

    // color image
    if( with_color )
    {
        color_image_t img( width, height ) ;

        for( size_t i = 0; i < width * height; ++i )
        {
            img.set_pixel( i, so_math::vec4uc_t( 255, 255, 0, 255 ) )  ;
        }

        _image_ptr = color_image_t::create( std::move( img ),
            "[gl33_predef_framebuffer_plug::on_initialize] : framebuffer image" ) ;

        _tx_ptr = so_gpu::texture_2d_t::create( so_gpu::texture_2d_t( _image_ptr ),
            "[gl33_predef_framebuffer_plug::on_initialize] : framebuffer image texture" ) ;

        _fb_ptr->add_color_target( _tx_ptr ) ;
    }

    // depth image
    if( with_depth )
    {
        depth_image_t img( width, height ) ;
        for( size_t i = 0; i < width * height; ++i )
        {
            img.set_pixel( i, 0.0f )  ;
        }

        _depth_ptr = depth_image_t::create( std::move( img ),
            "[gl33_predef_framebuffer_plug::on_initialize] : framebuffer depth image" ) ;

        _dtx_ptr = so_gpu::depth_texture_2d_t::create( so_gpu::depth_texture_2d_t( _depth_ptr ),
            "[gl33_predef_framebuffer_plug::on_initialize] : framebuffer depth texture" ) ;

        _fb_ptr->set_depth_target( _dtx_ptr ) ;
    }

    {
        auto const res = this_t::api()->create_framebuffer( _fb_ptr ) ;
        if( so_gpu::no_success( res ) )
        {
            so_log::global_t::error( "[gl33_predef_framebuffer_plug::on_initialize] : "
                "framebuffer creation" ) ;
            return so_gpx::plug_result::failed ;
        }
    }

    if( with_depth )
    {
        auto const res = this_t::api()->create_texture( _dtx_ptr ) ;
        if( so_gpu::no_success( res ) )
        {
            so_log::global_t::error( "[gl33_predef_framebuffer_plug::on_initialize] : "
                "depth texture creation" ) ;
            return so_gpx::plug_result::failed ;
        }
    }

    if( with_color )
    {
        auto const res = this_t::api()->create_texture( _tx_ptr ) ;
        if( so_gpu::no_success( res ) )
        {
            so_log::global_t::error( "[gl33_predef_framebuffer_plug::on_initialize] : "
                "color texture creation" ) ;
            return so_gpx::plug_result::failed ;
        }
    }

    if( with_color )
    {
        auto const res = this_t::api()->create_image( _image_ptr ) ;
        if( so_gpu::no_success( res ) )
        {
            so_log::global_t::error( "[gl33_predef_framebuffer_plug::on_initialize] : "
                "color image creation" ) ;
            return so_gpx::plug_result::failed ;
        }
    }

    if( with_depth )
    {
        auto const res = this_t::api()->create_image( _depth_ptr ) ;
        if( so_gpu::no_success( res ) )
        {
            so_log::global_t::error( "[gl33_predef_framebuffer_plug::on_initialize] : "
                "depth image creation" ) ;
            return so_gpx::plug_result::failed ;
        }
    }

    if( with_color )
    {
        auto const res = this_t::api()->alloc_image_memory( _image_ptr,
            so_gpu::image_alloc_info_t( true ) ) ;

        if( so_gpu::no_success( res ) )
        {
            so_log::global_t::error( "[gl33_predef_framebuffer_plug::on_initialize] : "
                "image allocation failed" ) ;
            return so_gpx::plug_result::failed ;
        }
    }

    if( with_depth )
    {
        auto const res = this_t::api()->alloc_image_memory( _depth_ptr,
            so_gpu::image_alloc_info_t( true ) ) ;

        if( so_gpu::no_success( res ) )
        {
            so_log::global_t::error( "[gl33_predef_framebuffer_plug::on_initialize] : "
                "depth image allocation failed" ) ;
            return so_gpx::plug_result::failed ;
        }
    }
    
    {
        auto const res = this_t::api()->validate( _fb_ptr ) ;
        if( so_gpu::no_success(res) )
        {
            so_log::global_t::error( "[gl33_predef_framebuffer_plug::on_initialize] : "
                "Framebuffer validation failed" ) ;
            return so_gpx::plug_result::failed ;
        }
    }

    // insert framebuffers and fb textures
    {
        ii.mgr->get_fb2d_mgr()->insert( _sd->fb_name, _fb_ptr ) ;
        if( with_color )
            ii.mgr->get_tx2d_mgr()->insert( _sd->fb_name + "_color_0", _tx_ptr ) ;
        if( with_depth )
            ii.mgr->get_tx2d_mgr()->insert( _sd->fb_name + "_depth_0", _dtx_ptr ) ;
    }

    return so_gpx::plug_result::ok ;
}

//*************************************************************************************
so_gpx::plug_result gl33_predef_framebuffer_plug::on_release( void_t )
{
    this_t::api()->release_framebuffer( _fb_ptr ) ;
    this_t::api()->release_texture( _dtx_ptr ) ;
    this_t::api()->release_texture( _tx_ptr ) ;
    this_t::api()->release_image( _image_ptr ) ;
    this_t::api()->release_image( _depth_ptr ) ;

    return so_gpx::plug_result::ok ;
}

//*************************************************************************************
so_gpx::plug_result gl33_predef_framebuffer_plug::on_execute( execute_info_cref_t ei )
{
    // clear
    if( ei.rnd_id == 0 )
    {
        this_t::api()->set_clear_color( _color ) ;
        // @todo depth values required

        this_t::api()->clear( 
            so_core::is_not_nullptr(_image_ptr), 
            so_core::is_not_nullptr(_depth_ptr), false ) ;
    }
    // begin
    else if( ei.rnd_id == 1 )
    {
        this_t::api()->use_framebuffer( _fb_ptr ) ;
        this_t::api()->set_viewport( _viewport ) ;
    }
    // end
    else if( ei.rnd_id == 2 )
    {
        this_t::api()->use_framebuffer( nullptr ) ;
    }

    return so_gpx::plug_result::ok ;
}

//*************************************************************************************
so_gpx::plug_result gl33_predef_framebuffer_plug::on_update( update_info_cref_t )
{
    _color = _sd->color ;
    _viewport = _sd->vp ;

    return so_gpx::plug_result::ok ;
}

//*************************************************************************************
void_t gl33_predef_framebuffer_plug::destroy( void_t )
{
    this_t::destroy( this ) ;
}

//*************************************************************************************