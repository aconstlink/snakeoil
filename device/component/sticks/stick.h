//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "../input_component.h"
#include "stick_state.h"

#include <snakeoil/math/vector/vector2.hpp>

namespace so_device
{
    namespace so_input
    {
        struct stick : input_component<stick>
        {
            size_t cid = size_t( -1 ) ;

            stick_state state = stick_state::none ;

            /// in [-1,1]
            so_math::vec2f_t intensity = so_math::vec2f_t( 0.0f ) ;


        };
        so_typedef(stick) ;
    }
}