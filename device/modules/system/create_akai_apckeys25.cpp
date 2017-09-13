//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "system_module.h"

#include "../../devices/midi/midi_device.h"

#include "../../component/buttons/binary_button.h"
#include "../../component/buttons/value_button.h"
#include "../../component/sliders/slider.h"
#include "../../component/leds/binary_led.h"
#include "../../component/leds/multi_led.h"
#include "../../component/knobs/rotary_knob.h"

using namespace so_device ;

//*************************************************************************************
so_device::midi_device_ptr_t system_module::create_akai_apckey25( void_t )
{
    so_device::midi_device::device_info di ;
    di.device_name = "APC Key 25" ;
    so_device::midi_device mdev( di ) ;

    // buttons
    {
        for( size_t i=0; i<=39; ++i )
        {
            so_std::string_t i_str = std::to_string( i ) ;

            auto logic = so_device::midi_device::input_component_logic_t() ;
            logic.follow_up_funk = [=] ( so_device::so_input::iinput_component_ptr_t cptr )
            {
                auto & b = *reinterpret_cast< so_device::so_input::binary_button_ptr_t >( cptr ) ;
                if( b.state == so_device::button_state::pressed )
                {
                    b.state = so_device::button_state::pressing ;
                    return true ;
                }
                else if( b.state == so_device::button_state::pressing )
                {
                    return true ;
                }
                else if( b.state == so_device::button_state::released )
                {
                    b.state = so_device::button_state::none ;
                }
                return false ;
            } ;

            logic.midi_message_funk = [=] ( so_device::so_input::iinput_component_ptr_t cptr,
                so_device::midi_message_cref_t msg )
            {
                auto & b = *reinterpret_cast< so_device::so_input::binary_button_ptr_t >( cptr ) ;

                // press
                if( msg == so_device::midi_message_t( 144, byte_t( i ), 127, 0 ) )
                {
                    b.state = so_device::button_state::pressed ;
                    return true ;
                }
                // release
                else if( msg == so_device::midi_message_t( 128, byte_t( i ), 127, 0 ) )
                {
                    b.state = so_device::button_state::released ;
                    return true ;
                }
                return false ;
            } ;

            mdev.add_component( "b_" + i_str, logic,
                so_device::so_input::binary_button_t::create(
                    "[system_module::create_akai_midimix] : b_" + i_str ) ) ;
        }

        
    }

    // rouge binary buttons
    {
        so_std::vector< size_t > const ids = 
        { 64, 65, 66, 67, 68, 69, 70, 71, 82, 83, 84, 85, 86, 91, 93, 98 } ;

        for( auto i : ids )
        {
            so_std::string_t i_str = std::to_string( i ) ;

            auto logic = so_device::midi_device::input_component_logic_t() ;
            logic.follow_up_funk = [=] ( so_device::so_input::iinput_component_ptr_t cptr )
            {
                auto & b = *reinterpret_cast< so_device::so_input::binary_button_ptr_t >( cptr ) ;
                if( b.state == so_device::button_state::pressed )
                {
                    b.state = so_device::button_state::pressing ;
                    return true ;
                }
                else if( b.state == so_device::button_state::pressing )
                {
                    return true ;
                }
                else if( b.state == so_device::button_state::released )
                {
                    b.state = so_device::button_state::none ;
                }
                return false ;
            } ;

            logic.midi_message_funk = [=] ( so_device::so_input::iinput_component_ptr_t cptr,
                so_device::midi_message_cref_t msg )
            {
                auto & b = *reinterpret_cast< so_device::so_input::binary_button_ptr_t >( cptr ) ;

                // press
                if( msg.compare_s_b1( 144, byte_t( i ) ) )
                {
                    b.state = so_device::button_state::pressed ;
                    return true ;
                }
                // release
                else if( msg == so_device::midi_message_t( 128, byte_t( i ), 127, 0 ) )
                {
                    b.state = so_device::button_state::released ;
                    return true ;
                }
                return false ;
            } ;

            mdev.add_component( "b_" + i_str, logic,
                so_device::so_input::binary_button_t::create(
                    "[system_module::create_akai_midimix] : b_" + i_str ) ) ;
        }
    }

    // rouge binary buttons II
#if 0
    {
        so_std::vector< size_t > const ids = { 64, 81 } ;
        for( auto i : ids )
        {
            so_std::string_t i_str = std::to_string( i ) ;

            auto logic = so_device::midi_device::input_component_logic_t() ;
            logic.follow_up_funk = [=] ( so_device::so_input::iinput_component_ptr_t cptr )
            {
                auto & b = *reinterpret_cast< so_device::so_input::binary_button_ptr_t >( cptr ) ;
                if( b.state == so_device::button_state::pressed )
                {
                    b.state = so_device::button_state::pressing ;
                    return true ;
                }
                else if( b.state == so_device::button_state::pressing )
                {
                    return true ;
                }
                else if( b.state == so_device::button_state::released )
                {
                    b.state = so_device::button_state::none ;
                }
                return false ;
            } ;

            logic.midi_message_funk = [=] ( so_device::so_input::iinput_component_ptr_t cptr,
                so_device::midi_message_cref_t msg )
            {
                auto & b = *reinterpret_cast< so_device::so_input::binary_button_ptr_t >( cptr ) ;

                // press
                if( msg.compare_s_b1( 144, byte_t( i ) ) )
                {
                    b.state = so_device::button_state::pressed ;
                    return true ;
                }
                // release
                else if( msg == so_device::midi_message_t( 128, byte_t( i ), 0, 0 ) )
                {
                    b.state = so_device::button_state::released ;
                    return true ;
                }
                return false ;
            } ;

            mdev.add_component( "b_" + i_str, logic,
                so_device::so_input::binary_button_t::create(
                    "[system_module::create_akai_midimix] : b_" + i_str ) ) ;
        }
    }
#endif

    // piano value buttons
    {
        for( size_t id = 48; id <= 72; ++id )
        {
            so_std::string_t i_str = std::to_string( id ) ;

            auto logic = so_device::midi_device::input_component_logic_t() ;
            logic.follow_up_funk = [=] ( so_device::so_input::iinput_component_ptr_t cptr )
            {
                auto & b = *reinterpret_cast< so_device::so_input::value_button_ptr_t >( cptr ) ;
                if( b.state == so_device::button_state::pressed )
                {
                    b.state = so_device::button_state::pressing ;
                    return true ;
                }
                else if( b.state == so_device::button_state::pressing )
                {
                    return true ;
                }
                else if( b.state == so_device::button_state::released )
                {
                    b.state = so_device::button_state::none ;
                }
                return false ;
            } ;

            logic.midi_message_funk = [=] ( so_device::so_input::iinput_component_ptr_t cptr,
                so_device::midi_message_cref_t msg )
            {
                auto & b = *reinterpret_cast< so_device::so_input::value_button_ptr_t >( cptr ) ;

                byte_t const value = msg.byte2 ;

                // press
                if( msg.compare_s_b1( 145, byte_t( id ) ) )
                {
                    b.state = so_device::button_state::pressed ;
                    b.value = float_t(value) / 127.0f ;
                    return true ;
                }
                // release
                else if( msg == so_device::midi_message_t( 129, byte_t( id ), 0, 0 ) )
                {
                    b.state = so_device::button_state::released ;
                    return true ;
                }
                return false ;
            } ;

            mdev.add_component( "vb_" + i_str, logic,
                so_device::so_input::value_button_t::create(
                    "[system_module::create_akai_apckey25] : b_" + i_str ) ) ;
        }
    }

    // knobs
    {
        for( size_t i=48; i<=55; ++i )
        {
            so_std::string_t i_str = std::to_string( i ) ;

            auto logic = so_device::midi_device::input_component_logic_t() ;
            logic.follow_up_funk = [=] ( so_device::so_input::iinput_component_ptr_t cptr )
            {
                return false ;
            } ;

            logic.midi_message_funk = [=] ( so_device::so_input::iinput_component_ptr_t cptr,
                so_device::midi_message_cref_t msg )
            {
                auto & c = *reinterpret_cast< so_device::so_input::rotary_knob_ptr_t >( cptr ) ;

                if( msg.compare_s_b1( 176, byte_t( i ) ) )
                {
                    c.intensity = float_t( msg.byte2 ) / 127.0f ;
                    return true ;
                }

                return false ;
            } ;

            mdev.add_component( "k_" + i_str, logic,
                so_device::so_input::rotary_knob_t::create(
                    "[system_module::create_akai_apckey25] : Rotary Knob " + i_str ) ) ;
        }
    }

    // multi led colors:
    // 1: green
    // 2: green blink
    // 3: red
    // 4: red blink
    // 5: yellow 
    // 6: yellow blink

    // multi leds
    {
        typedef std::chrono::high_resolution_clock iclock_t ;

        for( size_t i = 0; i <= 39; ++i )
        {
            so_std::string_t i_str = std::to_string( i ) ;

            auto logic = so_device::midi_device::output_component_logic_t() ;

            logic.handle_funk = [=] ( so_device::so_output::ioutput_component_ptr_t cptr,
                so_device::midi_message_ref_t msg )
            {
                so_device::so_output::multi_led_ref_t led =
                    *reinterpret_cast< so_device::so_output::multi_led_ptr_t >( cptr ) ;

                so_device::led_state ls ;
                if( led.acquire_and_reset_changed_state( ls ) )
                {
                    if( ls == so_device::led_state::on )
                    {
                        led.tp = iclock_t::now() ;

                        msg = so_device::midi_message_t( 144, byte_t( i ), led.value, 0 ) ;
                        return so_device::midi_output_result::transmit_and_queue ;
                    }
                    else if( ls == so_device::led_state::off )
                    {
                        msg = so_device::midi_message_t( 144, byte_t( i ), 0, 0 ) ;
                        return so_device::midi_output_result::transmit_and_dismiss ;
                    }
                }
                // follow up
                else
                {
                    iclock_t::time_point tp = iclock_t::now() ;
                    iclock_t::duration d = tp - led.tp ;

                    size_t d_ms = size_t( std::chrono::duration_cast< std::chrono::milliseconds >( d ).count() ) ;
                    if( d_ms > led.blink_ms )
                    {
                        led.tp = tp ;
                        if( led.value != 0 )
                        {
                            led.tmp_value = led.value ;
                            led.value = 0 ;
                        }
                        else
                        {
                            led.value = led.tmp_value ;
                            led.tmp_value = 0 ;
                        }
                        msg = so_device::midi_message_t( 144, byte_t( i ), led.value, 0 ) ;
                        return so_device::midi_output_result::transmit_and_queue ;
                    }

                    return so_device::midi_output_result::queue ;
                }

                return so_device::midi_output_result::dismiss ; ;
            } ;

            mdev.add_component( "led_" + i_str, logic,
                so_device::so_output::multi_led_t::create(
                    "[system_module::create_akai_apckey25] : Led " + i_str ) ) ;
        }
    }

    // binary leds 
    {
        typedef std::chrono::high_resolution_clock iclock_t ;

        so_std::vector< size_t > const ids = { 64, 81 } ;
        for( auto i : ids )
        {
            so_std::string_t i_str = std::to_string( i ) ;

            auto logic = so_device::midi_device::output_component_logic_t() ;

            logic.handle_funk = [=] ( so_device::so_output::ioutput_component_ptr_t cptr,
                so_device::midi_message_ref_t msg )
            {
                so_device::so_output::binary_led_ref_t led =
                    *reinterpret_cast< so_device::so_output::binary_led_ptr_t >( cptr ) ;

                so_device::led_state ls ;
                if( led.acquire_and_reset_changed_state( ls ) )
                {
                    if( ls == so_device::led_state::on )
                    {
                        led.tp = iclock_t::now() ;
                        led.value = true ;

                        msg = so_device::midi_message_t( 144, byte_t( i ), 127, 0 ) ;
                        return so_device::midi_output_result::transmit_and_queue ;
                    }
                    else if( ls == so_device::led_state::off )
                    {
                        msg = so_device::midi_message_t( 144, byte_t( i ), 0, 0 ) ;
                        return so_device::midi_output_result::transmit_and_dismiss ;
                    }
                }
                // follow up
                else
                {
                    iclock_t::time_point tp = iclock_t::now() ;
                    iclock_t::duration d = tp - led.tp ;

                    size_t d_ms = size_t( std::chrono::duration_cast< std::chrono::milliseconds >( d ).count() ) ;
                    if( d_ms > led.blink_ms )
                    {
                        led.tp = tp ;
                        led.value = !led.value ;
                        
                        msg = so_device::midi_message_t( 144, byte_t( i ), led.value ? 127 : 0, 0 ) ;
                        return so_device::midi_output_result::transmit_and_queue ;
                    }

                    return so_device::midi_output_result::queue ;
                }

                return so_device::midi_output_result::dismiss ; ;
            } ;

            mdev.add_component( "led_" + i_str, logic,
                so_device::so_output::binary_led_t::create(
                    "[system_module::create_akai_apckey25] : Led " + i_str ) ) ;
        }
    }

    return so_device::midi_device_t::create( std::move( mdev ),
        "[system_module::create_akai_apckey25] : AKAI APC Key 25" ) ;
}