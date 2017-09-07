//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_DEVICE_COMPINENTS_TOUCH_TOUCH_STATE_H_
#define _SNAKEOIL_DEVICE_COMPINENTS_TOUCH_TOUCH_STATE_H_


namespace so_device
{
    namespace so_component
    {
        enum class touch_state
        {
            none,
            touched,
            touching,
            released
        };
    }
}

#endif
