//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "imgui_plug_factory.h"

#if SNAKEOIL_TARGET_GRAPHICS_OPENGL_33
#include "gl33/gl33_imgui_plug.h"
#endif

#include <snakeoil/gpx/plug/plug.h>
#include <snakeoil/gpu/api/gl/igl_33_api.h>
#include <snakeoil/log/global.h>

using namespace so_imgui ;

//************************************************************************************
imgui_plug_factory::imgui_plug_factory(
    imgui::shared_data_ptr_t sd_ptr )
{
    _sd = sd_ptr ;
}

//************************************************************************************
imgui_plug_factory::imgui_plug_factory( this_rref_t rhv )
{
    so_move_member_ptr( _sd, rhv ) ;
}

//************************************************************************************
imgui_plug_factory::~imgui_plug_factory( void_t )
{
}

//************************************************************************************
imgui_plug_factory::this_ptr_t imgui_plug_factory::create(
    this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_imgui::memory::alloc( std::move( rhv ), p ) ;
}

//************************************************************************************
void_t imgui_plug_factory::destroy( this_ptr_t ptr )
{
    so_imgui::memory::dealloc( ptr ) ;
}

//************************************************************************************
so_gpx::iplug_ptr_t imgui_plug_factory::create_plug( so_gpu::api_type const t,
    so_gpu::iapi_ptr_t aptr )
{
    switch( t )
    {
    case so_gpu::api_type::null:
        break ;

#if SNAKEOIL_TARGET_GRAPHICS_OPENGL_33
    case so_gpu::api_type::gl_33:

        return so_imgui::so_gl33::gl33_imgui_plug_t::create(
            so_imgui::so_gl33::gl33_imgui_plug_t( _sd,
                static_cast< so_gpu::so_gl::igl_33_api_ptr_t >( aptr ) ),
            "[imgui_plug_factory::create_plug]" ) ;
#endif
    default:

        so_log::global::error( "[imgui_plug_factory::create_plug] : "
            "can not create plug for api: " + so_gpu::to_string( t ) ) ;
        break ;
    }

    return so_gpx::so_null::null_plug_t::create(
        so_gpx::so_null::null_plug_t( static_cast< so_gpu::so_null::inull_api_ptr_t >( aptr ) ),
        "[imgui_plug_factory::create_plug]" ) ;
}

//************************************************************************************
void_t imgui_plug_factory::destroy_plug( so_gpx::iplug_ptr_t ptr )
{
    ptr->destroy() ;
}

//************************************************************************************
void_t imgui_plug_factory::destroy( void_t )
{
    this_t::destroy( this ) ;
}