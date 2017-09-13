//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "../input_component.h"

namespace so_device
{
    namespace so_input
    {
        struct rotary_knob : public input_component<rotary_knob>
        {
            size_t cid = size_t( -1 ) ;

            /// in [0,1]
            float_t intensity = 0.0f ;
        };

        so_typedef( rotary_knob ) ;
    }
}
