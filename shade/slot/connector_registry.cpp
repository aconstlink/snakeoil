//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "connector_registry.h"

using namespace so_shade ;

//**********************************************************************************************
connector_registry::connector_registry( void_t )
{

}

//**********************************************************************************************
connector_registry::connector_registry( this_rref_t rhv )
{
    _connectors = std::move( rhv._connectors ) ;
}

//**********************************************************************************************
connector_registry::~connector_registry( void_t )
{
    for( auto cptr : _connectors )
        cptr->destroy() ;
}

//**********************************************************************************************
so_shade::result connector_registry::register_connector( so_shade::iconnector_ptr_t cptr )
{
    if( so_core::is_nullptr(cptr) )
        return so_shade::invalid_pointer ;

    auto iter = std::find( _connectors.begin(), _connectors.end(), cptr ) ;
    if( iter != _connectors.end() )
        return so_shade::invalid_argument ;

    _connectors.push_back( cptr ) ;

    return so_shade::ok ;
}

//**********************************************************************************************
so_shade::result connector_registry::unregister_connector( so_shade::iconnector_ptr_t cptr )
{
    if( so_core::is_nullptr( cptr ) )
        return so_shade::invalid_pointer;

    auto iter = std::find( _connectors.begin(), _connectors.end(), cptr );
    if( iter == _connectors.end() )
        return so_shade::invalid_argument;

    _connectors.erase( iter ) ;

    return so_shade::ok;
}

//**********************************************************************************************
bool_t connector_registry::call_on_connect( so_shade::iinput_slot_ptr_t is_ptr,
    so_shade::ioutput_slot_ptr_t os_ptr, connect_direction dir )
{
    for( auto ptr : _connectors )
    {
        if( so_core::is_not( ptr->on_connect( is_ptr, os_ptr, dir ) ) )
            return false;
    }

    return true;
}

//**********************************************************************************************
void_t connector_registry::call_on_disconnect( so_shade::iinput_slot_ptr_t is_ptr )
{
    for( auto ptr : _connectors )
    {
        ptr->on_disconnect( is_ptr ) ;
    }
}

//**********************************************************************************************
void_t connector_registry::call_on_disconnect( so_shade::ioutput_slot_ptr_t os_ptr )
{
    for( auto ptr : _connectors )
    {
        ptr->on_disconnect( os_ptr );
    }
}

//**********************************************************************************************

