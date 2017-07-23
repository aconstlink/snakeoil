//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "generic_funk_node.h"

using namespace so_shade ;
using namespace so_shade::so_tree ;

//**********************************************************************************************
generic_funk_node::generic_funk_node( so_std::string_cref_t name ) : base_t(name) 
{}

//**********************************************************************************************
generic_funk_node::generic_funk_node( this_rref_t rhv ) : base_t( std::move(rhv) )
{}

//**********************************************************************************************
generic_funk_node::~generic_funk_node( void_t ) 
{}

//**********************************************************************************************
generic_funk_node::this_ptr_t generic_funk_node::create( this_rref_t rhv, so_memory::purpose_cref_t p ) 
{
    return so_shade::memory::alloc( std::move(rhv), p ) ;
}

//**********************************************************************************************
void_t generic_funk_node::destroy( this_ptr_t ptr ) 
{
    so_shade::memory::dealloc( ptr ) ;
}

//**********************************************************************************************
node_ptr_t generic_funk_node::clone( void_t ) const 
{
    return nullptr ;
}

//**********************************************************************************************
void_t generic_funk_node::destroy( void_t ) 
{
    this_t::destroy( this ) ;
}
