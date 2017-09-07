//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_DEVICE_COMPONENTS_LIGHTS_LIGHT_BAR_H_
#define _SNAKEOIL_DEVICE_COMPONENTS_LIGHTS_LIGHT_BAR_H_

#include "../output_component.h"
#include "light_state.h"

#include <chrono>

namespace so_device
{
    namespace so_component
    {
        /// a led in active state can be on or off
        /// an inactive led can not be on.
        struct light_bar : public output_component<light_bar>
        {
            size_t cid = size_t( -1 ) ;

            float_t value = 0.0f ;

        public:

            void_t change_state_to( bool_t b )
            {
                _last_state = _cur_state ;
                _cur_state = b ? light_state::on : light_state::off ;
            }

            void_t change_to_on( void_t )
            {
                _last_state = _cur_state ;
                _cur_state = light_state::on ;
            }

            void_t change_to_off( void_t )
            {
                if( _cur_state != light_state::off )
                {
                    _last_state = _cur_state ;
                    _cur_state = light_state::off ;
                }
            }

            bool_t acquire_and_reset_changed_state( light_state & state_out )
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

            light_state _last_state = light_state::undefined ;
            light_state _cur_state = light_state::undefined ;
        };
        so_typedef( light_bar ) ;
    }
}

#endif
