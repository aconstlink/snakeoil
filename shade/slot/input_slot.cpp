//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "input_slot.h"

#include "output_slot.h"

#include <snakeoil/log/global.h>
#include <snakeoil/core/macros/move.h>

using namespace so_shade ;

//*************************************************************************************
input_slot::this_ptr_t input_slot::create( this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_shade::memory::alloc( std::move(rhv), p ) ;
}

//*************************************************************************************
void_t input_slot::destroy( this_ptr_t ptr )
{
    so_shade::memory::dealloc( ptr ) ;
}

//*************************************************************************************
input_slot::input_slot( so_std::string_cref_t name ) : _name(name)
{}

//*************************************************************************************
input_slot::input_slot( so_std::string_cref_t name, so_shade::type_cref_t t ) :
_name( name ), _type( t )
{}

//*************************************************************************************
input_slot::input_slot( this_rref_t rhv ) : connector_registry(std::move(rhv)) 
{
    _name = std::move( rhv._name ) ;
    _type = std::move( rhv._type ) ;
    so_move_member_ptr( _os_ptr, rhv ) ;
}

//*************************************************************************************
input_slot::~input_slot( void_t )
{
    // move the os to the tmp variable so
    // there will be no cyclic disconnect calls
    auto * tmp = _os_ptr ;
    _os_ptr = nullptr ;

    if( so_core::is_not_nullptr(tmp) )
        tmp->disconnect( this ) ;
}

//*************************************************************************************
bool_t input_slot::is_input_connected( void_t ) const 
{
    return so_core::is_not_nullptr( _os_ptr ) ;
}

//*************************************************************************************
so_shade::result input_slot::connect( so_shade::ioutput_slot_ptr_t os_ptr ) 
{
    if( so_core::is_nullptr(os_ptr) )
        return so_shade::invalid_argument ;

    // @todo need to implement reconnection
    so_assert( so_core::is_nullptr(_os_ptr) ) ;

    if( so_core::is_not_nullptr(os_ptr) )
    {
        // @todo check type compatibility
    }

    // ask the connector first, if the connection is valid
    if( so_core::is_not( this_t::call_on_connect( this, os_ptr, connect_direction::output_to_input ) ) )
    {
        so_log::global::error( "[so_shade::input_slot::connect] : invalid connection" ) ;
        return so_shade::invalid_argument ;
    }

    {
        auto const res = os_ptr->connect( this ) ;
        if( so_log::global::error( so_shade::no_success(res), 
            "[so_shade::input_slot::connect] : unable to connect to output" ) )
        {
            return res ;
        }
    }

    _os_ptr = os_ptr ;      

    return so_shade::ok ;
}

//*************************************************************************************
so_shade::result input_slot::disconnect( so_shade::ioutput_slot_ptr_t os_ptr ) 
{
    if( so_core::is_nullptr(os_ptr) )
        return so_shade::invalid_argument ;

    if( so_core::is_nullptr(_os_ptr) )
        return so_shade::ok ;

    if( _os_ptr != os_ptr ) 
        return so_shade::invalid_pointer ;

    // tell all connectors
    this_t::call_on_disconnect( os_ptr ) ;

    _os_ptr = nullptr ;

    return os_ptr->disconnect( this ) ;
}

//*************************************************************************************
so_shade::result input_slot::disconnect_output_slots( void_t ) 
{
    if( so_core::is_nullptr(_os_ptr) )
        return so_shade::ok ;

    auto * tmp = _os_ptr ;
    _os_ptr = nullptr ;

    return tmp->disconnect( this ) ;
}

//*************************************************************************************
void_t input_slot::destroy( void_t )
{
    this_t::destroy( this ) ;
}

//*************************************************************************************
so_std::string_cref_t input_slot::get_name( void_t ) const 
{
    return _name ;
}

//*************************************************************************************
so_shade::type_cref_t input_slot::get_type( void_t ) const
{
    return _type ;
}

//**********************************************************************************************
void_t input_slot::set_type( so_shade::type_cref_t type_in ) 
{
    _type = type_in ;
}

//**********************************************************************************************
so_shade::result input_slot::register_connector( so_shade::iconnector_ptr_t ptr )
{
    return connector_registry::register_connector( ptr ) ;
}

//**********************************************************************************************
so_shade::result input_slot::unregister_connector( so_shade::iconnector_ptr_t ptr )
{
    return connector_registry::unregister_connector( ptr ) ;
}
