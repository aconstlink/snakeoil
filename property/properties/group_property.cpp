//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "group_property.h"

using namespace so_property ;

//*************************************************************************************
group_property::group_property( so_std::utf8_cref_t name )
{
    _name = name ;
}

//*************************************************************************************
group_property::group_property( this_rref_t rhv ) : 
    _name(std::move(rhv._name)), _properties( std::move(rhv._properties))
{
}

//*************************************************************************************
group_property::~group_property( void_t )
{
    for( auto * ptr : _properties )
    {
        ptr->destroy() ;
    }
}

//*************************************************************************************
group_property::this_ptr_t group_property::create( this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_property::memory::alloc( std::move( rhv ), p ) ;
}

//*************************************************************************************
void_t group_property::destroy( this_ptr_t ptr )
{
    so_property::memory::dealloc( ptr ) ;
}

//*************************************************************************************
bool_t group_property::add_child( iproperty_ptr_t pptr )
{
    auto const iter = std::find( _properties.begin(), _properties.end(), pptr ) ;
    if( iter != _properties.end() )
    {
        return false ;
    }

    _properties.push_back( pptr ) ;

    return true ;
}

//*************************************************************************************
so_std::utf8_cref_t group_property::get_name( void_t ) const
{
    return _name ;
}

//*************************************************************************************
void_t group_property::apply( so_property::ivisitor_ptr_t vptr )
{
    if( so_core::is_nullptr( vptr ) )
        return ;

    vptr->visit( this ) ;

    this_t::traverse_children( vptr ) ;

    vptr->post_visit( this ) ;
}

//*************************************************************************************
void_t group_property::destroy( void_t )
{
    this_t::destroy( this ) ;
}

//*************************************************************************************
void_t group_property::traverse_children( so_property::ivisitor_ptr_t vptr )
{
    for( auto * ptr : _properties )
    {
        ptr->apply( vptr ) ;
    }
}