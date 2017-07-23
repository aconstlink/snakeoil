//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "arg_node.h"

using namespace so_shade ;
using namespace so_shade::so_tree ;

//*************************************************************************************
arg_node::arg_node( void_t ) 
{
}

//*************************************************************************************
arg_node::arg_node( this_rref_t )
{
}

//*************************************************************************************
arg_node::arg_node( this_cref_t )
{
}

//*************************************************************************************
arg_node::~arg_node( void_t )
{
}

//*************************************************************************************
arg_node::this_ptr_t arg_node::create( so_memory::purpose_cref_t p )
{
    return so_shade::memory::alloc( this_t(), p ) ;
}

//*************************************************************************************
arg_node::this_ptr_t arg_node::create( this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_shade::memory::alloc( std::move(rhv), p ) ;
}

//*************************************************************************************
void_t arg_node::destroy( this_ptr_t ptr )
{
    so_shade::memory::dealloc( ptr ) ;
}

//*************************************************************************************
void_t arg_node::destroy( void_t )
{
    this_t::destroy( this ) ;
}

//*************************************************************************************
node_ptr_t arg_node::clone( void_t ) const 
{
    return nullptr ;
}
