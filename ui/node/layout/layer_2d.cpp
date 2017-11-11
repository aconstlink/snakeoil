//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "layer_2d.h"
#include "../../visitor/ivisitor.h"

#include <snakeoil/log/global.h>

using namespace so_ui ;
using namespace so_ui::so_node ;

//*************************************************************************
layer_2d::layer_2d( void_t )
{}

//*************************************************************************
layer_2d::layer_2d( this_rref_t rhv ) : base_t( std::move( rhv ) )
{}

//*************************************************************************
layer_2d::layer_2d( so_ui::so_node::iparent_ptr_t pptr ) : base_t( pptr )
{}

//*************************************************************************
layer_2d::~layer_2d( void_t )
{}

//*************************************************************************
layer_2d::this_ptr_t layer_2d::create( so_memory::purpose_cref_t purp )
{
    return so_ui::memory::alloc( this_t(), purp ) ;
}

//*************************************************************************
layer_2d::this_ptr_t layer_2d::create( this_rref_t rhv, so_memory::purpose_cref_t purp )
{
    return so_ui::memory::alloc( std::move( rhv ), purp ) ;
}

//*************************************************************************
void_t layer_2d::destroy( this_ptr_t ptr )
{
    so_ui::memory::dealloc( ptr ) ;
}

//*************************************************************************
void_t layer_2d::set_decorated( so_ui::so_node::node_ptr_t nptr )
{
    base_t::set_decorated( nptr ) ;
}

//*************************************************************************
so_ui::result layer_2d::apply( so_ui::so_visitor::ivisitor_ptr_t vptr )
{
    auto res = vptr->visit( this ) ;

    base_t::traverse_decorated( vptr, res ) ;

    return vptr->post_visit( this ) ;
}

//*************************************************************************
void_t layer_2d::destroy( void_t )
{
    this_t::destroy( this ) ;
}

//*************************************************************************
