//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_DEVICE_COMPONENTS_KNOBS_ROTARY_KNOB_H_
#define _SNAKEOIL_DEVICE_COMPONENTS_KNOBS_ROTARY_KNOB_H_

#include "../input_component.h"

namespace so_device
{
    namespace so_component
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

#endif
