//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "logic_group.h"

using namespace so_imex ;
using namespace so_imex::so_node ;

//*************************************************************************************
logic_group::logic_group( void_t ) 
{}

//*************************************************************************************
logic_group::logic_group( this_rref_t rhv ) : base_t( std::move(rhv) )
{}

//*************************************************************************************
logic_group::~logic_group( void_t ) 
{}

//*************************************************************************************
logic_group::this_ptr_t  logic_group::create( so_memory::purpose_cref_t p ) 
{
    return so_imex::memory::alloc( this_t(), p ) ;
}

//*************************************************************************************
logic_group::this_ptr_t logic_group::create( this_rref_t rhv, so_memory::purpose_cref_t p ) 
{
    return so_imex::memory::alloc( std::move(rhv), p ) ;
}

//*************************************************************************************
void_t logic_group::destroy( this_ptr_t ptr ) 
{
    so_imex::memory::dealloc( ptr ) ;
}

//*************************************************************************************
bool_t logic_group::add_child( node_ptr_t nptr ) 
{
    return base_t::add_child( nptr ) ;
}

//*************************************************************************************
void_t logic_group::destroy( void_t ) 
{
    this_t::destroy( this ) ;
}
