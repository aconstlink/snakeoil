//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "render_2d.h"

#include "../image/image_render_2d.h"
#include "../circle/circle_render_2d.h"
#include "../rect/rect_render_2d.h"
#include "../line/line_render_2d.h"

using namespace so_gfx ;

//************************************************************************************
render_2d::render_2d( so_gpx::render_system_ptr_t rs )
{
    _rs_ptr = rs ;

    // create
    _ir_ptr = so_gfx::image_render_2d_t::create( so_gfx::image_render_2d_t(rs), 
        "[render_2d::render_2d] : image_render_2d" ) ;

    _rr_ptr = so_gfx::rect_render_2d_t::create( so_gfx::rect_render_2d_t( rs ),
        "[render_2d::render_2d] : rect_render_2d" ) ;

    _lr_ptr = so_gfx::line_render_2d_t::create( so_gfx::line_render_2d_t( rs ),
        "[render_2d::render_2d] : line_render_2d" ) ;

    _cr_ptr = so_gfx::circle_render_2d_t::create( so_gfx::circle_render_2d_t( rs ),
        "[render_2d::render_2d] : circle_render_2d" ) ;
}

//************************************************************************************
render_2d::render_2d( this_rref_t rhv )
{
    so_move_member_ptr( _rs_ptr, rhv ) ;
    so_move_member_ptr( _ir_ptr, rhv ) ;
    so_move_member_ptr( _rr_ptr, rhv ) ;
    so_move_member_ptr( _lr_ptr, rhv ) ;
    so_move_member_ptr( _cr_ptr, rhv ) ;
}

//************************************************************************************
render_2d::~render_2d( void_t )
{
    so_gfx::image_render_2d_t::destroy( _ir_ptr ) ;
    so_gfx::rect_render_2d_t::destroy( _rr_ptr ) ;
    so_gfx::line_render_2d_t::destroy( _lr_ptr ) ;
    so_gfx::circle_render_2d_t::destroy( _cr_ptr ) ;
}

//************************************************************************************
render_2d::this_ptr_t render_2d::create( this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_gpx::memory::alloc( std::move( rhv ), p ) ;
}

//************************************************************************************
void_t render_2d::destroy( this_ptr_t ptr )
{
    so_gpx::memory::dealloc( ptr ) ;
}

//************************************************************************************
so_gfx::rect_render_2d_ptr_t render_2d::get_rect_render_2d( void_t )
{
    return _rr_ptr ;
}

//************************************************************************************
so_gfx::circle_render_2d_ptr_t render_2d::get_circle_render_2d( void_t )
{
    return _cr_ptr ;
}

//************************************************************************************
so_gfx::image_render_2d_ptr_t render_2d::get_image_render_2d( void_t )
{
    return _ir_ptr ;
}

//************************************************************************************
so_gfx::line_render_2d_ptr_t render_2d::get_line_render_2d( void_t )
{
    return _lr_ptr ;
}

//************************************************************************************
void_t render_2d::init( void_t ) 
{
    _rr_ptr->init() ;
    _cr_ptr->init() ;
    _ir_ptr->init() ;
    _lr_ptr->init() ;
}

//************************************************************************************
void_t render_2d::release( void_t ) 
{
    _rr_ptr->release() ;
    _cr_ptr->release() ;
    _ir_ptr->release() ;
    _lr_ptr->release() ;
}

//************************************************************************************
void_t render_2d::set_view_projection( so_math::mat4f_cref_t view, so_math::mat4f_cref_t proj ) 
{
    _rr_ptr->set_view_projection( view, proj ) ;
    _cr_ptr->set_view_projection( view, proj ) ;
    _ir_ptr->set_view_projection( view, proj ) ;
    _lr_ptr->set_view_projection( view, proj ) ;
}

//************************************************************************************
void_t render_2d::prepare_for_rendering( void_t ) 
{
    _rr_ptr->prepare_for_rendering() ;
    _cr_ptr->prepare_for_rendering() ;
    _ir_ptr->prepare_for_rendering() ;
    _lr_ptr->prepare_for_rendering() ;
}

//************************************************************************************
void_t render_2d::render_range( size_t const a, size_t const b ) 
{
    for( size_t i = a; i < b; ++i )
    {
        _rr_ptr->render( i ) ;
        _cr_ptr->render( i ) ;
        _ir_ptr->render( i ) ;
        _lr_ptr->render( i ) ;
    }
}

//************************************************************************************
void_t render_2d::render( size_t const i ) 
{
    _rr_ptr->render( i ) ;
    _cr_ptr->render( i ) ;
    _ir_ptr->render( i ) ;
    _lr_ptr->render( i ) ;
}