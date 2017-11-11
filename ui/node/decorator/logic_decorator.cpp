//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "logic_decorator.h"
#include "../../visitor/ivisitor.h"

#include <snakeoil/log/global.h>

using namespace so_ui ;
using namespace so_ui::so_node ;

//*************************************************************************
logic_decorator::logic_decorator( void_t ) 
{}

//*************************************************************************
logic_decorator::logic_decorator( this_rref_t rhv ) : base_t( std::move(rhv) )
{}

//*************************************************************************
logic_decorator::logic_decorator( so_ui::so_node::iparent_ptr_t pptr ) : base_t(pptr)
{}

//*************************************************************************
logic_decorator::~logic_decorator( void_t )
{}

//*************************************************************************
logic_decorator::this_ptr_t logic_decorator::create( so_memory::purpose_cref_t purp )
{
    return so_ui::memory::alloc( this_t(), purp ) ;
}

//*************************************************************************
logic_decorator::this_ptr_t logic_decorator::create( this_rref_t rhv, so_memory::purpose_cref_t purp ) 
{
    return so_ui::memory::alloc( std::move(rhv), purp ) ;
}

//*************************************************************************
void_t logic_decorator::destroy( this_ptr_t ptr ) 
{
    so_ui::memory::dealloc( ptr ) ;
}

//*************************************************************************
void_t logic_decorator::set_decorated( so_ui::so_node::node_ptr_t nptr ) 
{
    base_t::set_decorated( nptr ) ;
}

//*************************************************************************
so_ui::result logic_decorator::apply( so_ui::so_visitor::ivisitor_ptr_t vptr ) 
{
    auto res = vptr->visit( this ) ;

    base_t::traverse_decorated( vptr, res ) ;

    return vptr->post_visit( this ) ;
}

//*************************************************************************
void_t logic_decorator::destroy( void_t )
{
    this_t::destroy( this ) ;
}

//*************************************************************************
