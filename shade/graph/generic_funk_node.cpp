//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "generic_funk_node.h"

#include "../slot/input_slot.h"
#include "../slot/output_slot.h"
#include "../kernel/ikernel.h"

#include <snakeoil/log/global.h>

using namespace so_shade ;
using namespace so_shade::so_graph ;

//**********************************************************************************************
generic_funk_node::connector::connector( creator_ptr_t ptr ) : _creator_ptr(ptr)
{}

//**********************************************************************************************
generic_funk_node::connector::connector( this_rref_t rhv )
{
    so_move_member_ptr( _creator_ptr, rhv ) ;
}

//**********************************************************************************************
generic_funk_node::connector::~connector( void_t )
{}

//**********************************************************************************************
generic_funk_node::connector::this_ptr_t generic_funk_node::connector::create( this_rref_t rhv )
{
    return so_shade::memory::alloc( std::move(rhv), 
        "[so_shade::generic_funk_node::connector::create] : connector" ) ;
}

//**********************************************************************************************
void_t generic_funk_node::connector::destroy( this_ptr_t ptr )
{
    so_shade::memory::dealloc( ptr ) ;
}

//**********************************************************************************************
void_t generic_funk_node::connector::exchange_creator_by( creator_ptr_t ptr ) 
{
    _creator_ptr = ptr ;
}

//**********************************************************************************************
void_t generic_funk_node::connector::on_disconnect( so_shade::iinput_slot_cptr_t )
{
    
}

//**********************************************************************************************
void_t generic_funk_node::connector::on_disconnect( so_shade::ioutput_slot_cptr_t )
{
    
}

//**********************************************************************************************
bool_t generic_funk_node::connector::on_connect( so_shade::iinput_slot_ptr_t is_ptr,
    so_shade::ioutput_slot_ptr_t os_ptr, connect_direction dir )
{
    bool_t res = false ;

    if( dir == connect_direction::input_to_output )
    {
        res = _creator_ptr->check_kernel_slot_connectability( os_ptr ) ;
    }        
    else if( dir == connect_direction::output_to_input )
    {
        res = _creator_ptr->check_kernel_slot_connectability( os_ptr ) ;
        if( so_core::is(res) ) 
            is_ptr->set_type( os_ptr->get_type() ) ;

        _creator_ptr->determine_output_slot_type_from_inputs() ;
    }       

    return res ;
}

//**********************************************************************************************
void_t generic_funk_node::connector::destroy( void_t )
{
    this_t::destroy( this ) ;
}

//**********************************************************************************************
generic_funk_node::generic_funk_node( so_shade::so_kernel::ikernel_ptr_t kptr ) : 
    base_t(kptr->get_name()), _kernel_ptr( kptr )
{
    auto * con_ptr = this_t::connector::create( this_t::connector(this) ) ;

    // input slots
    {
        size_t const num_inputs = kptr->get_num_inputs() ;
        for( size_t i=0; i<num_inputs; ++i )
        {
            so_std::string const arg_name = "arg_" + std::to_string(i) ;
            auto * is_ptr = so_shade::input_slot_t::create( so_shade::input_slot_t( arg_name ),
                "[so_shade::generic_funk_node::generic_funk_node] : input_slot: " + arg_name ) ;

            auto const res = base_t::add_input_slot( is_ptr ) ;
            so_log::global::error( so_shade::no_success(res), 
                "[so_shade::generic_funk_node::generic_funk_node] : add input slot" ) ;

            is_ptr->register_connector( con_ptr ) ;
        }
    }

    // output slots
    {
        size_t const num_outputs = kptr->get_num_outputs() ;
        for( size_t i=0; i<num_outputs; ++i )
        {
            so_std::string const out_name = "out_" + std::to_string(i) ;
            auto * os_ptr = so_shade::output_slot_t::create( so_shade::output_slot_t(out_name),
                "[so_shade::generic_funk_node::generic_funk_node] : output_slot: " + out_name ) ;

            auto const res = base_t::add_output_slot( os_ptr ) ;
            so_log::global::error( so_shade::no_success( res ),
                "[so_shade::generic_funk_node::generic_funk_node] : add output slot" ) ;

            os_ptr->register_connector( con_ptr );
        }
    }

    _con_ptr = con_ptr ;
}

//**********************************************************************************************
generic_funk_node::generic_funk_node( this_rref_t rhv )  : base_t( std::move(rhv) )
{
    so_move_member_ptr( _kernel_ptr, rhv ) ;
    so_move_member_ptr( _con_ptr, rhv ) ;
    
    _con_ptr->exchange_creator_by( this ) ;
}

//**********************************************************************************************
generic_funk_node::~generic_funk_node( void_t )
{
    this_t::for_each_input( [&]( so_shade::iinput_slot_ptr_t sptr )
    {
        sptr->unregister_connector( _con_ptr ) ;
    }) ;

    this_t::for_each_output( [&] ( so_shade::ioutput_slot_ptr_t sptr )
    {
        sptr->unregister_connector( _con_ptr );
    } );

    if( so_core::is_not_nullptr( _con_ptr ) )
        _con_ptr->destroy() ;

    if( so_core::is_not_nullptr( _kernel_ptr ) )
        _kernel_ptr->destroy() ;
}

//**********************************************************************************************
generic_funk_node::this_ptr_t generic_funk_node::create( this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_shade::memory::alloc( std::move(rhv), p ) ;
}

//**********************************************************************************************
void_t generic_funk_node::destroy( this_ptr_t ptr )
{
    so_shade::memory::dealloc( ptr ) ;
}

//**********************************************************************************************
void_t generic_funk_node::destroy( void_t )
{
    this_t::destroy( this ) ;
}

//**********************************************************************************************
so_shade::result generic_funk_node::connect_by_name( so_std::string_cref_t name, 
    so_shade::ioutput_slot_ptr_t os_ptr ) 
{    
    return input_slots::connect_by_name( name, os_ptr ) ;
}

//**********************************************************************************************
bool_t generic_funk_node::check_kernel_slot_connectability( so_shade::ioutput_slot_cptr_t os_ptr )
{
    so_shade::types_t types ;
    types.push_back( os_ptr->get_type() ) ;

    return _kernel_ptr->validate_input_types( types ) ;
}

//**********************************************************************************************
void_t generic_funk_node::determine_output_slot_type_from_inputs( void_t ) 
{
    so_shade::types_t const types = std::move(this_t::get_input_slot_types()) ;
    so_shade::types_t const os_types = std::move(_kernel_ptr->determine_output_types( types )) ;

    for( size_t i=0; i<_kernel_ptr->get_num_outputs(); ++i )
    {
        auto * os_ptr = this_t::get_output_slot( i ) ;
        if( so_core::is_not_nullptr(os_ptr) )
            os_ptr->set_type( os_types[i] ) ;
    }
}
