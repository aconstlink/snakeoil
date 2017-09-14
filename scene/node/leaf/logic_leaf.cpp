//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "logic_leaf.h"
#include "../../visitor/ivisitor.h"

#include <snakeoil/log/global.h>

using namespace so_scene ;
using namespace so_scene::so_node ;

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
    return so_scene::memory::alloc( this_t(), purp ) ;
}

//*************************************************************************
logic_leaf::this_ptr_t logic_leaf::create( this_rref_t rhv, so_memory::purpose_cref_t purp ) 
{
    return so_scene::memory::alloc( std::move(rhv), purp ) ;
}

//*************************************************************************
void_t logic_leaf::destroy( this_ptr_t ptr ) 
{
    so_scene::memory::dealloc( ptr ) ;
}

//*************************************************************************
so_scene::result logic_leaf::apply( so_scene::so_visitor::ivisitor_ptr_t vptr ) 
{
    auto res = vptr->visit( this ) ;
    so_log::global::warning( so_scene::no_success(res), 
        "[so_scene::logic_leaf::apply] : visit" ) ;
    return vptr->post_visit( this ) ;
}

//*************************************************************************
void_t logic_leaf::destroy( void_t ) 
{
    this_t::destroy( this ) ;
}

//*************************************************************************
