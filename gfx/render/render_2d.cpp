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