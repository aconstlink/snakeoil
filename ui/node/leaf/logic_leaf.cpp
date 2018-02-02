//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "logic_leaf.h"
#include "../../visitor/ivisitor.h"

#include <snakeoil/log/global.h>

using namespace so_ui ;
using namespace so_ui::so_node ;

//*************************************************************************
logic_leaf::logic_leaf( void_t ) 
{}

//*************************************************************************
logic_leaf::logic_leaf( iparent_ptr_t pptr ) : base_t(pptr)
{}

//*************************************************************************
logic_leaf::logic_leaf( this_rref_t rhv ) : base_t( std::move(rhv) )
{}

//*************************************************************************
logic_leaf::~logic_leaf( void_t ) 
{}

//*************************************************************************
logic_leaf::this_ptr_t logic_leaf::create( so_memory::purpose_cref_t purp ) 
{
    return so_ui::memory::alloc( this_t(), purp ) ;
}

//*************************************************************************
logic_leaf::this_ptr_t logic_leaf::create( this_rref_t rhv, so_memory::purpose_cref_t purp ) 
{
    return so_ui::memory::alloc( std::move(rhv), purp ) ;
}

//*************************************************************************
void_t logic_leaf::destroy( this_ptr_t ptr ) 
{
    so_ui::memory::dealloc( ptr ) ;
}

//*************************************************************************
so_ui::result logic_leaf::apply( so_ui::so_visitor::ivisitor_ptr_t vptr ) 
{
    auto res = vptr->visit( this ) ;
    
    if( res == so_ui::result::call_base )
    {
        base_t::apply( vptr ) ;
    }

    return vptr->post_visit( this ) ;
}

//*************************************************************************
void_t logic_leaf::destroy( void_t ) 
{
    this_t::destroy( this ) ;
}

//*************************************************************************
