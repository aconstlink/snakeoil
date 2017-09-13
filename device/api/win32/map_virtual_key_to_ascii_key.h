//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "../../component/keys/ascii_key.h"
#include "../../component/keys/key_state.h"

#include <windows.h>

namespace so_device
{
    namespace so_win32
    {
        static so_device::ascii_key map_virtual_keycode_to_ascii_key( WPARAM const wparam )
        {
            if( wparam >= 0x30 && wparam <= 0x39 )
            {
                size_t dif = size_t( wparam - 0x30 ) ;
                return convert_ascii_number_keys( dif ) ;
            }

            if( wparam >= 0x41 && wparam <= 0x5a )
            {
                size_t dif = size_t( wparam - 0x41 ) ;
                return convert_ascii_letter_keys( dif ) ;
            }

            // 0 - 9 on numpad
            if( wparam >= 0x60 && wparam <= 0x69 )
            {
                size_t dif = size_t( wparam - 0x60 ) ;
                return convert_ascii_numpad_number_keys( dif ) ;
            }

            // VK_F1 - VK_F12
            if( wparam >= 0x70 && wparam <= 0x7B )
            {
                size_t dif = size_t( wparam - 0x70 ) ;
                return convert_ascii_function_keys( dif ) ;
            }

            switch( wparam )
            {
            case VK_BACK: return ascii_key::back_space ;
            case VK_TAB: return ascii_key::tab ;
            case VK_RETURN: return ascii_key::k_return ;
            case VK_ESCAPE: return ascii_key::escape ;
            case VK_SPACE: return ascii_key::space ;
            case VK_LEFT: return ascii_key::arrow_left ;
            case VK_UP: return ascii_key::arrow_up ;
            case VK_RIGHT: return ascii_key::arrow_right ;
            case VK_DOWN: return ascii_key::arrow_down ;
            case VK_SHIFT: return ascii_key::shift_left ;
            case VK_LSHIFT: return ascii_key::shift_left ;
            case VK_RSHIFT: return ascii_key::shift_right ;
            case VK_CONTROL: return ascii_key::ctrl_left ;
            case VK_LCONTROL: return ascii_key::ctrl_left ;
            case VK_RCONTROL: return ascii_key::ctrl_right ;
            case VK_ADD: return ascii_key::num_add ;
            case VK_SUBTRACT: return ascii_key::num_sub ;
            case VK_MENU: return ascii_key::context ;
            case 0xBD: return ascii_key::minus ;
            case 0xBB: return ascii_key::plus ;

            default: return ascii_key::invalid ;
            }
        }

        /// map with window message
        static so_device::key_state map_key_state_raw_wm_for_vzt_device( UINT const msg )
        {
            using so_device::key_state ;

            switch( msg )
            {
            case WM_KEYUP: return key_state::released;
            case WM_KEYDOWN: return key_state::pressed ;
            }
            return key_state::none ;
        }
    }
}