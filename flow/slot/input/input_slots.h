//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_FLOW_INPUT_SLOT_INPUT_SLOTS_H_
#define _SNAKEOIL_FLOW_INPUT_SLOT_INPUT_SLOTS_H_

#include "../iinput_slot.h"

#include <snakeoil/std/container/map.hpp>
#include <snakeoil/std/string/string.hpp>

namespace so_flow
{
    class SNAKEOIL_FLOW_API input_slots
    {
        so_this_typedefs( input_slots ) ;

        typedef so_std::map< so_flow::key_t, so_flow::iinput_slot_ptr_t > inputs_t ;

    private:

        inputs_t _inputs ;

    private:

        input_slots( this_cref_t ) {}

    public:

        input_slots( void_t ) ;
        input_slots( this_rref_t ) ;
        ~input_slots( void_t ) ;

    public:

        so_flow::result add_slot( so_flow::key_cref_t key, so_flow::iinput_slot_ptr_t ) ;
        so_flow::result remove_slot( so_flow::key_cref_t key ) ;
        so_flow::result remove_slot( so_flow::iinput_slot_ptr_t ) ;

        so_flow::iinput_slot_ptr_t get_slot( so_flow::key_cref_t ) ;


    };
    so_typedef( input_slots ) ;
}

#endif
