//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_DEVICE_VDEV_GAMEPAD_XBOX_XBOX_360_H_
#define _SNAKEOIL_DEVICE_VDEV_GAMEPAD_XBOX_XBOX_360_H_

#include "../../ivdev.h"
#include "xbox_enums.h"

#include <snakeoil/std/container/map.hpp>
#include <snakeoil/math/vector/vector2.hpp>

namespace so_device
{
    namespace so_vgamepad
    {
        class SNAKEOIL_DEVICE_API xbox_360 : public so_vdev::ivdev
        {
            so_this_typedefs( xbox_360 ) ;


            typedef so_std::map< xbox_button, xbox_button_state > __button_states_t ;
            so_typedefs( __button_states_t, button_states ) ;

            typedef so_std::map< xbox_shoulder, xbox_button_state > __shoulder_states_t ;
            so_typedefs( __shoulder_states_t, shoulder_states ) ;

            typedef so_std::map< xbox_dpad, xbox_button_state > __dpad_states_t ;
            so_typedefs( __dpad_states_t, dpad_states ) ;

            struct stick_data
            {
                xbox_stick_state s ;
                xbox_button_state bs ;
                so_math::vec2f_t value ;
            };
            typedef so_std::map< xbox_stick, stick_data > __stick_states_t ;
            so_typedefs( __stick_states_t, stick_states ) ;

            struct trigger_data
            {
                xbox_button_state s ;
                float_t value ;
            };
            typedef so_std::map< xbox_trigger, trigger_data > __trigger_states_t ;
            so_typedefs( __trigger_states_t, trigger_states ) ;

        private:

            button_states_t _button_states ;
            shoulder_states_t _shoulder_states ;
            dpad_states_t _dpad_states ;
            stick_states_t _stick_states ;
            trigger_states_t _trigger_states ;

        public: 

            static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
            static void_t destroy( this_ptr_t ) ;
            
        public: // buttons

            bool_t is_pressed( so_device::so_vgamepad::xbox_button ) const ;
            bool_t is_pressing( so_device::so_vgamepad::xbox_button ) const  ;
            bool_t is_released( so_device::so_vgamepad::xbox_button ) const ;

        public: // shoulder

            bool_t is_pressed( so_device::so_vgamepad::xbox_shoulder ) const ;
            bool_t is_pressing( so_device::so_vgamepad::xbox_shoulder ) const ;
            bool_t is_released( so_device::so_vgamepad::xbox_shoulder ) const ;

        public: // dpad

            bool_t is_pressed( so_device::so_vgamepad::xbox_dpad ) const ;
            bool_t is_pressing( so_device::so_vgamepad::xbox_dpad ) const ;
            bool_t is_released( so_device::so_vgamepad::xbox_dpad ) const ;

        public: // trigger

            bool_t any_trigger_changed( void_t ) const ;
            bool_t is_pressed( so_device::so_vgamepad::xbox_trigger, float_out_t ) const ;
            bool_t is_pressing( so_device::so_vgamepad::xbox_trigger, float_out_t ) const ;
            bool_t is_released( so_device::so_vgamepad::xbox_trigger ) const ;

        public: // sticks

            bool_t any_tilt_changed( void_t ) const ;
            bool_t is_tilt( so_device::so_vgamepad::xbox_stick, so_math::vec2f_out_t ) const ;
            bool_t is_tilting( so_device::so_vgamepad::xbox_stick, so_math::vec2f_out_t ) const ;
            bool_t is_untilt( so_device::so_vgamepad::xbox_stick ) const ;

            bool_t is_pressed( so_device::so_vgamepad::xbox_stick ) const ;
            bool_t is_pressing( so_device::so_vgamepad::xbox_stick ) const  ;
            bool_t is_released( so_device::so_vgamepad::xbox_stick ) const ;

        public: // motor

            bool_t set_motor_on( so_device::so_vgamepad::xbox_motor, float_t ) ;
            bool_t set_motor_off( so_device::so_vgamepad::xbox_motor ) ;

        public: // query

            bool_t is_active( void_t ) const ;

        public:

            bool_t set_shoulder_state( so_device::so_vgamepad::xbox_shoulder,
                so_device::so_vgamepad::xbox_button_state ) ;

            bool_t set_button_state( so_device::so_vgamepad::xbox_button,
                so_device::so_vgamepad::xbox_button_state ) ;

            bool_t set_dpad_state( so_device::so_vgamepad::xbox_dpad,
                so_device::so_vgamepad::xbox_button_state ) ;

            bool_t set_stick_state( so_device::so_vgamepad::xbox_stick,
                so_device::so_vgamepad::xbox_stick_state, so_math::vec2f_in_t ) ;

            bool_t set_stick_state( so_device::so_vgamepad::xbox_stick,
                so_device::so_vgamepad::xbox_button_state ) ;

            bool_t set_trigger_state( so_device::so_vgamepad::xbox_trigger,
                so_device::so_vgamepad::xbox_button_state, float_t ) ;

        public: // 

            virtual void_t reset( void_t ) ;

        public:

            virtual void_t destroy( void_t ) ;
        };
        so_typedef( xbox_360 ) ;
    }
}

#endif
