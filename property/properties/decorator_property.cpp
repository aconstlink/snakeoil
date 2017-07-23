//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "decorator_property.h"
#include "../ivisitor.h"

using namespace so_property ;

//*************************************************************************************
decorator_property::decorator_property( so_std::utf8_cref_t name, so_property::iproperty_ptr_t pptr) :
    _name( name ), _deco_ptr(pptr)
{
}

//*************************************************************************************
decorator_property::decorator_property( this_rref_t rhv )
{
    so_move( _name, rhv ) ;
    so_move_member_ptr( _deco_ptr, rhv ) ;
}

//*************************************************************************************
decorator_property::~decorator_property( void_t )
{
    if( so_core::is_not_nullptr( _deco_ptr ) )
        _deco_ptr->destroy() ;
}

//*************************************************************************************
so_std::utf8_cref_t decorator_property::get_name( void_t ) const
{
    return _name ;
}

//*************************************************************************************
void_t decorator_property::apply( so_property::ivisitor_ptr_t ptr )
{
    if( so_core::is_nullptr( ptr ) )
        return ;

    ptr->visit( this ) ;
    this_t::traverse_decorated( ptr ) ;
}

//*************************************************************************************
void_t decorator_property::destroy( void_t )
{

}

//*************************************************************************************
void_t decorator_property::traverse_decorated( so_property::ivisitor_ptr_t vptr )
{
    if( so_core::is_nullptr( _deco_ptr ) || so_core::is_nullptr(vptr) )
        return ;

    return _deco_ptr->apply( vptr ) ;
}