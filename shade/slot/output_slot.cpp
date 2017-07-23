//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "output_slot.h"
#include "input_slot.h"

#include <snakeoil/log/log.h>

using namespace so_shade ;

//*************************************************************************************
output_slot::output_slot( so_std::string_cref_t name ) : _name(name)
{}

//*************************************************************************************
output_slot::output_slot( so_std::string_cref_t name, so_shade::type_cref_t t )  : 
    _name(name), _type(t)
{}

//*************************************************************************************
output_slot::output_slot( this_rref_t rhv ) : connector_registry( std::move(rhv) )
{
    _name = std::move(rhv._name) ;
    _type = std::move(rhv._type) ;
    _inputs = std::move(rhv._inputs) ;
}

//*************************************************************************************
output_slot::~output_slot( void_t ) 
{
    // move the slots to the tmp container so
    // there will be no cyclic disconnect calls
    slots_t tmp = std::move(_inputs) ;

    for( auto * sptr : tmp ) 
    {
        sptr->disconnect( this ) ;
    }
}

//*************************************************************************************
output_slot::this_ptr_t output_slot::create( this_rref_t rhv, so_memory::purpose_cref_t p ) 
{
    return so_shade::memory::alloc( std::move(rhv), p ) ;
}

//*************************************************************************************
void_t output_slot::destroy( this_ptr_t ptr ) 
{
    so_shade::memory::dealloc( ptr ) ;
}

//*************************************************************************************
so_shade::result output_slot::connect( so_shade::iinput_slot_ptr_t iptr ) 
{
    if( so_core::is_nullptr(iptr) )
        return so_shade::invalid_argument ;

    auto iter = std::find( _inputs.begin(), _inputs.end(), iptr ) ;
    if( iter != _inputs.end() )
        return so_shade::ok ;

    if( so_core::is_not( this_t::call_on_connect(iptr, this, connect_direction::input_to_output) ) )
    {
        so_log::log::error("[so_shade::output_slot::connect] : invalid connection") ;
        return so_shade::invalid_argument ;
    }

    _inputs.push_back( iptr ) ;

    return so_shade::ok ;
}

//*************************************************************************************
so_shade::result output_slot::disconnect( so_shade::iinput_slot_ptr_t sptr ) 
{
    if( so_core::is_nullptr(sptr) )
        return so_shade::ok ;

    auto iter = std::find( _inputs.begin(), _inputs.end(), sptr ) ;
    if( iter == _inputs.end() )
        return so_shade::invalid_argument ;

    this_t::call_on_disconnect( sptr ) ;

    // 1. take the slot out of the container
    _inputs.erase( iter ) ;
   
    // 3. call disconnect on the input slot
    return sptr->disconnect( this ) ;
}

//*************************************************************************************
so_shade::result output_slot::disconnect_input_slots( void_t ) 
{
    // 1. move the inputs so the inputs can call their disconnect
    slots_t tmp_slots = std::move(_inputs) ;

    // 2. calling disconnect on the tmp inputs
    for( auto * sptr : tmp_slots )
    {
        sptr->disconnect( this ) ;
    }

    return so_shade::ok ;
}

//*************************************************************************************
void_t output_slot::destroy( void_t ) 
{
    this_t::destroy( this ) ;
}

//*************************************************************************************
so_std::string_cref_t output_slot::get_name( void_t ) const 
{
    return _name ;
}

//*************************************************************************************
so_shade::type_cref_t output_slot::get_type( void_t ) const
{
    return _type ;
}

//**********************************************************************************************
void_t output_slot::set_type( so_shade::type_cref_t type_in ) 
{
    _type = type_in ;
}

//**********************************************************************************************
so_shade::result output_slot::register_connector( so_shade::iconnector_ptr_t ptr )
{
    return connector_registry::register_connector( ptr ) ;
}

//**********************************************************************************************
so_shade::result output_slot::unregister_connector( so_shade::iconnector_ptr_t ptr )
{
    return connector_registry::unregister_connector( ptr ) ;
}
