//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "akai.h"

#include "../../../component/buttons/binary_button.h"
#include "../../../component/sliders/slider.h"
#include "../../../component/leds/binary_led.h"
#include "../../../component/knobs/rotary_knob.h"

#include <chrono>

using namespace so_device ;
using namespace so_device::so_midi ;

//****************************************************************************************
so_device::midi_device_ptr_t akai::create_midimix( void_t ) 
{
    so_device::midi_device::device_info di ;
    di.device_name = "MIDI Mix" ;
    so_device::midi_device mdev( di ) ;

    // buttons
    {
        so_std::vector< size_t > ids = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24 } ;
        for( auto i : ids )
        {
            so_std::string_t i_str = std::to_string( i ) ;

            auto logic = so_device::midi_device::input_component_logic_t() ;
            logic.follow_up_funk = [=]( so_device::so_component::iinput_component_ptr_t cptr )
            {
                auto & b = *reinterpret_cast<so_device::so_component::binary_button_ptr_t>( cptr ) ;
                if( b.state == so_device::so_component::button_state::pressed )
                {
                    b.state = so_device::so_component::button_state::pressing ;
                    return true ;
                }
                else if( b.state == so_device::so_component::button_state::pressing )
                {
                    return true ;
                }
                else if( b.state == so_device::so_component::button_state::released )
                {
                    b.state = so_device::so_component::button_state::none ;
                }
                return false ;
            } ;

            logic.midi_message_funk = [=]( so_device::so_component::iinput_component_ptr_t cptr,
                so_device::midi_message_cref_t msg )
            {
                auto & b = *reinterpret_cast<so_device::so_component::binary_button_ptr_t>( cptr ) ;

                // press
                if( msg == so_device::midi_message_t( 144, byte_t( i ), 127, 0 ) )
                {
                    b.state = so_device::so_component::button_state::pressed ;
                    return true ;
                }
                // release
                else if( msg == so_device::midi_message_t( 128, byte_t( i ), 127, 0 ) )
                {
                    b.state = so_device::so_component::button_state::released ;
                    return true ;
                }
                return false ;
            } ;

            mdev.add_component( "b_" + i_str, logic,
                so_device::so_component::binary_button_t::create( 
                "[so_device::akai::create_midimix] : b_" + i_str ) ) ;
        }
    }

    // sliders
    {
        so_std::vector< size_t > ids = { 19, 23, 27, 31, 49, 53, 57, 61, 62 } ;
        for( auto i : ids )
        {
            so_std::string_t i_str = std::to_string( i ) ;

            auto logic = so_device::midi_device::input_component_logic_t() ;
            logic.follow_up_funk = [=]( so_device::so_component::iinput_component_ptr_t cptr )
            {
                return false ;
            } ;

            logic.midi_message_funk = [=]( so_device::so_component::iinput_component_ptr_t cptr,
                so_device::midi_message_cref_t msg )
            {
                auto & c = *reinterpret_cast<so_device::so_component::slider_ptr_t>( cptr ) ;

                if( msg.compare_s_b1( 176, byte_t( i ) ) )
                {
                    c.intensity = float_t( msg.byte2 ) / 127.0f ;
                    return true ;
                }

                return false ;
            } ;

            mdev.add_component( "s_" + i_str, logic,
                so_device::so_component::slider_t::create( 
                "[so_device::akai::create_midimix] : Slider " + i_str ) ) ;
        }
    }

    // knobs
    {
        so_std::vector< size_t > ids = { 16, 17, 18, 20, 21, 22,24,25,26,28,29,30,46,47,48,50,51,52,54,55,56,58,59,60 } ;
        for( auto i : ids )
        {
            so_std::string_t i_str = std::to_string( i ) ;

            auto logic = so_device::midi_device::input_component_logic_t() ;
            logic.follow_up_funk = [=]( so_device::so_component::iinput_component_ptr_t cptr )
            {
                return false ;
            } ;

            logic.midi_message_funk = [=]( so_device::so_component::iinput_component_ptr_t cptr,
                so_device::midi_message_cref_t msg )
            {
                auto & c = *reinterpret_cast<so_device::so_component::rotary_knob_ptr_t>( cptr ) ;

                if( msg.compare_s_b1( 176, byte_t( i ) ) )
                {
                    c.intensity = float_t( msg.byte2 ) / 127.0f ;
                    return true ;
                }

                return false ;
            } ;

            mdev.add_component( "k_" + i_str, logic,
                so_device::so_component::rotary_knob_t::create( 
                "[so_device::akai::create_midimix] : Rotary Knob " + i_str ) ) ;
        }
    }

    // leds
    {
        typedef std::chrono::high_resolution_clock iclock_t ;

        so_std::vector< size_t > ids = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24 } ;
        for( auto i : ids )
        {
            so_std::string_t i_str = std::to_string( i ) ;

            auto logic = so_device::midi_device::output_component_logic_t() ;

            logic.handle_funk = [=]( so_device::so_component::ioutput_component_ptr_t cptr,
                so_device::midi_message_ref_t msg )
            {
                so_device::so_component::binary_led_ref_t led =
                    *reinterpret_cast<so_device::so_component::binary_led_ptr_t>( cptr ) ;

                so_device::so_component::led_state ls ;
                if( led.acquire_and_reset_changed_state( ls ) )
                {
                    if( ls == so_device::so_component::led_state::on )
                    {
                        led.tp = iclock_t::now() ;
                        led.value = true ;

                        msg = so_device::midi_message_t( 144, byte_t( i ), 127, 0 ) ;
                        return so_device::midi_output_result::transmit_and_queue ;
                    }
                    else if( ls == so_device::so_component::led_state::off )
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

                    size_t d_ms = size_t(std::chrono::duration_cast<std::chrono::milliseconds>( d ).count()) ;
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
                so_device::so_component::binary_led_t::create( 
                "[so_device::akai::create_midimix] : Binary Led " + i_str ) ) ;
        }
    }

    return so_device::midi_device_t::create( std::move(mdev), 
        "[so_device::akai::create_midimix] : Midi Mix Midi Controller Device" ) ;
}
