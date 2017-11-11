
#include "line_render_2d.h"

#include "../../node/layout/area_2d.h"
#include "../../node/layout/layer_2d.h"


#include "../../node/render/renderable.h"
#include "../../node/render/render_state.h"

using namespace so_ui ;
using namespace so_ui::so_visitor ;

//****************************************************************************
line_render_2d::line_render_2d( void_t )
{}

//****************************************************************************
line_render_2d::line_render_2d( so_gfx::line_render_2d_ptr_t ptr ) : _rnd_ptr( ptr )
{
    _astack.push( so_math::vec4f_t( 0.0f, 0.0f, 1.0f, 1.0f ) ) ;
    _layer_stack.push( 0 ) ;

    {
        this_t::render_state_data_t rsd ;
        _render_state_stack.push( rsd ) ;
    }
}

//****************************************************************************
line_render_2d::line_render_2d( this_rref_t rhv )
{
    _astack = std::move( rhv._astack ) ;
    _layer_stack = std::move( rhv._layer_stack ) ;
    _render_state_stack = std::move( rhv._render_state_stack ) ;
}

//****************************************************************************
line_render_2d::~line_render_2d( void_t )
{}

//****************************************************************************
line_render_2d::this_ptr_t line_render_2d::create( so_memory::purpose_cref_t p )
{
    return so_ui::memory::alloc( this_t(), p ) ;
}

//****************************************************************************
void_t line_render_2d::destroy( this_ptr_t ptr )
{
    so_ui::memory::dealloc( ptr ) ;
}

//****************************************************************************
void_t line_render_2d::set_render( so_gfx::line_render_2d_ptr_t ptr )
{
    _rnd_ptr = ptr ;
}

//****************************************************************************
so_gfx::line_render_2d_ptr_t line_render_2d::get_render( void_t )
{
    return _rnd_ptr ;
}

//****************************************************************************
so_math::vec4f_cref_t line_render_2d::get_top_area( void_t ) const
{
    return _astack.top() ;
}

//****************************************************************************
size_t line_render_2d::get_top_layer( void_t ) const
{
    return _layer_stack.top() ;
}

//****************************************************************************
so_ui::result line_render_2d::visit( so_ui::so_node::render_state_ptr_t nptr )
{
    this_t::render_state_data rsd ;
    rsd.attr = nptr->get_attributes() ;
    rsd.ss = nptr->get_states() ;

    _render_state_stack.push( rsd ) ;
    return so_ui::ok ;
}


//****************************************************************************
so_ui::result line_render_2d::post_visit( so_ui::so_node::render_state_ptr_t )
{
    return so_ui::ok ;
}

//****************************************************************************
so_ui::result line_render_2d::visit( so_ui::so_node::renderable_ptr_t nptr )
{
    nptr->on_draw( this ) ;
    return so_ui::ok ;
}

//****************************************************************************
so_ui::result line_render_2d::visit( so_ui::so_node::layer_2d_ptr_t lptr )
{
    _layer_stack.push( lptr->get_layer() ) ;
    return so_ui::ok ;
}

//****************************************************************************
so_ui::result line_render_2d::post_visit( so_ui::so_node::layer_2d_ptr_t )
{
    _layer_stack.pop() ;
    return so_ui::ok ;
}

//****************************************************************************
so_ui::result line_render_2d::visit( so_ui::so_node::area_2d_ptr_t aptr )
{
    _astack.push( aptr->get_area() ) ;

    return so_ui::ok ;
}

//****************************************************************************
so_ui::result line_render_2d::post_visit( so_ui::so_node::area_2d_ptr_t )
{
    _astack.pop() ;

    return so_ui::ok ;
}

//****************************************************************************
void_t line_render_2d::destroy( void_t )
{
    this_t::destroy( this ) ;
}