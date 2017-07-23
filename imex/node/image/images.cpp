//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "images.h"

using namespace so_imex ;
using namespace so_imex::so_node ;

//*************************************************************************************
images::images( void_t ) 
{
}

//*************************************************************************************
images::images( this_rref_t rhv ) : base_t( std::move(rhv) )
{
}

//*************************************************************************************
images::~images( void_t )
{
}

//*************************************************************************************
images::this_ptr_t images::create( so_memory::purpose_cref_t p )
{
    return so_imex::memory::alloc( this_t(), p ) ;
}

//*************************************************************************************
images::this_ptr_t images::create( this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_imex::memory::alloc( std::move(rhv), p ) ;
}

//*************************************************************************************
void_t images::destroy( this_ptr_t ptr )
{
    so_imex::memory::dealloc( ptr ) ;
}

//*************************************************************************************
void_t images::destroy( void_t )
{
    this_t::destroy( this ) ;
}

//*************************************************************************************
