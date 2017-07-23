//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "logic_decorator.h"

using namespace so_imex ;
using namespace so_imex::so_node ;

//*************************************************************************************
logic_decorator::logic_decorator( void_t ) 
{}

//*************************************************************************************
logic_decorator::logic_decorator( this_rref_t rhv ) : base_t( std::move(rhv) )
{}

//*************************************************************************************
logic_decorator::~logic_decorator( void_t ) 
{}

//*************************************************************************************
logic_decorator::this_ptr_t logic_decorator::create( so_memory::purpose_cref_t p ) 
{
    return so_imex::memory::alloc( this_t(), p ) ;
}

//*************************************************************************************
logic_decorator::this_ptr_t logic_decorator::create( this_rref_t rhv, so_memory::purpose_cref_t p ) 
{
    return so_imex::memory::alloc( std::move(rhv), p ) ;
}

//*************************************************************************************
void_t logic_decorator::destroy( this_ptr_t ptr ) 
{
    so_imex::memory::dealloc( ptr ) ;
}

//*************************************************************************************
void_t logic_decorator::set_decorated( node_ptr_t nptr ) 
{
    base_t::set_decorated( nptr ) ;
}

//*************************************************************************************
void_t logic_decorator::destroy( void_t ) 
{
    this_t::destroy( this ) ;
}

//*************************************************************************************
