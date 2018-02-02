//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "gfx_draw_2d.h"

#include <snakeoil/math/matrix/matrix4.hpp>
#include <snakeoil/core/macros/move.h>

using namespace so_ui ;
using namespace so_ui::so_visitor ;

//*************************************************************************************
gfx_draw_2d::gfx_draw_2d( so_gfx::render_2d_ptr_t )
{

}

//*************************************************************************************
gfx_draw_2d::this_ptr_t gfx_draw_2d::create( 
    this_rref_t rhv, so_memory::purpose_cref_t p ) 
{
    return so_ui::memory::alloc( std::move(rhv), p ) ;
}

//*************************************************************************************
void_t gfx_draw_2d::destroy( this_ptr_t ptr ) 
{
    so_ui::memory::dealloc( ptr ) ;
}

//*************************************************************************************
gfx_draw_2d::gfx_draw_2d( this_rref_t rhv ) : base_t(std::move(rhv) )
{}

//*************************************************************************************
gfx_draw_2d::~gfx_draw_2d( void_t ) 
{}

//*************************************************************************************
void_t gfx_draw_2d::destroy( void_t ) 
{
    this_t::destroy( this ) ;
}
