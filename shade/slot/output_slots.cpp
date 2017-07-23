//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "output_slots.h"
#include "input_slots.h"

#include "../slot/output_slot.h"
#include "../slot/input_slot.h"

#include <algorithm>

using namespace so_shade ;

//*************************************************************************************
output_slots::output_slots( void_t ) 
{}

//*************************************************************************************
output_slots::output_slots( this_rref_t rhv )
{
    _outputs = std::move( rhv._outputs ) ;
}

//*************************************************************************************
output_slots::~output_slots( void_t )
{
    auto tmp = std::move(_outputs) ;
    for( auto * optr : tmp )
    {
        optr->disconnect_input_slots() ;
        optr->destroy() ;
    }
}

//*************************************************************************************
so_shade::ioutput_slot_ptr_t output_slots::find_output_slot_by_name( so_std::string_cref_t name )
{
    auto iter = std::find_if( _outputs.begin(), _outputs.end(), [&]( so_shade::ioutput_slot_ptr_t optr )
    {
        return optr->get_name() == name ;
    } ) ;

    if( iter == _outputs.end() )
        return nullptr ;

    return *iter ;
}

//*************************************************************************************
so_shade::ioutput_slot_ptr_t output_slots::get_output_slot( size_t i ) 
{
    if( i>=_outputs.size() )
        return nullptr ;

    return _outputs[i] ;
}

//*************************************************************************************
so_shade::result output_slots::connect_by_name( so_std::string_cref_t name, iinput_slot_ptr_t iptr ) 
{
    if( so_core::is_nullptr(iptr) )
        return so_shade::invalid_pointer ;

    auto iter = std::find_if( _outputs.begin(), _outputs.end(), [&]( so_shade::ioutput_slot_ptr_t optr )
    {
        return optr->get_name() == name ;
    } ) ;

    if( iter == _outputs.end() )
        return so_shade::invalid_argument ;

    return (*iter)->connect( iptr ) ;
}

//*************************************************************************************
so_shade::result output_slots::connect_by_name( so_std::string_cref_t my,
                so_std::string_cref_t other, input_slots_ptr_t isn_ptr ) 
{
    if( so_core::is_nullptr( isn_ptr ) )
        return so_shade::invalid_pointer ;

    auto * os_ptr = this_t::find_output_slot_by_name( my ) ;
    if( so_core::is_nullptr( os_ptr ) )
        return so_shade::invalid_argname ;

    return isn_ptr->connect_by_name( other, os_ptr ) ;
}

//*************************************************************************************
so_shade::result output_slots::add_output_slot( ioutput_slot_ptr_t in_ptr )
{
    if( so_core::is_nullptr(in_ptr) ) 
        return so_shade::invalid_pointer ;

    auto iter = std::find_if( _outputs.begin(), _outputs.end(), [&]( so_shade::ioutput_slot_ptr_t optr )
    {
        return optr->get_name() == in_ptr->get_name() ;
    } ) ;

    if( iter != _outputs.end() )
        return so_shade::invalid_argument ;

    _outputs.push_back( in_ptr ) ;

    return so_shade::ok ;
}

//*************************************************************************************
void_t output_slots::for_each_output( output_slot_funk_t funk ) 
{
    for( auto * sptr : _outputs )
    {
        funk( sptr ) ;
    }
}

//*************************************************************************************
types_t output_slots::get_output_types( void_t ) const 
{
    types_t output_types ;
    output_types.reserve( _outputs.size() ) ;

    for( auto * sptr : _outputs ) 
    {
        output_types.push_back( sptr->get_type() ) ;
    }

    return std::move( output_types ) ;
}
