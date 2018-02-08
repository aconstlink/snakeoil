//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "gl33_framebuffer_begin_plug.h"

using namespace so_gfx ;
using namespace so_gfx::so_gl33 ;

//*************************************************************************************
gl33_framebuffer_begin_plug::gl33_framebuffer_begin_plug( framebuffer_begin_t::shared_data_ptr_t,
    so_gpu::so_gl::igl_33_api_ptr_t aptr ) : this_base_t( aptr )
{

}

//*************************************************************************************
gl33_framebuffer_begin_plug::gl33_framebuffer_begin_plug( this_rref_t rhv ) : 
    this_base_t( std::move(rhv) )
{

}

//*************************************************************************************
gl33_framebuffer_begin_plug::~gl33_framebuffer_begin_plug( void_t )
{

}

//*************************************************************************************
gl33_framebuffer_begin_plug::this_ptr_t gl33_framebuffer_begin_plug::create( 
    this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_gfx::memory::alloc( std::move( rhv ), p ) ;
}

//*************************************************************************************
void_t gl33_framebuffer_begin_plug::destroy( this_ptr_t ptr )
{
    so_gfx::memory::dealloc( ptr ) ;
}

//*************************************************************************************
so_gpx::plug_result gl33_framebuffer_begin_plug::on_load( load_info_cref_t )
{
    return so_gpx::plug_result::ok ;
}

//*************************************************************************************
so_gpx::plug_result gl33_framebuffer_begin_plug::on_unload( void_t )
{
    return so_gpx::plug_result::ok ;
}

//*************************************************************************************
so_gpx::plug_result gl33_framebuffer_begin_plug::on_initialize( init_info_cref_t )
{
    return so_gpx::plug_result::ok ;
}

//*************************************************************************************
so_gpx::plug_result gl33_framebuffer_begin_plug::on_release( void_t )
{
    return so_gpx::plug_result::ok ;
}

//*************************************************************************************
so_gpx::plug_result gl33_framebuffer_begin_plug::on_execute( execute_info_cref_t )
{
    return so_gpx::plug_result::ok ;
}

//*************************************************************************************
so_gpx::plug_result gl33_framebuffer_begin_plug::on_update( update_info_cref_t )
{
    return so_gpx::plug_result::ok ;
}

//*************************************************************************************
void_t gl33_framebuffer_begin_plug::destroy( void_t )
{
    this_t::destroy( this ) ;
}

//*************************************************************************************