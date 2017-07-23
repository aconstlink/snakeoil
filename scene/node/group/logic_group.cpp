//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "logic_group.h"

using namespace so_scene ;
using namespace so_scene::so_node ;

//*******************************************************************
logic_group::this_ptr_t logic_group::create( so_memory::purpose_cref_t purp ) 
{
    return so_scene::memory::alloc( this_t(), purp ) ;
}

//*******************************************************************
logic_group::this_ptr_t logic_group::create( this_rref_t rhv, so_memory::purpose_cref_t purp ) 
{
    return so_scene::memory::alloc( std::move(rhv), purp ) ;
}

//*******************************************************************
void_t logic_group::destroy( this_ptr_t ptr ) 
{
    so_scene::memory::dealloc( ptr ) ;
}

//*******************************************************************
logic_group::logic_group( void_t ) 
{}

//*******************************************************************
logic_group::logic_group( iparent_ptr_t ptr ) : base_t(ptr)
{}

//*******************************************************************
logic_group::logic_group( this_rref_t rhv ) : base_t(std::move(rhv))
{}

//*******************************************************************
logic_group::~logic_group( void_t ) 
{}

//*******************************************************************
so_scene::result logic_group::add_child( node_ptr_t nptr )
{
    return base_t::add_child( nptr ) ;
}

//*******************************************************************
size_t logic_group::remove_child( node_ptr_t nptr )
{
    return base_t::remove_child( nptr ) ;
}

//*******************************************************************
so_scene::result logic_group::apply( so_scene::so_visitor::ivisitor_ptr_t ptr )
{
    return base_t::apply( ptr ) ;
}

//*******************************************************************
void_t logic_group::destroy( void_t ) 
{
    this_t::destroy( this ) ;
}
