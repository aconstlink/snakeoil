//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "../../../component/buttons/button_state.h"
#include "../../../component/buttons/three_button.h"

#include <windows.h>

namespace so_device
{
    namespace so_win32
    {
        using so_device::button_state ;
        using so_device::three_button ;

        static button_state map_raw_button_state_left_to_button_state( USHORT const usButtonFlags )
        {
            if( usButtonFlags & RI_MOUSE_BUTTON_1_UP ) return button_state::released ;
            if( usButtonFlags & RI_MOUSE_BUTTON_1_DOWN ) return button_state::pressed ;
            return button_state::none ;
        }

        static button_state map_raw_button_state_right_to_button_state( USHORT const usButtonFlags )
        {
            if( usButtonFlags & RI_MOUSE_BUTTON_2_UP ) return button_state::released ;
            if( usButtonFlags & RI_MOUSE_BUTTON_2_DOWN ) return button_state::pressed ;
            return button_state::none ;
        }

        static button_state map_raw_button_state_middle_to_button_state( USHORT const usButtonFlags )
        {
            if( usButtonFlags & RI_MOUSE_BUTTON_3_UP ) return button_state::released ;
            if( usButtonFlags & RI_MOUSE_BUTTON_3_DOWN ) return button_state::pressed ;
            return button_state::none ;
        }
        
        static three_button map_raw_button_left_to_three_button( USHORT const usButtonFlags )
        {
            if( usButtonFlags & RI_MOUSE_BUTTON_1_DOWN ) return three_button::left ;
            if( usButtonFlags & RI_MOUSE_BUTTON_1_UP ) return three_button::left ;
            return three_button::none ;
        }

        static three_button map_raw_button_middle_to_three_button( USHORT const usButtonFlags )
        {
            if( usButtonFlags & RI_MOUSE_BUTTON_3_DOWN ) return three_button::middle ;
            if( usButtonFlags & RI_MOUSE_BUTTON_3_UP ) return three_button::middle ;
            return three_button::none ;
        }

        static three_button map_raw_button_right_to_three_button( USHORT const usButtonFlags )
        {
            if( usButtonFlags & RI_MOUSE_BUTTON_2_DOWN ) return three_button::right ;
            if( usButtonFlags & RI_MOUSE_BUTTON_2_UP ) return three_button::right ;
            return three_button::none ;
        }
    }
}

