//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "area_2d.h"
#include "../../visitor/ivisitor.h"

#include <snakeoil/log/global.h>

using namespace so_ui ;
using namespace so_ui::so_node ;

//*************************************************************************
area_2d::area_2d( void_t )
{}

//*************************************************************************
area_2d::area_2d( this_rref_t rhv ) : base_t( std::move( rhv ) )
{}

//*************************************************************************
area_2d::area_2d( so_ui::so_node::iparent_ptr_t pptr ) : base_t( pptr )
{}

//*************************************************************************
area_2d::~area_2d( void_t )
{}

//*************************************************************************
area_2d::this_ptr_t area_2d::create( so_memory::purpose_cref_t purp )
{
    return so_ui::memory::alloc( this_t(), purp ) ;
}

//*************************************************************************
area_2d::this_ptr_t area_2d::create( this_rref_t rhv, so_memory::purpose_cref_t purp )
{
    return so_ui::memory::alloc( std::move( rhv ), purp ) ;
}

//*************************************************************************
void_t area_2d::destroy( this_ptr_t ptr )
{
    so_ui::memory::dealloc( ptr ) ;
}

//*************************************************************************
void_t area_2d::set_decorated( so_ui::so_node::node_ptr_t nptr )
{
    base_t::set_decorated( nptr ) ;
}

//*************************************************************************
so_ui::result area_2d::apply( so_ui::so_visitor::ivisitor_ptr_t vptr )
{
    auto res = vptr->visit( this ) ;

    base_t::traverse_decorated( vptr, res ) ;

    return vptr->post_visit( this ) ;
}

//*************************************************************************
void_t area_2d::destroy( void_t )
{
    this_t::destroy( this ) ;
}

//*************************************************************************
void_t area_2d::set_area( so_math::vec4f_cref_t area_in )
{
    _data = area_in ;
} 

//*************************************************************************
so_math::vec4f_cref_t area_2d::get_area( void_t ) const
{
    return _data ;
}