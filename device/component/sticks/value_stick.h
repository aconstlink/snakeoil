//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_DEVICE_COMPONENTS_STICKS_VALUE_STICK_H_
#define _SNAKEOIL_DEVICE_COMPONENTS_STICKS_VALUE_STICK_H_

#include "../input_component.h"
#include "stick_state.h"

#include <snakeoil/math/vector/vector2.hpp>

namespace so_device
{
    namespace so_component
    {
        struct value_stick : input_component<value_stick>
        {
            so_this_typedefs( value_stick ) ;

            size_t cid = size_t( -1 ) ;

            stick_state state_x = stick_state::none ;
            stick_state state_y = stick_state::none ;

            so_math::vec2i16_t value = so_math::vec2i16_t(0) ;
            so_math::vec2i16_t max_value = so_math::vec2i16_t((1<<15)-1);

            value_stick( void_t ){}
            value_stick( this_rref_t rhv )
            {
                cid = rhv.cid ;
                rhv.cid = size_t(-1) ;

                state_x = rhv.state_x ;
                state_y = rhv.state_y ;

                value = rhv.value ;
                max_value = rhv.max_value ;
            }

            /// returns the value in [-1,1]
            so_math::vec2f_t intensity( void_t ) const 
            {
                return so_math::vec2f_t(
                    float_t(value.x()) / float_t(max_value.x()),
                    float_t(value.y()) / float_t(max_value.y())
                    ) ; 
            }

            stick_state state( void_t ) const 
            {
                if( state_x == stick_state::tilting || state_y == stick_state::tilting )
                {
                    return stick_state::tilting ;
                }
                else if( state_x == stick_state::tilted || state_y == stick_state::tilted )
                {
                    return stick_state::tilted ;
                }
                else if( state_x == stick_state::untilted || state_y == stick_state::untilted )
                {
                    return stick_state::untilted ;
                }

                return stick_state::none ;
            }

            bool_t state_is( stick_state s )
            {
                return this_t::state() == s ;
            }

        };
        so_typedef( value_stick ) ;
    }
}

#endif

