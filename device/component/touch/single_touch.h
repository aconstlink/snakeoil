//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "../input_component.h"
#include "touch_state.h"

#include <snakeoil/math/vector/vector2.hpp>

namespace so_device
{
    namespace so_input
    {
        struct single_touch: public input_component< single_touch >
        {
            size_t bid = size_t( -1 ) ;
            touch_state state = touch_state::none ;

            // normalized positive coordinate
            so_math::vec2f_t npc ;

            // normalized centered coordinate
            so_math::vec2f_t ncc ;

            bool_t state_is( touch_state s )
            {
                return state == s ;
            }
        };

        so_typedef( single_touch ) ;
    }
}