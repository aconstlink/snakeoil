//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "logic_leaf.h"

#include <snakeoil/log/log.h>

using namespace so_imex ;
using namespace so_imex::so_node ;

//*************************************************************************
logic_leaf::logic_leaf( void_t ) 
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
    return so_imex::memory::alloc( this_t(), purp ) ;
}

//*************************************************************************
logic_leaf::this_ptr_t logic_leaf::create( this_rref_t rhv, so_memory::purpose_cref_t purp ) 
{
    return so_imex::memory::alloc( std::move(rhv), purp ) ;
}

//*************************************************************************
void_t logic_leaf::destroy( this_ptr_t ptr ) 
{
    so_imex::memory::dealloc( ptr ) ;
}

//*************************************************************************
void_t logic_leaf::destroy( void_t ) 
{
    this_t::destroy( this ) ;
}

//*************************************************************************
