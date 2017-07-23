//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_DEVICE_COMPONENTS_STICKS_STICK_STATE_H_
#define _SNAKEOIL_DEVICE_COMPONENTS_STICKS_STICK_STATE_H_

namespace so_device
{
    namespace so_component
    {
        enum class stick_state
        {
            none,
            untilted,
            tilted,
            tilting
        };
    }
}

#endif
