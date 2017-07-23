//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "imported_from.h"

#include <snakeoil/log/log.h>

using namespace so_imex ;
using namespace so_imex::so_node ;

//*************************************************************************************
imported_from::imported_from( void_t ) 
{}

//*************************************************************************************
imported_from::imported_from( so_imex::string_cref_t format ) 
{
    _format = format ;
}

//*************************************************************************************
imported_from::imported_from( so_imex::string_cref_t format, 
    so_imex::so_node::node_ptr_t decorated ) 
{
    _format = format ;
    this_t::set_decorated( decorated ) ;
}

//*************************************************************************************
imported_from::imported_from( this_rref_t rhv )  
{
    _format = std::move( rhv._format ) ;
}

//*************************************************************************************
imported_from::~imported_from( void_t )  
{}

//*************************************************************************************
imported_from::this_ptr_t imported_from::create( so_memory::purpose_cref_t purp )  
{
    return so_imex::memory::alloc( this_t(), purp ) ;
}

//*************************************************************************************
imported_from::this_ptr_t imported_from::create( this_rref_t rhv, so_memory::purpose_cref_t purp )  
{
    return so_imex::memory::alloc( std::move(rhv), purp ) ;
}

//*************************************************************************************
void_t imported_from::destroy( this_ptr_t ptr )  
{
    so_imex::memory::dealloc( ptr ) ;
}

//*************************************************************************************
void_t imported_from::set_decorated( so_imex::so_node::node_ptr_t node_ptr ) 
{
    base_t::set_decorated( node_ptr ) ;
}

//*************************************************************************************
void_t imported_from::destroy( void_t )  
{
    this_t::destroy( this ) ;
}

//*************************************************************************************
