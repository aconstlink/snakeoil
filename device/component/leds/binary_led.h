//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "../output_component.h"
#include "led_state.h"

#include <chrono>

namespace so_device
{
    namespace so_output
    {
        /// a led in active state can be on or off
        /// an inactive led can not be on.
        struct binary_led : public output_component<binary_led>
        {            
            size_t cid = size_t( -1 ) ;
            
            typedef std::chrono::high_resolution_clock clock_t ;
            clock_t::time_point tp ;
            
            /// blink each ms
            /// -1 : no blink
            size_t blink_ms = size_t(-1) ;

            bool_t value = false ;

        public:

            void_t change_state_to( bool_t b )
            {
                _last_state = _cur_state ;
                _cur_state = b ? led_state::on : led_state::off ;
            }

            void_t change_to_on( void_t )
            {
                blink_ms = size_t(-1)  ;
                _last_state = _cur_state ;
                _cur_state = led_state::on ;
            }

            void_t change_to_on_and_blink( size_t blink_ms_ )
            {
                blink_ms = blink_ms_ ;
                _last_state = _cur_state ;
                _cur_state = led_state::on ;
            }

            void_t change_to_off( void_t )
            {
                if( _cur_state != led_state::off )
                {
                    _last_state = _cur_state ;
                    _cur_state = led_state::off ;
                }
            }

            bool_t acquire_and_reset_changed_state( led_state & state_out )
            {
                if( _last_state != _cur_state )
                {
                    _last_state = _cur_state ;
                    state_out = _cur_state ;
                    return true ;
                }
                state_out = _last_state ;
                return false ;
            }

        private:

            led_state _last_state = led_state::undefined ;
            led_state _cur_state = led_state::undefined ;
        };
        so_typedef( binary_led ) ;
    }
}
