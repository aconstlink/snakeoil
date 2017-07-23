//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "input_slots.h"

#include "input_slot.h"

#include <snakeoil/variable/ivariable.h>
#include <snakeoil/log/log.h>

using namespace so_flow ;

//***********************************************************************
input_slots::input_slots( void_t ) 
{}

//***********************************************************************
input_slots::input_slots( this_rref_t rhv )
{
    _inputs = std::move( rhv._inputs ) ;
}

//***********************************************************************
input_slots::~input_slots( void_t )
{
   
}

//***********************************************************************
so_flow::result input_slots::add_slot( so_flow::key_cref_t key, so_flow::iinput_slot_ptr_t is_ptr ) 
{
    if( so_core::is_nullptr(is_ptr) )
        return so_flow::invalid_argument ;

    auto iter = std::find( _inputs.begin(), _inputs.end(), key ) ;
    if( iter != _inputs.end() )
        return so_flow::invalid_argument ;


}

//***********************************************************************
so_flow::result input_slots::remove_slot( so_flow::key_cref_t key ) 
{

}

//***********************************************************************
so_flow::result input_slots::remove_slot( so_flow::iinput_slot_ptr_t ) 
{

}

//***********************************************************************
so_flow::iinput_slot_ptr_t input_slots::get_slot( so_flow::key_cref_t ) 
{

}
