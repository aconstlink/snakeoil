//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "draw_leafs.h"

#include "../../component/layout/column_layout.h"
#include "../../component/layout/row_layout.h"
#include "../../component/transform/rect_transform.h"

#include <snakeoil/gfx/rect/rect_render_2d.h>

#include <array>

using namespace so_ui ;
using namespace so_ui::so_visitor ;

//*************************************************************************
draw_leafs::draw_leafs( so_math::vec4f_cref_t rect, so_gfx::rect_render_2d_ptr_t rnd ) : _rect_rnd_ptr( rnd )
{
    _astack.push( rect ) ;
    _in_row_stack.push( false ) ;
}

//*************************************************************************
draw_leafs::draw_leafs( this_rref_t rhv )
{
    _astack = std::move( rhv._astack ) ;
    _in_row_stack = std::move( rhv._in_row_stack ) ;
    _count = rhv._count ;
    _layer = rhv._layer ;

    so_move_member_ptr( _rect_rnd_ptr, rhv ) ;
}

//*************************************************************************
draw_leafs::this_ptr_t draw_leafs::create( this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_ui::memory::alloc( std::move( rhv ), p ) ;
}

//*************************************************************************
void_t draw_leafs::destroy( this_ptr_t ptr )
{
    return so_ui::memory::dealloc( ptr ) ;
}

//*************************************************************************
draw_leafs::~draw_leafs( void_t ) {}

//*************************************************************************
so_ui::result draw_leafs::visit( so_ui::so_node::node_ptr_t nptr )
{
    {
        so_ui::so_component::column_layout_ptr_t comp ;
        if( nptr->find_component( comp ) )
        {
            _in_row_stack.push( false ) ;
        }
    }

    {
        so_ui::so_component::row_layout_ptr_t comp ;
        if( nptr->find_component( comp ) )
        {
            _in_row_stack.push( true ) ;
        }
    }

    so_ui::so_component::rect_transform_ptr_t rt = nullptr ;
    if( nptr->find_component( rt ) )
    {
        auto const dims = rt->get_dims() ;

        auto const top = _astack.top() ;

        auto const xy = top.xy() + dims.xy() ;
        auto const wh = dims.zw() ;

        _astack.push( so_math::vec4f_t( xy, wh ) ) ;
    }
    return so_ui::ok ;
}

//*************************************************************************
so_ui::result draw_leafs::post_visit( so_ui::so_node::node_ptr_t nptr )
{
    {
        if( nptr->has_component<so_ui::so_component::column_layout_t>() ||
            nptr->has_component<so_ui::so_component::row_layout_t>() )
        {
            _in_row_stack.pop() ;
        }
    }

    so_ui::so_component::rect_transform_ptr_t rt = nullptr ;
    if( nptr->find_component( rt ) )
    {
        _astack.pop() ;
    }
    return so_ui::ok ;
}

//*************************************************************************
so_ui::result draw_leafs::visit( so_ui::so_node::leaf_ptr_t nptr )
{
    so_ui::so_component::rect_transform_ptr_t rt = nullptr ;
    if( so_core::is_not( nptr->find_component( rt ) ) ) 
        return so_ui::ok ;

    static std::array< so_math::vec4f_t, 3 > colors_row = {
        so_math::vec4f_t( 1.0f, 0.0f, 0.0f, 1.0f ),
        so_math::vec4f_t( 0.0f, 1.0f, 0.0f, 1.0f ),
        so_math::vec4f_t( 0.0f, 0.0f, 1.0f, 1.0f )
    } ;

    static std::array< so_math::vec4f_t, 3 > colors_columns = {
        so_math::vec4f_t( 1.0f, 1.0f, 0.0f, 1.0f ),
        so_math::vec4f_t( 0.0f, 1.0f, 1.0f, 1.0f ),
        so_math::vec4f_t( 1.0f, 0.0f, 1.0f, 1.0f )
    } ;

    so_math::vec4f_t const area = _astack.top() ;
    so_math::vec4f_t const local = rt->get_dims() ;
    so_math::vec2f_t const pos = area.xy() + local.xy();
    so_math::vec2f_t const scale = local.zw();

    size_t const in_row = _in_row_stack.top() ;
    _rect_rnd_ptr->draw_rect( _layer, pos, so_math::vec2f_t(), scale, 0.0f,
        in_row == 1 ? colors_row[ _count % 3 ] : colors_columns[ _count % 3 ] ) ;

    _count++ ;
    return so_ui::ok ;
}

//*************************************************************************
so_ui::result draw_leafs::post_visit( so_ui::so_node::leaf_ptr_t )
{
    return so_ui::ok ;
}

//*************************************************************************
void_t draw_leafs::destroy( void_t )
{
    this_t::destroy( this ) ;
}