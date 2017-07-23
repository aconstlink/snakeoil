//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_DEVICE_COMPINENTS_BUTTONS_BUTTON_STATE_H_
#define _SNAKEOIL_DEVICE_COMPINENTS_BUTTONS_BUTTON_STATE_H_


namespace so_device
{
    namespace so_component
    {
        enum class button_state
        {
            none,
            pressed,
            pressing,
            released
        };
    }
}

#endif
