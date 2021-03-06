//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "../input_component.h"
#include "button_state.h"

namespace so_device
{
    namespace so_input
    {
        struct value_button : public input_component< value_button >
        {
            size_t bid = size_t( -1 ) ;

            button_state state = button_state::none ;

            float_t intensity( void_t ) const
            {
                return value ;
            }

            bool_t state_is( button_state s )
            {
                return state == s ;
            }

            float_t value = 0.0f ;
        };
        so_typedef( value_button ) ;
    }
}

