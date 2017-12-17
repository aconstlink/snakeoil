//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "layout_updater.h"

#include "../../node/layout/area_2d.h"
#include "../../node/layout/row_layout.h"
#include "../../node/layout/column_layout.h"

using namespace so_ui ;
using namespace so_ui::so_visitor ;

//****************************************************************************
layout_updater::layout_updater( void_t )
{
    _astack.push( so_math::vec4f_t(0.0f, 0.0f, 1.0f, 1.0f ) ) ;
}

//****************************************************************************
layout_updater::layout_updater( this_cref_t )
{

}

//****************************************************************************
layout_updater::layout_updater( this_rref_t )
{

}

//****************************************************************************
layout_updater::~layout_updater( void_t )
{

}

//****************************************************************************
layout_updater::this_ptr_t layout_updater::create( so_memory::purpose_cref_t p )
{
    return so_ui::memory::alloc( this_t(), p ) ;
}

//****************************************************************************
void_t layout_updater::destroy( this_ptr_t ptr )
{
    so_ui::memory::dealloc( ptr ) ;
}

//****************************************************************************
so_ui::result layout_updater::visit( so_ui::so_node::row_layout_ptr_t nptr )
{
    float_t const max_weight = nptr->get_max_weight() ;
    size_t const num_children = nptr->get_num_children() ;

    float_t y_offset = 0.0f ;

    for( size_t i = 0; i < num_children; ++i )
    {
        float_t const weight = nptr->get_weight(i) ;
        float_t const scale = weight / max_weight ;

        auto * aptr = reinterpret_cast< so_ui::so_node::area_2d_ptr_t>( nptr->get_child( i ) ) ;
        
        so_math::vec4f_t const dims = _astack.top() ;

        float_t const some_height = dims.w()*scale ;
        y_offset += some_height ;
        aptr->set_area( so_math::vec4f_t( dims.x(), (dims.y()+dims.w()) - y_offset, dims.z(), some_height ) ) ;
    }

    return so_ui::ok ;
}

//****************************************************************************
so_ui::result layout_updater::post_visit( so_ui::so_node::row_layout_ptr_t )
{
    return so_ui::ok ;
}

//****************************************************************************
so_ui::result layout_updater::visit( so_ui::so_node::column_layout_ptr_t nptr )
{
    float_t const max_weight = nptr->get_max_weight() ;
    size_t const num_children = nptr->get_num_children() ;

    float_t x_offset = 0.0f ;

    for( size_t i = 0; i < num_children; ++i )
    {
        float_t const weight = nptr->get_weight( i ) ;
        float_t const scale = weight / max_weight ;

        auto * aptr = reinterpret_cast< so_ui::so_node::area_2d_ptr_t >( nptr->get_child( i ) ) ;

        so_math::vec4f_t const dims = _astack.top() ;

        float_t const some_width = dims.z()*scale ;
        
        aptr->set_area( so_math::vec4f_t( dims.x()+x_offset, dims.y(), some_width, dims.w() ) ) ;
        x_offset += some_width ;
    }

    return so_ui::ok ;
}

//****************************************************************************
so_ui::result layout_updater::post_visit( so_ui::so_node::column_layout_ptr_t )
{
    return so_ui::ok ;
}

//****************************************************************************
so_ui::result layout_updater::visit( so_ui::so_node::area_2d_ptr_t aptr )
{
    _astack.push( aptr->get_area() ) ;

    return so_ui::ok ;
}

//****************************************************************************
so_ui::result layout_updater::post_visit( so_ui::so_node::area_2d_ptr_t )
{
    _astack.pop() ;

    return so_ui::ok ;
}

//****************************************************************************
void_t layout_updater::destroy( void_t )
{
    this_t::destroy( this ) ;
}