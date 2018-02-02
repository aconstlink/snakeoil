//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "gfx_draw_2d.h"

#include <snakeoil/math/vector/vector2.hpp>

using namespace so_ui ;
using namespace so_ui::so_component ;

//****************************************************************************
gfx_draw_2d::gfx_draw_2d( void_t )
{
    _funk = [=] ( so_ui::so_visitor::gfx_draw_2d_ptr_t ) { } ;
}

//****************************************************************************
gfx_draw_2d::gfx_draw_2d( render_funk_t funk )
{
    _funk = funk ;
}

//****************************************************************************
gfx_draw_2d::gfx_draw_2d( this_rref_t rhv )
{
    _funk = std::move( rhv._funk ) ;
}

//****************************************************************************
gfx_draw_2d::~gfx_draw_2d( void_t )
{}

//****************************************************************************
gfx_draw_2d::this_ptr_t gfx_draw_2d::create( so_memory::purpose_cref_t p )
{
    return this_t::create( this_t(), p ) ;
}

//****************************************************************************
gfx_draw_2d::this_ptr_t gfx_draw_2d::create( this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_ui::memory::alloc( std::move( rhv ), p ) ;
}

//****************************************************************************
void_t gfx_draw_2d::destroy( this_ptr_t ptr )
{
    so_ui::memory::dealloc( ptr ) ;
}

//****************************************************************************
void_t gfx_draw_2d::destroy( void_t )
{
    this_t::destroy( this ) ;
}

//****************************************************************************
void_t gfx_draw_2d::render( so_ui::so_visitor::gfx_draw_2d_ptr_t ptr )
{
    _funk( ptr ) ;
}