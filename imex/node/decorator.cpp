//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "decorator.h"

using namespace so_imex ;
using namespace so_imex::so_node ;

//*************************************************************************************
decorator::decorator( void_t ) 
{}

//*************************************************************************************
decorator::decorator( this_rref_t rhv ) : node( std::move(rhv) )
{
    so_move_member_ptr( _decorated_ptr, rhv ) ;

    if( _decorated_ptr != nullptr ) 
        _decorated_ptr->set_parent( this ) ;
}

//*************************************************************************************
decorator::decorator( node_ptr_t nptr ) 
{
    _decorated_ptr = nptr ;
}

//*************************************************************************************
decorator::~decorator( void_t ) 
{
    if( _decorated_ptr != nullptr )
        _decorated_ptr->destroy() ;
}

//*************************************************************************************
node_ptr_t decorator::get_decorated( void_t ) 
{
    return _decorated_ptr ;
}

//*************************************************************************************
node_cptr_t decorator::get_decorated( void_t ) const 
{
    return _decorated_ptr ;
}

//*************************************************************************************
bool_t decorator::has_decorated( void_t ) const 
{
    return so_core::is_not_nullptr( _decorated_ptr ) ;
}

//*************************************************************************************
void_t decorator::set_decorated( node_ptr_t nptr ) 
{
    if( _decorated_ptr != nullptr )
        _decorated_ptr->set_parent( nullptr ) ;

    _decorated_ptr = nptr ;
   
    if( _decorated_ptr != nullptr )
        _decorated_ptr->set_parent( this ) ;
}

