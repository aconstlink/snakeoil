//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "render_2d.h"

#include <snakeoil/math/matrix/matrix4.hpp>
#include <snakeoil/core/macros/move.h>

using namespace so_ui ;
using namespace so_ui::so_visitor ;

//*************************************************************************************
render_2d::this_ptr_t render_2d::create( 
    this_rref_t rhv, so_memory::purpose_cref_t p ) 
{
    return so_ui::memory::alloc( std::move(rhv), p ) ;
}

//*************************************************************************************
void_t render_2d::destroy( this_ptr_t ptr ) 
{
    so_ui::memory::dealloc( ptr ) ;
}

//*************************************************************************************
render_2d::render_2d( this_rref_t rhv ) : base_t(std::move(rhv) )
{}

//*************************************************************************************
render_2d::~render_2d( void_t ) 
{}

//*************************************************************************************
void_t render_2d::destroy( void_t ) 
{
    this_t::destroy( this ) ;
}
