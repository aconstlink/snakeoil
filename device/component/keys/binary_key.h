//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "../input_component.h"
#include "key_state.h"

namespace so_device
{
    namespace so_input
    {
        struct binary_key : public input_component< binary_key >
        {
            // button id
            size_t bid = size_t( -1 ) ;

            // button state 
            key_state state = key_state::none ;


            bool_t state_is( key_state s )
            {
                return state == s ;
            }
        };

        so_typedef( binary_key ) ;
    }
}

