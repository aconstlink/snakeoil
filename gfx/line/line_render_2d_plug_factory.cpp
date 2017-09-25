//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "line_render_2d_plug_factory.h"

#include "gl33/gl33_line_render_2d_plug.h"

#include <snakeoil/gpu/api/gl/igl_33_api.h>
#include <snakeoil/log/global.h>

using namespace so_gfx ;

//************************************************************************************
line_render_2d_plug_factory::line_render_2d_plug_factory( line_render_2d_shared_data_ptr_t sd_ptr )
{
    _sd = sd_ptr ;
}

//************************************************************************************
line_render_2d_plug_factory::line_render_2d_plug_factory( this_rref_t rhv )
{
    so_move_member_ptr( _sd, rhv ) ;
}

//************************************************************************************
line_render_2d_plug_factory::~line_render_2d_plug_factory( void_t )
{
}

//************************************************************************************
line_render_2d_plug_factory::this_ptr_t line_render_2d_plug_factory::create(
    this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_gfx::memory::alloc( std::move( rhv ), p ) ;
}

//************************************************************************************
void_t line_render_2d_plug_factory::destroy( this_ptr_t ptr )
{
    so_gfx::memory::dealloc( ptr ) ;
}

//************************************************************************************
so_gpx::iplug_ptr_t line_render_2d_plug_factory::create_plug( so_gpu::api_type const t,
    so_gpu::iapi_ptr_t aptr )
{
    switch( t )
    {
    case so_gpu::api_type::gl_33:

        return so_gfx::so_gl33::gl33_line_render_2d_plug_t::create(
            so_gfx::so_gl33::gl33_line_render_2d_plug_t( _sd,
                static_cast< so_gpu::so_gl::igl_33_api_ptr_t >( aptr ) ),
            "[line_render_2d_plug_factory::create_plug]" ) ;

    default:

        so_log::global::error( "[line_render_2d_plug_factory::create_plug] : "
            "can not create plug for api: " + so_gpu::to_string( t ) ) ;
    }

    return nullptr ;
}

//************************************************************************************
void_t line_render_2d_plug_factory::destroy_plug( so_gpx::iplug_ptr_t ptr )
{
    ptr->destroy() ;
}

//************************************************************************************
void_t line_render_2d_plug_factory::destroy( void_t )
{
    this_t::destroy( this ) ;
}