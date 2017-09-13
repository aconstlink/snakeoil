//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_DEVICE_COMPONENTS_BUTTONS_BINARY_BUTTON_H_
#define _SNAKEOIL_DEVICE_COMPONENTS_BUTTONS_BINARY_BUTTON_H_

#include "../input_component.h"
#include "button_state.h"

namespace so_device
{
    namespace so_input
    {
        struct binary_button : public input_component< binary_button >
        {
            // button id
            size_t bid = size_t( -1 ) ;

            // button state 
            button_state state = button_state::none ;


            bool_t state_is( button_state s )
            {
                return state == s ;
            }
        };

        so_typedef( binary_button ) ;
    }
}

#endif
