//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "inout_slot.h"

#include "input_slot.h"
#include "output_slot.h"

using namespace so_shade ;

//*************************************************************************************
inout_slot::inout_slot( so_std::string_cref_t name ) : _name(name)
{
    _input_ptr = so_shade::input_slot_t::create( name + ".input", 
        "[so_shade::inout_slot::inout_slot] : input_slot" ) ;    

    _output_ptr = so_shade::output_slot_t::create( name + ".output", 
        "[so_shade::inout_slot::inout_slot] : output_slot" ) ;

}

//*************************************************************************************
inout_slot::inout_slot( this_rref_t rhv )
{
    _name = std::move( rhv._name ) ;
    _type = std::move( rhv._type ) ;
    so_move_member_ptr( _input_ptr, rhv ) ;
    so_move_member_ptr( _output_ptr, rhv ) ;
}

//*************************************************************************************
inout_slot::~inout_slot( void_t )
{

}

//*************************************************************************************
inout_slot::this_ptr_t inout_slot::create( this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_shade::memory::alloc( std::move(rhv), p ) ;
}

//*************************************************************************************
void_t inout_slot::destroy( this_ptr_t ptr )
{
    so_shade::memory::dealloc( ptr ) ;
}

//*************************************************************************************
so_std::string_cref_t inout_slot::get_name( void_t ) const 
{
    return _name ;
}

//*************************************************************************************
so_shade::type_cref_t inout_slot::get_type( void_t ) const 
{
    return _type ;
}

//*************************************************************************************
void_t inout_slot::set_type( so_shade::type_cref_t type_in ) 
{
    _type = type_in ;
}

//*************************************************************************************
bool_t inout_slot::is_input_connected( void_t ) const
{
    return _input_ptr->is_input_connected() ;
}

//*************************************************************************************
so_shade::result inout_slot::connect( so_shade::ioutput_slot_ptr_t sptr )
{
    return _input_ptr->connect( sptr ) ;
}

//*************************************************************************************
so_shade::result inout_slot::connect( so_shade::iinput_slot_ptr_t sptr )
{
    return _output_ptr->connect( sptr ) ;
}

//*************************************************************************************
void_t inout_slot::destroy( void_t ) 
{
    this_t::destroy( this ) ;
}

//*************************************************************************************
so_shade::result inout_slot::disconnect( so_shade::ioutput_slot_ptr_t os_ptr ) 
{
    if( _output_ptr != os_ptr )
        return so_shade::invalid_pointer ;

    _output_ptr = nullptr ;

    return _input_ptr->disconnect( os_ptr ) ;
}

//*************************************************************************************
so_shade::result inout_slot::disconnect_output_slots( void_t )
{
    if( so_core::is_nullptr(_input_ptr) )
        return so_shade::ok ;

    return _input_ptr->disconnect_output_slots() ;
}

//*************************************************************************************
so_shade::result inout_slot::disconnect( so_shade::iinput_slot_ptr_t is_ptr )
{
    if( _input_ptr != is_ptr )
        return so_shade::invalid_pointer ;

    _input_ptr = nullptr ;

    return _output_ptr->disconnect( is_ptr ) ;
}

//*************************************************************************************
so_shade::result inout_slot::disconnect_input_slots( void_t )
{
    if(so_core::is_nullptr( _output_ptr ))
        return so_shade::ok ;

    return _output_ptr->disconnect_input_slots() ;
}

//*************************************************************************************
so_shade::result inout_slot::register_connector( so_shade::iconnector_ptr_t ptr )
{
    return connector_registry::register_connector( ptr ) ;
}

//*************************************************************************************
so_shade::result inout_slot::unregister_connector( so_shade::iconnector_ptr_t ptr )
{
    return connector_registry::unregister_connector( ptr ) ;
}
