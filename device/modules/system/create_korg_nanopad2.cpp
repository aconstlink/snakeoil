//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "system_module.h"

#include "../../devices/midi/midi_device.h"

#include "../../component/buttons/value_button.h"
#include "../../component/sliders/slider.h"
#include "../../component/touch/single_touch.h"

using namespace so_device ;

//*************************************************************************************
so_device::midi_device_ptr_t system_module::create_korg_nanopad2( void_t )
{
    so_device::midi_device::device_info di ;
    di.device_name = "nanoPAD2" ;
    so_device::midi_device mdev( di ) ;

    // binary buttons
    {
        // in the touch pad are actually binary buttons if 
        // "touch scale" is active.
        // it seems those buttons are in [48, 72] with 
        // press (144; id, 100, 0)
        // release (128; id, 64, 0)
    }

    // value buttons
    {
        so_std::vector< size_t > ids = {
            36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51
             } ;

        for( auto i : ids )
        {
            so_std::string_t i_str = std::to_string( i ) ;

            auto logic = so_device::midi_device::input_component_logic_t() ;
            logic.follow_up_funk = [=] ( so_device::so_component::iinput_component_ptr_t cptr )
            {
                auto & b = *reinterpret_cast< so_device::so_component::value_button_ptr_t >( cptr ) ;

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

            logic.midi_message_funk = [=] ( so_device::so_component::iinput_component_ptr_t cptr,
                so_device::midi_message_cref_t msg )
            {
                auto & b = *reinterpret_cast< so_device::so_component::value_button_ptr_t >( cptr ) ;
                
                byte_t const value = msg.byte2 ;

                // press
                if( msg.compare_s_b1( 144, byte_t( i ) ) )
                {
                    b.state = so_device::so_component::button_state::pressed ;
                    b.value = float_t(value) / 127.0f ;
                    return true ;
                }
                // release
                else if( msg == so_device::midi_message_t( 128, byte_t( i ), 64, 0 ) )
                {
                    b.state = so_device::so_component::button_state::released ;
                    b.value = 0 ;
                    return true ;
                }
                return false ;
            } ;

            mdev.add_component( "b_" + i_str, logic,
                so_device::so_component::value_button_t::create(
                    "[system_module::create_korg_nanopad2] : b_" + i_str ) ) ;
        }
    }

    // pad
    {
        auto logic = so_device::midi_device::input_component_logic_t() ;
        logic.follow_up_funk = [=] ( so_device::so_component::iinput_component_ptr_t cptr )
        {
            auto & b = *reinterpret_cast< so_device::so_component::single_touch_ptr_t >( cptr ) ;

            if( b.state == so_device::so_component::touch_state::touched)
            {
                b.state = so_device::so_component::touch_state::touching ;
                return true ;
            }
            else if( b.state == so_device::so_component::touch_state::touching )
            {
                return true ;
            }
            else if( b.state == so_device::so_component::touch_state::released )
            {
                b.state = so_device::so_component::touch_state::none ;
            }
            return false ;
        } ;

        logic.midi_message_funk = [=] ( so_device::so_component::iinput_component_ptr_t cptr,
            so_device::midi_message_cref_t msg )
        {
            auto & b = *reinterpret_cast< so_device::so_component::single_touch_ptr_t >( cptr ) ;

            byte_t const value = msg.byte2 ;

            // x direction
            if( msg.compare_s_b1( 176, 1 ) )
            {
                if( b.state != so_device::so_component::touch_state::touching )
                    b.state = so_device::so_component::touch_state::touched ;

                b.npc.x() = float_t(value) / 127.0f ;
                b.ncc.x() = b.npc.x() * 2.0f - 1.0f ;
                return true ;
            }
            else if( msg.compare_s_b1( 176, 2 ) )
            {
                if( b.state != so_device::so_component::touch_state::touching )
                    b.state = so_device::so_component::touch_state::touched ;

                b.npc.y() = float_t( value ) / 127.0f ;
                b.ncc.y() = b.npc.y() * 2.0f - 1.0f ;
                return true ;
            }

            // release
            else if( msg == so_device::midi_message_t( 176, 16, 0, 0 ) )
            {
                b.state = so_device::so_component::touch_state::released ;
                b.npc = so_math::vec2f_t() ;
                b.ncc = so_math::vec2f_t() ;
                return true ;
            }
            return false ;
        } ;

        mdev.add_component( "touch", logic,
            so_device::so_component::single_touch_t::create(
                "[system_module::create_korg_nanopad2] : touch" ) ) ;
        
    }

    return so_device::midi_device_t::create( std::move( mdev ),
        "[system_module::create_korg_nanopad2] : KORG nanoPAD2" ) ;
}