//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_DEVICE_VDEV_GAMEPAD_XBOX_ENUMS_H_
#define _SNAKEOIL_DEVICE_VDEV_GAMEPAD_XBOX_ENUMS_H_

namespace so_device
{
    namespace so_vgamepad
    {
        enum class xbox_button
        {
            invalid, a, b, x, y, back, start
        };

        enum class xbox_dpad
        {
            invalid, left, up, right, down
        };

        enum class xbox_shoulder
        {
            invalid, left, right
        };

        enum class xbox_trigger
        {
            invalid, left, right
        };

        enum class xbox_stick
        {
            invalid, left, right
        };

        enum class xbox_motor
        {
            invalid, left, right
        };
        
        enum class xbox_button_state
        {
            invalid,
            pressed,
            pressing,
            released
        };

        enum class xbox_stick_state
        {
            invalid,
            tilt, 
            tilting,
            untilt
        };
    }
}

#endif
