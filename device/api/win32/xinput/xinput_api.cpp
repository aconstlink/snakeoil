//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "xinput_api.h"

#include "../../gamepad_notify.h"

#include "../../../devices/gamepad/gamepad_device.h"
#include "../../../component/buttons/binary_button.h"
#include "../../../component/buttons/value_button.h"
#include "../../../component/sticks/value_stick.h"
#include "../../../component/motors/value_motor.h"

#include "../../../modules/igamepad_module.h"

#include "../../../devices/notification/funk_notify.h"
#include "../../../vdev/gamepad/xbox/xbox_360.h"
// more vdevs here

#include <windows.h>
#include <Xinput.h>

#include <snakeoil/log/global.h>

#include <snakeoil/core/cast.hpp>

using namespace so_device ;
using namespace so_device::so_win32 ;

//****************************************************************************************
xinput_api::xinput_api( void_t ) 
{
#if defined( SNAKEOIL_OS_WIN8 )
    XInputEnable( true ) ;
#endif

    this_t::init_gamepads() ;
}

//****************************************************************************************
xinput_api::xinput_api( this_rref_t rhv )
{
    _devices = std::move( rhv._devices ) ;
    _xinput_mappings = std::move( rhv._xinput_mappings ) ;
    _notifies = std::move( rhv._notifies ) ;
}

//****************************************************************************************
xinput_api::~xinput_api( void_t )
{
#if defined( SNAKEOIL_OS_WIN8 )
    XInputEnable( false ) ;
#endif

    for( auto item : _devices )
    {
        so_device::gamepad_device_t::destroy(item.device_ptr) ;
        so_device::so_win32::xinput_device_t::destroy( item.xinput_ptr) ;
    }

    for( auto * nptr : _notifies )
    {
        nptr->destroy() ;
    }
}

//****************************************************************************************
xinput_api::this_ptr_t xinput_api::create( so_memory::purpose_cref_t p )
{
    return so_device::memory::alloc( this_t(), p ) ;
}

//****************************************************************************************
void_t xinput_api::destroy( this_ptr_t ptr )
{
    so_device::memory::dealloc( ptr ) ;
}

//****************************************************************************************
void_t xinput_api::create_devices( so_device::igamepad_module_ptr_t )
{
}

//****************************************************************************************
so_device::result xinput_api::register_device( so_device::key_cref_t,
    so_device::gamepad_device_ptr_t )
{
    return so_device::failed ;
}

//****************************************************************************************
so_device::result xinput_api::unregister_device( so_device::key_cref_t )
{
    return so_device::failed ;
}

//****************************************************************************************
void_t xinput_api::update_gamepad( void_t )
{
    // reset all virtual devices
    // @todo improve this!
    for( auto & item : _devices )
    {
        for( auto & iitem : item.vdevs )
        {
            iitem->reset() ;
        }
    }

    for( auto & item : _devices )
    {
        so_device::gamepad_device_ref_t gamepad = *item.device_ptr ;
        xinput_device_ref_t hdev = *item.xinput_ptr ;

        XINPUT_STATE state ;
        DWORD res = XInputGetState( item.xinput_ptr->get_id(), &state ) ;
        if( so_log::global::warning(res != ERROR_SUCCESS, 
            "[so_device::so_win32::xinput_module::update] : XInputGetState" ) ) 
            continue ;

        // handle buttons
        _xinput_mappings.for_each_xinput_button( [&]( xinput_button_to_enum const & mapping )
        {
            this_t::handle_button( state, mapping.xinput_button, hdev, gamepad ) ;
        } ) ;
                
        // triggers
        {
            this_t::handle_trigger( this_t::which_trigger::left, state, hdev, gamepad ) ;
            this_t::handle_trigger( this_t::which_trigger::right, state, hdev, gamepad ) ;
        }

        // sticks
        {
            this_t::handle_stick( this_t::which_stick::left, state, hdev, gamepad ) ;
            this_t::handle_stick( this_t::which_stick::right, state, hdev, gamepad ) ;
        }

        so_device::gamepad_device::gamepad_messages_t out_msgs ;
        {
            item.device_ptr->update( out_msgs ) ;
        }

        // motors
        {
            XINPUT_VIBRATION vib ;
            memset( &vib, 0, sizeof(XINPUT_VIBRATION) ) ;

            bool_t any_change = false ;

            for( auto & msg : out_msgs )
            {
                xinput_outputs output_comp_enum ;
                if( so_core::is_not( _xinput_mappings.enum_for_id( msg.id, output_comp_enum ) ) )
                {
                    so_log::global::error("[so_device::so_win32::xinput_module::update] : \
                                        output component enum for id not found. Check output message.") ;
                    continue ;
                }

                // check motors
                {
                    if( output_comp_enum == xinput_outputs::motor_left )
                    {
                        vib.wLeftMotorSpeed = msg.info != 0 ? msg.value : 0 ;
                        any_change = true ;
                    }
                    else if( output_comp_enum == xinput_outputs::motor_right )
                    {
                        vib.wRightMotorSpeed = msg.info != 0 ? msg.value : 0 ;                        
                        any_change = true ;
                    }
                }
            }

            if( any_change  ) XInputSetState( item.xinput_ptr->get_id(), &vib ) ;
        }

        //
        // must be done after the new state is consumed.
        // i.e. the gamepad received all new states
        //
        {
            hdev.exchange_state( state ) ;
        }        
    }
}

//****************************************************************************************
void_t xinput_api::destroy( void_t )
{
    this_t::destroy( this ) ;
}

//****************************************************************************************
so_device::gamepad_device_ptr_t xinput_api::find_any_device( void_t ) 
{
    if( _devices.size() == 0 )
        return nullptr ;

    return _devices[0].device_ptr ;
}

//****************************************************************************************
so_device::so_vgamepad::xbox_360_ptr_t xinput_api::find_device( size_t const id )
{
    if( _devices.size() <= id )
        return nullptr ;

    return dynamic_cast<so_device::so_vgamepad::xbox_360_ptr_t>( _devices[ id ].vdevs[0] ) ;
}

//****************************************************************************************
bool_t xinput_api::register_for_any_device( so_device::so_vdev::ivdev_ptr_t ivdev_ptr )
{
    return this_t::register_device( 0, ivdev_ptr ) ;
}

//****************************************************************************************
bool_t xinput_api::register_device( size_t const dev_id, so_device::so_vdev::ivdev_ptr_t ivdev_ptr )
{
    if( so_core::is_nullptr( ivdev_ptr ) )
        return false ;

    if( so_core::is_not( so_core::can_cast< so_device::so_vgamepad::xbox_360_ptr_t >( ivdev_ptr ) ) )
        return false ;

    
    if( _devices.size() <= dev_id )
        return false ;

    so_device::so_vgamepad::xbox_360_ptr_t vptr = static_cast<
        so_device::so_vgamepad::xbox_360_ptr_t >( ivdev_ptr ) ;


    // handle virtual device

    // 1. register notification

    so_device::funk_notify_ptr_t gamepad_notify = so_device::funk_notify_t::create(
        [=]( so_device::idevice_ptr_t, so_std::string_cref_t name,
            so_device::so_input::iinput_component_ptr_t comp_ptr )
    {

        // sticks
        if( so_core::is_not_nullptr(
            dynamic_cast< so_device::so_input::value_stick_ptr_t >( comp_ptr ) ) )
        {
            auto * stick_ptr = reinterpret_cast<
                so_device::so_input::value_stick_ptr_t >( comp_ptr ) ;

            so_device::so_vgamepad::xbox_stick_state xbbs =
                so_device::so_vgamepad::xbox_stick_state::invalid ;

            if( stick_ptr->state_is( so_device::stick_state::tilted ) )
            {
                xbbs = so_device::so_vgamepad::xbox_stick_state::tilt ;
            }
            else if( stick_ptr->state_is( so_device::stick_state::tilting ) )
            {
                xbbs = so_device::so_vgamepad::xbox_stick_state::tilting ;
            }
            else if( stick_ptr->state_is( so_device::stick_state::untilted ) )
            {
                xbbs = so_device::so_vgamepad::xbox_stick_state::untilt ;
            }

            if( name == "stick_right" )
            {
                vptr->set_stick_state( so_device::so_vgamepad::xbox_stick::right, xbbs,
                    stick_ptr->intensity() ) ;
            }
            else if( name == "stick_left" )
            {
                vptr->set_stick_state( so_device::so_vgamepad::xbox_stick::left, xbbs,
                    stick_ptr->intensity() ) ;
            }
        }

        // value buttons
        if( so_core::is_not_nullptr(
            dynamic_cast< so_device::so_input::value_button_ptr_t >( comp_ptr ) ) )
        {
            auto * b_ptr = reinterpret_cast<
                so_device::so_input::value_button_ptr_t >( comp_ptr ) ;

            so_device::so_vgamepad::xbox_button_state xbbs =
                so_device::so_vgamepad::xbox_button_state::invalid ;

            if( b_ptr->state_is( so_device::button_state::pressed ) )
            {
                xbbs = so_device::so_vgamepad::xbox_button_state::pressed ;
            }
            else if( b_ptr->state_is( so_device::button_state::pressing ) )
            {
                xbbs = so_device::so_vgamepad::xbox_button_state::pressing ;
            }
            else if( b_ptr->state_is( so_device::button_state::released ) )
            {
                xbbs = so_device::so_vgamepad::xbox_button_state::released ;
            }

            if( name == "trigger_left" )
            {
                vptr->set_trigger_state( so_device::so_vgamepad::xbox_trigger::left, xbbs,
                    b_ptr->intensity() ) ;
            }
            else if( name == "trigger_right" )
            {
                vptr->set_trigger_state( so_device::so_vgamepad::xbox_trigger::right, xbbs,
                    b_ptr->intensity() ) ;
            }
        }

        // binary buttons
        if( so_core::is_not_nullptr(
            dynamic_cast< so_device::so_input::binary_button_ptr_t >( comp_ptr ) ) )
        {
            auto * b_ptr = reinterpret_cast<
                so_device::so_input::binary_button_ptr_t >( comp_ptr ) ;

            so_device::so_vgamepad::xbox_button_state xbbs =
                so_device::so_vgamepad::xbox_button_state::invalid ;

            if( b_ptr->state_is( so_device::button_state::pressed ) )
            {
                xbbs = so_device::so_vgamepad::xbox_button_state::pressed ;
            }
            else if( b_ptr->state_is( so_device::button_state::pressing ) )
            {
                xbbs = so_device::so_vgamepad::xbox_button_state::pressing ;
            }
            else if( b_ptr->state_is( so_device::button_state::released ) )
            {
                xbbs = so_device::so_vgamepad::xbox_button_state::released ;
            }

            if( name == "shoulder_left" )
            {
                vptr->set_shoulder_state( so_device::so_vgamepad::xbox_shoulder::left, xbbs ) ;
            }
            else if( name == "shoulder_right" )
            {
                vptr->set_shoulder_state( so_device::so_vgamepad::xbox_shoulder::right, xbbs ) ;
            }
            else if( name == "a" )
            {
                vptr->set_button_state( so_device::so_vgamepad::xbox_button::a, xbbs ) ;
            }
            else if( name == "b" )
            {
                vptr->set_button_state( so_device::so_vgamepad::xbox_button::b, xbbs ) ;
            }
            else if( name == "x" )
            {
                vptr->set_button_state( so_device::so_vgamepad::xbox_button::x, xbbs ) ;
            }
            else if( name == "y" )
            {
                vptr->set_button_state( so_device::so_vgamepad::xbox_button::y, xbbs ) ;
            }
            else if( name == "start" )
            {
                vptr->set_button_state( so_device::so_vgamepad::xbox_button::start, xbbs ) ;
            }
            else if( name == "back" )
            {
                vptr->set_button_state( so_device::so_vgamepad::xbox_button::back, xbbs ) ;
            }
            else if( name == "dpad_left" )
            {
                vptr->set_dpad_state( so_device::so_vgamepad::xbox_dpad::left, xbbs ) ;
            }
            else if( name == "dpad_right" )
            {
                vptr->set_dpad_state( so_device::so_vgamepad::xbox_dpad::right, xbbs ) ;
            }
            else if( name == "dpad_up" )
            {
                vptr->set_dpad_state( so_device::so_vgamepad::xbox_dpad::up, xbbs ) ;
            }
            else if( name == "dpad_down" )
            {
                vptr->set_dpad_state( so_device::so_vgamepad::xbox_dpad::down, xbbs ) ;
            }
            else if( name == "thumb_left" )
            {
                vptr->set_stick_state( so_device::so_vgamepad::xbox_stick::left, xbbs ) ;
            }
            else if( name == "thumb_right" )
            {
                vptr->set_stick_state( so_device::so_vgamepad::xbox_stick::right, xbbs ) ;
            }
        }

    }, "[] : gamepad funk" ) ;

    _devices[ dev_id ].device_ptr->register_notification( gamepad_notify ) ;

    // 2. register virtual device
    // add virtual device
    _devices[ dev_id ].vdevs.push_back( ivdev_ptr ) ;    

    return true ;
}

//****************************************************************************************
bool_t xinput_api::unregister_virtual_device( so_device::so_vdev::ivdev_ptr_t ptr )
{
    for( gamepad_data & d : _devices )
    {
        // 1. find device
        auto iter = std::find( d.vdevs.begin(), d.vdevs.end(), ptr ) ;
        if( iter == d.vdevs.end() ) continue ;

        // 2. remove device from list
        d.vdevs.erase( iter ) ;
    }
    
    return false ;
}

//****************************************************************************************
void_t xinput_api::install_gamepad_notify( so_device::igamepad_notify_ptr_t ptr )
{
    auto iter = std::find( _notifies.begin(), _notifies.end(), ptr ) ;
    if( iter != _notifies.end() ) return ;

    _notifies.push_back( ptr ) ;

    so_log::global_t::warning("[xinput_api::install_gamepad_notify] : will currently have no effect") ;
}

//****************************************************************************************
void_t xinput_api::init_gamepads( void_t ) 
{
#if defined( SNAKEOIL_OS_WIN8 )
    XInputEnable( true ) ;
#endif

    // by XInput 1.4 spec. only 4 devices supported.
    for( DWORD i = 0; i < 4; ++i )
    {
        {
            XINPUT_CAPABILITIES caps ;
            DWORD res = XInputGetCapabilities( i, XINPUT_FLAG_GAMEPAD, &caps ) ;

            if( res == ERROR_DEVICE_NOT_CONNECTED ) continue ;

            if( so_log::global::error( res != ERROR_SUCCESS,
                "[xinput_module::init_gamepads] : Error query controller caps." ) )
                continue ;

            if( so_log::global::error( caps.Type != XINPUT_DEVTYPE_GAMEPAD,
                "[xinput_module::init_gamepads] : xinput device is not a gamepad." ) )
                continue ;

            so_log::global::status( "[xinput_module::init_gamepads] : Device " + 
                std::to_string(i) + ": Gamepad found" ) ;
        }

        XINPUT_STATE state ;
        DWORD res = XInputGetState( i, &state ) ;
        if( res != ERROR_SUCCESS ) continue ;

        gamepad_data_t gd ;
        gd.xinput_ptr = xinput_device::create( xinput_device(i), 
            "[xinput_module::init_gamepads] : xinput_device" ) ;

        gd.device_ptr = this_t::create_gamepad() ;

        gd.xinput_ptr->exchange_state( state ) ;

        _devices.push_back( gd ) ;

        {
            auto * x360 = so_device::memory::alloc( so_device::so_vgamepad::xbox_360_t(),
                "[xinput_api::init_gamepads] : xbox360 controller" ) ;

            auto const ires = this_t::register_device( i, x360 ) ;
            so_log::global::error( so_core::is_not( ires ), "[xinput_api::init_gamepads] : "
                "Could not create xbox360 controller" ) ;
        }

    }
}

//****************************************************************************************
so_device::gamepad_device_ptr_t xinput_api::create_gamepad( void_t ) 
{
    auto * gamepad_ptr =  so_device::gamepad_device_t::create(
        "[xinput_module::create_gamepad] : gamepad_device" ) ;

    // add components

    // button
    _xinput_mappings.for_each_button( [&]( xinput_input_to_id const & mapping )
    {
        uint8_t const i = uint8_t(mapping.id) ;

        so_std::string_t i_str = std::to_string( i ) ;

        auto logic = so_device::gamepad_device::input_component_logic_t() ;
        logic.follow_up_funk = [=]( so_device::so_input::iinput_component_ptr_t cptr )
        {
            auto & b = *reinterpret_cast<so_device::so_input::binary_button_ptr_t>( cptr ) ;
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

        logic.handle_funk = [=]( so_device::so_input::iinput_component_ptr_t cptr,
            so_device::gamepad_message_cref_t msg )
        {
            auto & b = *reinterpret_cast<so_device::so_input::binary_button_ptr_t>( cptr ) ;

            // press
            if( msg == so_device::gamepad_message_t( uint8_t( i ), uint8_t( 1 ), 0 ) )
            {
                b.state = so_device::button_state::pressed ;
                return true ;
            }
            // release
            else if( msg == so_device::gamepad_message_t( uint8_t( i ), uint8_t( 0 ), 0 ) )
            {
                b.state = so_device::button_state::released ;
                return true ;
            }
            return false ;
        } ;

        gamepad_ptr->add_component( mapping.name, logic,
            so_device::so_input::binary_button_t::create(
            "[so_device::xinput_module::create_gamepad] : b_" + i_str ) ) ;
    } ) ;

    // trigger
    _xinput_mappings.for_each_trigger( [&]( xinput_input_to_id const & mapping )
    {
        uint8_t const i = uint8_t(mapping.id) ;

        auto logic = so_device::gamepad_device::input_component_logic_t() ;
        logic.follow_up_funk = [=]( so_device::so_input::iinput_component_ptr_t cptr )
        {
            auto & comp = *reinterpret_cast<so_device::so_input::value_button_ptr_t>( cptr ) ;
            if( comp.state == so_device::button_state::released )
            {
                comp.state = so_device::button_state::none ;
            }
            else if( comp.state == so_device::button_state::pressed )
            {
                comp.state = so_device::button_state::pressing ;
            }

            return false ;
        } ;

        logic.handle_funk = [=]( so_device::so_input::iinput_component_ptr_t cptr,
            so_device::gamepad_message_cref_t msg )
        {
            auto & comp = *reinterpret_cast<so_device::so_input::value_button_ptr_t>( cptr ) ;

            if( msg.compare_id_info(so_device::gamepad_message_t( uint8_t( i ), uint8_t( 0 ), 0 )) )
            {
                if( msg.value == 0 )
                {
                    comp.state = so_device::button_state::released ;
                }
                else if( comp.state != so_device::button_state::pressing )
                {
                    comp.state = so_device::button_state::pressed ;
                }
                
                comp.value = float_t( msg.value ) / 255.0f ;
                return true ;
            }
            return false ;
        } ;

        gamepad_ptr->add_component( mapping.name, logic,
            so_device::so_input::value_button_t::create(
            "[so_device::xinput_module::create_gamepad] : value_button : "+ mapping.name) ) ;
    } ) ;

    // sticks
    _xinput_mappings.for_each_stick( [&]( xinput_input_to_id const & mapping )
    {
        uint8_t const id = uint8_t(mapping.id) ;

        auto logic = so_device::gamepad_device::input_component_logic_t() ;
        logic.follow_up_funk = [=]( so_device::so_input::iinput_component_ptr_t cptr )
        {
            auto & comp = *reinterpret_cast<so_device::so_input::value_stick_ptr_t>( cptr ) ;

            if( comp.state_x == so_device::stick_state::untilted )
            {
                comp.state_x = so_device::stick_state::none ;
            }
            else if( comp.state_x == so_device::stick_state::tilted )
            {
                comp.state_x = so_device::stick_state::tilting ;
            }

            if( comp.state_y == so_device::stick_state::untilted )
            {
                comp.state_y = so_device::stick_state::none ;
            }
            else if( comp.state_y == so_device::stick_state::tilted )
            {
                comp.state_y = so_device::stick_state::tilting ;
            }

            return false ;
        } ;

        logic.handle_funk = [=]( so_device::so_input::iinput_component_ptr_t cptr,
            so_device::gamepad_message_cref_t msg )
        {
            auto & comp = *reinterpret_cast<so_device::so_input::value_stick_ptr_t>( cptr ) ;

            //  x-axis
            if( msg.compare_id_info( so_device::gamepad_message_t(id, 0, 0 ) ) )
            {
                if( msg.value == 0 )
                {
                    comp.state_x = so_device::stick_state::untilted ;
                }                
                else if( comp.state_x != so_device::stick_state::tilting )
                {
                    comp.state_x = so_device::stick_state::tilted ;
                }
                
                comp.value.x() = msg.value ;
                return true ;
            }
            // y-axis
            else if( msg.compare_id_info( so_device::gamepad_message_t(id, 1, 0 ) ) )
            {
                if( msg.value == 0 )
                {
                    comp.state_y = so_device::stick_state::untilted ;
                }                
                else if( comp.state_y != so_device::stick_state::tilting )
                {
                    comp.state_y = so_device::stick_state::tilted ;
                }

                comp.value.y() = msg.value ;
                return true ;
            }
            return false ;
        } ;
        
        uint16_t deadzone = 0 ;
        _xinput_mappings.deadzone_for_stick(mapping.input_enum, deadzone ) ;

        so_device::so_input::value_stick vs ;
        vs.max_value = so_math::vec2i16_t( 
            int16_t((1<<15)-1)-int16_t(deadzone) ) ;

        gamepad_ptr->add_component( mapping.name, logic,
            so_device::so_input::value_stick::create( std::move(vs),
            "[so_device::xinput_module::create_gamepad] : stick : " + mapping.name ) ) ;
    } ) ;

    // motors
    _xinput_mappings.for_each_motor( [&]( xinput_output_to_id const & mapping )
    {
        uint8_t const id = uint8_t( mapping.id ) ;

        auto logic = so_device::gamepad_device::output_component_logic_t() ;
        
        logic.handle_funk = [=]( so_device::so_output::ioutput_component_ptr_t cptr,
            so_device::gamepad_message_ref_t msg )
        {
            so_device::so_output::value_motor_ref_t comp = 
                *reinterpret_cast<so_device::so_output::value_motor_ptr_t>( cptr ) ;

            so_device::motor_state ms ;
            if( comp.acquire_and_reset_changed_state(ms) )
            {
                if( ms == so_device::motor_state::on )
                {
                    msg = so_device::gamepad_message_t( id, 1, comp.value ) ;
                    return so_device::gamepad_output_result::transmit_and_queue ;
                }
                else if( ms == so_device::motor_state::off )
                {
                    msg = so_device::gamepad_message_t( id, 0, 0 ) ;
                    return so_device::gamepad_output_result::transmit_and_dismiss ;
                }
            }
            // followup
            else if( comp.is_on() )
            {
                msg = so_device::gamepad_message_t( id, 1, comp.value ) ;
                return so_device::gamepad_output_result::transmit_and_queue ;
            }
            
            return so_device::gamepad_output_result::dismiss ;
        } ;

        gamepad_ptr->add_component( mapping.name, logic,
            so_device::so_output::value_motor::create( 
            "[so_device::xinput_module::create_gamepad] : stick : " + mapping.name ) ) ;
    } ) ;

    return gamepad_ptr ;
}

//****************************************************************************************
bool_t xinput_api::handle_button( XINPUT_STATE const & state, DWORD const button_flag,
    so_device::so_win32::xinput_device_cref_t xdev,
    so_device::gamepad_device_ref_t gamepad )
{
    so_device::gamepad_message_t msg ;

    if( xdev.is_pressed( state, button_flag ) )
    {        
        xinput_inputs enum_input ;
        if( _xinput_mappings.enum_for_button_id( button_flag, enum_input ) )
        {
            size_t id ;
            _xinput_mappings.id_for_enum( enum_input, id ) ;
            msg.id = uint8_t(id) ;
        }
       
        msg.info = 1 ;
        msg.value = 0 ;

        gamepad.receive_message( msg ) ;

        return true ;
    }
    else if( xdev.is_released( state, button_flag ) )
    {
        xinput_inputs enum_input ;
        if( _xinput_mappings.enum_for_button_id( button_flag, enum_input ) )
        {
            size_t id ;
            _xinput_mappings.id_for_enum( enum_input, id ) ;
            msg.id = uint8_t( id ) ;
        }

        msg.info = 0 ;
        msg.value = 0 ;

        gamepad.receive_message( msg ) ; 

        return true ;
    }
    return false ;
}

//****************************************************************************************
bool_t xinput_api::handle_stick( this_t::which_stick ws, XINPUT_STATE const & state,
    so_device::so_win32::xinput_device_cref_t xdev,
    so_device::gamepad_device_ref_t gamepad ) 
{
    so_device::gamepad_message_t msg_x ;
    so_device::gamepad_message_t msg_y ;

    if( ws == this_t::which_stick::left )
    {
        so_math::vector2< int16_t > change ;
        auto ss = xdev.check_left_stick( state, change ) ;
        if( ss == so_device::stick_state::none ) 
            return false ;

        size_t id ;
        _xinput_mappings.id_for_enum( xinput_inputs::stick_left, id ) ;

        msg_x.id = uint8_t( id ) ;
        msg_y.id = uint8_t( id ) ;
        msg_x.info = 0 ;
        msg_y.info = 1 ;
        
        msg_x.value = change.x() ;
        msg_y.value = change.y() ;

        gamepad.receive_message( msg_x ) ;
        gamepad.receive_message( msg_y ) ;

        return true ;
    }
    else if( ws == this_t::which_stick::right )
    {
        so_math::vector2< int16_t > change ;
        auto ss = xdev.check_right_stick( state, change ) ;
        if( ss == so_device::stick_state::none )
            return false ;

        size_t id ;
        _xinput_mappings.id_for_enum( xinput_inputs::stick_right, id ) ;

        msg_x.id = uint8_t( id ) ;
        msg_y.id = uint8_t( id ) ;
        msg_x.info = 0 ;
        msg_y.info = 1 ;

        msg_x.value = change.x() ;
        msg_y.value = change.y() ;

        gamepad.receive_message( msg_x ) ;
        gamepad.receive_message( msg_y ) ;

        return true ;
    }
    
    return false ;
}

//****************************************************************************************
bool_t xinput_api::handle_trigger( this_t::which_trigger wt, XINPUT_STATE const & state,
    so_device::so_win32::xinput_device_cref_t xdev,
    so_device::gamepad_device_ref_t gamepad ) 
{
    so_device::gamepad_message_t msg ;

    if( wt == this_t::which_trigger::left )
    {
        uint16_t change ;
        auto ss = xdev.check_left_trigger( state, change ) ;
        if( ss == so_device::button_state::none )
            return false ;

        size_t id ;
        _xinput_mappings.id_for_enum( xinput_inputs::trigger_left, id ) ;

        msg.id = uint8_t( id ) ;       
        msg.info = 0 ;
        msg.value = change ;

        gamepad.receive_message( msg ) ;
        

        return true ;
    }
    else if( wt == this_t::which_trigger::right )
    {
        uint16_t change ;
        auto ss = xdev.check_right_trigger( state, change ) ;
        if( ss == so_device::button_state::none )
            return false ;

        size_t id ;
        _xinput_mappings.id_for_enum( xinput_inputs::trigger_right, id ) ;

        msg.id = uint8_t( id ) ;
        msg.info = 0 ;
        msg.value = change ;

        gamepad.receive_message( msg ) ;


        return true ;
    }

    return false ;
}
