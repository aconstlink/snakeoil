//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "fill_area_2d.h"
#include "../../visitor/ivisitor.h"
#include "../../visitor/render/rect_render_2d.h"

#include <snakeoil/gfx/rect/rect_render_2d.h>

#include <snakeoil/log/global.h>

using namespace so_ui ;
using namespace so_ui::so_node ;

//*************************************************************************
fill_area_2d::fill_area_2d( void_t )
{}

//*************************************************************************
fill_area_2d::fill_area_2d( so_math::vec4f_cref_t c ) : _color(c)
{}

//*************************************************************************
fill_area_2d::fill_area_2d( iparent_ptr_t pptr ) : base_t( pptr )
{}

//*************************************************************************
fill_area_2d::fill_area_2d( this_rref_t rhv ) : base_t( std::move( rhv ) )
{
    _color = rhv._color ;
}

//*************************************************************************
fill_area_2d::~fill_area_2d( void_t )
{}

//*************************************************************************
fill_area_2d::this_ptr_t fill_area_2d::create( so_memory::purpose_cref_t purp )
{
    return so_ui::memory::alloc( this_t(), purp ) ;
}

//*************************************************************************
fill_area_2d::this_ptr_t fill_area_2d::create( this_rref_t rhv, so_memory::purpose_cref_t purp )
{
    return so_ui::memory::alloc( std::move( rhv ), purp ) ;
}

//*************************************************************************
void_t fill_area_2d::destroy( this_ptr_t ptr )
{
    so_ui::memory::dealloc( ptr ) ;
}

//*************************************************************************
so_math::vec4f_cref_t fill_area_2d::get_fill_area_2d( void_t ) const
{
    return _color ;
}

//*************************************************************************
void_t fill_area_2d::set_fill_area_2d( so_math::vec4f_cref_t c )
{
    _color = c ;
}

//*************************************************************************
void_t fill_area_2d::on_draw( so_ui::so_visitor::rect_render_2d_ptr_t rr_2d )
{
    so_math::vec4f_t const area = rr_2d->get_top_area() ;
    so_math::vec2f_t const offset = area.zw() * so_math::vec2f_t( 0.5f ) ;
    so_math::vec2f_t const pos = ( ( area.xy() + offset ) * so_math::vec2f_t( 2.0f ) - so_math::vec2f_t( 1.0f ) )   ;

    so_math::vec2f_t const scale = area.zw()* so_math::vec2f_t( 2.0f ) ;

    size_t const layer = rr_2d->get_top_layer() ;

    rr_2d->get_render()->draw_rect( layer, pos, scale, 0.0f, _color ) ;
}

//*************************************************************************
void_t fill_area_2d::destroy( void_t )
{
    this_t::destroy( this ) ;
}

//*************************************************************************
