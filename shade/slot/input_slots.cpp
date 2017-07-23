//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "input_slots.h"
#include "output_slots.h"

#include "../slot/input_slot.h"
#include "../slot/output_slot.h"

#include <snakeoil/log/log.h>

#include <algorithm>

using namespace so_shade ;

//*************************************************************************************
input_slots::input_slots( void_t ) 
{

}

//*************************************************************************************
input_slots::input_slots( this_rref_t rhv )
{
    _inputs = std::move( rhv._inputs ) ;
}

//*************************************************************************************
input_slots::~input_slots( void_t )
{    
    auto tmp = std::move(_inputs) ;
    for( auto slot_ptr : tmp )
    {
        slot_ptr->disconnect_output_slots() ;
        slot_ptr->destroy() ;
    }   
}

//*************************************************************************************
so_shade::result input_slots::connect_by_name( so_std::string_cref_t name, ioutput_slot_ptr_t in_ptr) 
{
    if( so_core::is_nullptr(in_ptr) )
        return so_shade::invalid_pointer ;

    auto iter = std::find_if( _inputs.begin(), _inputs.end(), [&]( so_shade::iinput_slot_ptr_t iptr )
    {
        return iptr->get_name() == name ;
    } ) ;

    if( iter == _inputs.end() )
        return so_shade::invalid_argument ;

    return (*iter)->connect(in_ptr) ;
}

//*************************************************************************************
so_shade::result input_slots::connect_by_name( so_std::string_cref_t my,
                so_std::string_cref_t other, output_slots_ptr_t osn_ptr ) 
{
    if( so_core::is_nullptr( osn_ptr ) )
        return so_shade::invalid_pointer ;

    auto * is_ptr = this_t::find_input_slot_by_name( my ) ;
    if( so_core::is_nullptr(is_ptr) )
        return so_shade::invalid_argname ;

    return osn_ptr->connect_by_name( other, is_ptr ) ;
}

//*************************************************************************************
so_shade::result input_slots::add_input_slot( iinput_slot_ptr_t sptr )
{
    if( so_core::is_nullptr(sptr) )
        return so_shade::invalid_pointer ;

    auto iter = std::find_if( _inputs.begin(), _inputs.end(), [&]( so_shade::iinput_slot_ptr_t iptr )
    {
        return iptr->get_name() == sptr->get_name() ;
    } ) ;

    if( iter != _inputs.end() )
        return so_shade::invalid_argument ;

    _inputs.push_back( sptr ) ;

    return so_shade::ok ;
}

//*************************************************************************************
input_slots::const_slots_t input_slots::find_all_free_input_slots( void_t ) const 
{
    const_slots_t ret ;

    for( auto * sptr : _inputs )
    {
        if( so_core::is_not(sptr->is_input_connected()) )
            ret.push_back( sptr ) ;
    }

    return std::move( ret ) ;
}

//*************************************************************************************
so_shade::iinput_slot_ptr_t input_slots::find_input_slot_by_name( so_std::string_cref_t name ) 
{
    auto iter = std::find_if( _inputs.begin(), _inputs.end(), [&]( so_shade::iinput_slot_ptr_t iptr )
    {
        return iptr->get_name() == name ;
    } ) ;

    if( iter == _inputs.end() )
        return nullptr ;

    return *iter ;
}

//*************************************************************************************
so_shade::result input_slots::remove_input_slot( iinput_slot_ptr_t sptr ) 
{
    if( so_core::is_nullptr(sptr) )
        return so_shade::invalid_argument ;

    auto iter = std::find( _inputs.begin(), _inputs.end(), sptr ) ;
    if( iter == _inputs.end() ) 
        return so_shade::invalid_argument ;

    _inputs.erase( iter ) ;

    return so_shade::ok ;
}

//*************************************************************************************
bool_t input_slots::try_connect_to_input_by_name( so_std::string_cref_t name,
            so_shade::iinput_slot_ptr_t to_ptr, so_shade::ioutput_slot_ptr_t incoming_ptr ) 
{
    if( to_ptr->get_name() == name )
    {
        auto const res = to_ptr->connect( incoming_ptr ) ;
        if( so_log::log::error( so_shade::no_success(res), 
            "[so_shade::input_slots::try_connect_to_input_by_name] : " + name ) )
        {
            return false ;
        }

        return true ;
    }
    return false ;
}

//*************************************************************************************
void_t input_slots::for_each_input( input_slot_funk_t funk ) 
{
    for( auto * sptr : _inputs )
    {
        funk( sptr ) ;
    }
}

//*************************************************************************************
types_t input_slots::get_input_slot_types( void_t ) const 
{
    types_t input_types ;
    input_types.reserve( _inputs.size() ) ;

    for( auto * is_ptr : _inputs )
    {
        input_types.push_back( is_ptr->get_type() ) ;
    }

    return std::move(input_types) ;
}
