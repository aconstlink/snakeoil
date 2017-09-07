//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_DEVICE_API_XINPUT_XINPUT_ENUMS_H_
#define _SNAKEOIL_DEVICE_API_XINPUT_XINPUT_ENUMS_H_

namespace so_device
{
    namespace so_win32
    {
        enum class xinput_inputs
        {
            button_back,            // XINPUT_GAMEPAD_BACK
            button_start,           // XINPUT_GAMEPAD_START
            button_a,               // XINPUT_GAMEPAD_A
            button_b,               // XINPUT_GAMEPAD_B
            button_x,               // XINPUT_GAMEPAD_X
            button_y,               // XINPUT_GAMEPAD_Y
            button_thumb_left,      // XINPUT_GAMEPAD_LEFT_THUMB
            button_thumb_right,     // XINPUT_GAMEPAD_RIGHT_THUMB
            button_shoulder_left,   // XINPUT_GAMEPAD_LEFT_SHOULDER
            button_shoulder_right,  // XINPUT_GAMEPAD_RIGHT_SHOULDER

            button_dpad_up,         // XINPUT_GAMEPAD_DPAD_UP
            button_dpad_down,       // XINPUT_GAMEPAD_DPAD_DOWN
            button_dpad_left,       // XINPUT_GAMEPAD_DPAD_LEFT
            button_dpad_right,      // XINPUT_GAMEPAD_DPAD_RIGHT
            
            stick_left,
            stick_right,

            trigger_left,
            trigger_right,
            num_inputs
        };

        enum class xinput_outputs
        {
            motor_left,
            motor_right,
            num_outputs
        };
    }
}

#endif
