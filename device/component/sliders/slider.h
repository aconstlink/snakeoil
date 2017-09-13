//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "../input_component.h"
#include "slider_state.h"

namespace so_device
{
    namespace so_input
    {
        /// we define a slide component as something
        /// that can slide left/right or up/down
        /// @note absolute position(unlike scroll)
        struct slider : public input_component<slider>
        {
            size_t cid = size_t( -1 ) ;

            slider_state state = slider_state::none ;

            /// in [0,1]
            float_t intensity = 0.0f ;
        };

        so_typedef( slider ) ;
    }
}
