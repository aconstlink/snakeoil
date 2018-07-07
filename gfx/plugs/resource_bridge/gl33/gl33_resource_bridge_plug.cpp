//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "gl33_resource_bridge_plug.h"
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
gl33_resource_bridge_plug::gl33_resource_bridge_plug( resource_bridge_t::shared_data_ptr_t sd_ptr,
    so_gpu::so_gl::igl_33_api_ptr_t aptr ) : this_base_t( aptr )
{
    _sd_ptr = sd_ptr ;
}

//*************************************************************************************
gl33_resource_bridge_plug::gl33_resource_bridge_plug( this_rref_t rhv ) :
    this_base_t( std::move( rhv ) )
{
    so_move_member_ptr( _sd_ptr, rhv ) ;
}

//*************************************************************************************
gl33_resource_bridge_plug::~gl33_resource_bridge_plug( void_t )
{
}

//*************************************************************************************
gl33_resource_bridge_plug::this_ptr_t gl33_resource_bridge_plug::create( 
    this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_gfx::memory::alloc( std::move( rhv ), p ) ;
}

//*************************************************************************************
void_t gl33_resource_bridge_plug::destroy( this_ptr_t ptr )
{
    so_gfx::memory::dealloc( ptr ) ;
}

//*************************************************************************************
so_gpx::plug_result gl33_resource_bridge_plug::on_load( load_info_cref_t li )
{
    return so_gpx::plug_result::ok ;
}

//*************************************************************************************
so_gpx::plug_result gl33_resource_bridge_plug::on_unload( void_t )
{
    return so_gpx::plug_result::ok ;
}

//*************************************************************************************
so_gpx::plug_result gl33_resource_bridge_plug::on_initialize( init_info_cref_t ii )
{
    return so_gpx::plug_result::ok ;
}

//*************************************************************************************
so_gpx::plug_result gl33_resource_bridge_plug::on_release( void_t )
{
    return so_gpx::plug_result::ok ;
}

//*************************************************************************************
so_gpx::plug_result gl33_resource_bridge_plug::on_execute( execute_info_cref_t )
{
    // do work
    return so_gpx::plug_result::ok ;
}

//*************************************************************************************
so_gpx::plug_result gl33_resource_bridge_plug::on_update( update_info_cref_t )
{
    return so_gpx::plug_result::ok ;
}

//*************************************************************************************
void_t gl33_resource_bridge_plug::destroy( void_t )
{
    this_t::destroy( this ) ;
}