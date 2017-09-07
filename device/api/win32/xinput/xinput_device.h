//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_DEVICE_API_WIN32_XINPUT_XINPUT_DEVICE_H_
#define _SNAKEOIL_DEVICE_API_WIN32_XINPUT_XINPUT_DEVICE_H_

#include "../../../typedefs.h"

#include "../../../component/buttons/button_state.h"
#include "../../../component/sticks/stick_state.h"

#include <snakeoil/math/vector/vector2.hpp>

#include <windows.h>
#include <Xinput.h>

namespace so_device
{
    namespace so_win32
    {
        /// helper device for state tracking
        class xinput_device
        {
            so_this_typedefs( xinput_device ) ;

        private:

            /// the xinput device id.
            DWORD _id = DWORD(-1) ;

            /// the current state
            XINPUT_STATE _state ;

        public:

            xinput_device( this_cref_t ) {}

        public:

            xinput_device( void_t ) ;
            xinput_device( this_rref_t ) ;
            xinput_device( DWORD id ) ;

            static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
            static void_t destroy( this_ptr_t ) ;

        public:

            DWORD get_id( void_t ) ;

            /// check if a pressed event occurred.
            bool_t is_pressed( XINPUT_STATE const & new_state, DWORD button ) const ;

            /// check if a pressing event occurred.
            bool_t is_pressing( XINPUT_STATE const & new_state, DWORD button ) const ;

            /// check if a released event occurred.
            bool_t is_released( XINPUT_STATE const & new_state, DWORD button ) const ;

            /// check the state of the left trigger
            so_component::button_state check_left_trigger( 
                XINPUT_STATE const & new_state, uint16_t & intensity_out ) const ;

            /// check the state of the right trigger
            so_component::button_state check_right_trigger( 
                XINPUT_STATE const & new_state, uint16_t & intensity_out ) const ;


            so_component::stick_state check_left_stick( 
                XINPUT_STATE const & new_state, so_math::vec2f_t & nnc_out ) const ;

            so_component::stick_state check_left_stick(
                XINPUT_STATE const & new_state, so_math::vector2<int16_t> & val_out ) const ;

            so_component::stick_state check_right_stick( 
                XINPUT_STATE const & new_state, so_math::vec2f_t & nnc_out ) const ;

            so_component::stick_state check_right_stick(
                XINPUT_STATE const & new_state, so_math::vector2<int16_t> & val_out ) const ;

            /// exchange the old xinput state with the new one.
            void_t exchange_state( XINPUT_STATE const & new_state ) ;            
        };
        so_typedef( xinput_device ) ;
    }
}

#endif
