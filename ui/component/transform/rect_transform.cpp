//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "rect_transform.h"

#include <snakeoil/math/vector/vector2.hpp>

using namespace so_ui ;
using namespace so_ui::so_component ;

//****************************************************************************
rect_transform::rect_transform( void_t )
{}

//****************************************************************************
rect_transform:: rect_transform( so_math::vec4f_cref_t r )
{
    _area = r ;
}

//****************************************************************************
rect_transform::rect_transform( this_rref_t rhv )
{
    _area = rhv._area ;
}

//****************************************************************************
rect_transform::~rect_transform( void_t )
{}

//****************************************************************************
rect_transform::this_ptr_t rect_transform::create( so_memory::purpose_cref_t p )
{
    return this_t::create( this_t(), p ) ;
}

//****************************************************************************
rect_transform::this_ptr_t rect_transform::create( this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_ui::memory::alloc( std::move( rhv ), p ) ;
}

//****************************************************************************
void_t rect_transform::destroy( this_ptr_t ptr )
{
    so_ui::memory::dealloc( ptr ) ;
}

//****************************************************************************
void_t rect_transform::destroy( void_t )
{
    this_t::destroy( this ) ;
}

//****************************************************************************
void_t rect_transform::set_rect( so_math::vec4f_cref_t r )
{
    _area = so_math::vec4f_t( r.xy(), r.zw() - r.xy() ) ;
}

//****************************************************************************
so_math::vec4f_t rect_transform::get_rect( void_t ) const
{
    return so_math::vec4f_t( _area.xy(), _area.xy() + _area.zw() ) ;
}

//****************************************************************************
void_t rect_transform::set_dims( so_math::vec4f_cref_t dims )
{
    _area = dims ;
}

//****************************************************************************
so_math::vec4f_t rect_transform::get_dims( void_t ) const
{
    return _area ;
}