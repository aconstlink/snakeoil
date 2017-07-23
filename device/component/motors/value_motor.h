//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_DEVICE_COMPONENTS_MOTORS_VALUE_MOTOR_H_
#define _SNAKEOIL_DEVICE_COMPONENTS_MOTORS_VALUE_MOTOR_H_

#include "../output_component.h"
#include "motor_state.h"

namespace so_device
{
    namespace so_component
    {
        struct value_motor : public output_component<value_motor>
        {
            size_t cid = size_t( -1 ) ;
            
            uint16_t value = uint16_t( 0 ) ;
            uint16_t max_value = uint16_t( -1 ) ;
            
            /// allows to set the value by [0,1] float
            void_t set_by_intensity( float_t const v )
            {
                value = uint16_t(v * float_t(max_value)) ;
            }
            
            void_t set_on( void_t ) 
            {
                _last_state = _cur_state ;
                _cur_state = motor_state::on ;
            }

            void_t set_off( void_t )
            {
                _last_state = _cur_state ;
                _cur_state = motor_state::off ;
            }

            bool_t is_on( void_t ) const
            {
                return _cur_state == motor_state::on ;
            }

            bool_t is_off( void_t ) const
            {
                return _cur_state == motor_state::off ;
            }

            bool_t acquire_and_reset_changed_state( motor_state & state_out )
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

            motor_state _last_state = motor_state::undefined ;
            motor_state _cur_state = motor_state::undefined ;
        };

        so_typedef( value_motor ) ;
    }
}

#endif
