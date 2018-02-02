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
        /// represents a vertical or horizontal scroll component
        struct scroll : public input_component<scroll>
        {
            size_t cid = size_t( -1 ) ;

            /// value < 0 : down/left
            /// value > 0 : up/right
            int_t value = 0 ;
        };

        so_typedef( scroll ) ;
    }
}
