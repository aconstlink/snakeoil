//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "xinput_mappings.h"

#include <Xinput.h>
#include <algorithm>

using namespace so_device ;
using namespace so_device::so_win32 ;

//****************************************************************************************
xinput_mappings::xinput_mappings( void_t )
{
    {
        button_to_enum.reserve( 14 ) ;
        button_to_enum.push_back( xinput_button_to_enum{ XINPUT_GAMEPAD_BACK, xinput_inputs::button_back } ) ;
        button_to_enum.push_back( xinput_button_to_enum{ XINPUT_GAMEPAD_START, xinput_inputs::button_start } ) ;
        button_to_enum.push_back( xinput_button_to_enum{ XINPUT_GAMEPAD_A, xinput_inputs::button_a } ) ;
        button_to_enum.push_back( xinput_button_to_enum{ XINPUT_GAMEPAD_B, xinput_inputs::button_b } ) ;
        button_to_enum.push_back( xinput_button_to_enum{ XINPUT_GAMEPAD_X, xinput_inputs::button_x } ) ;
        button_to_enum.push_back( xinput_button_to_enum{ XINPUT_GAMEPAD_Y, xinput_inputs::button_y } ) ;
        button_to_enum.push_back( xinput_button_to_enum{ XINPUT_GAMEPAD_LEFT_THUMB, xinput_inputs::button_thumb_left } ) ;
        button_to_enum.push_back( xinput_button_to_enum{ XINPUT_GAMEPAD_RIGHT_THUMB, xinput_inputs::button_thumb_right } ) ;
        button_to_enum.push_back( xinput_button_to_enum{ XINPUT_GAMEPAD_LEFT_SHOULDER, xinput_inputs::button_shoulder_left } ) ;
        button_to_enum.push_back( xinput_button_to_enum{ XINPUT_GAMEPAD_RIGHT_SHOULDER, xinput_inputs::button_shoulder_right } ) ;
        button_to_enum.push_back( xinput_button_to_enum{ XINPUT_GAMEPAD_DPAD_UP, xinput_inputs::button_dpad_up } ) ;
        button_to_enum.push_back( xinput_button_to_enum{ XINPUT_GAMEPAD_DPAD_DOWN, xinput_inputs::button_dpad_down } ) ;
        button_to_enum.push_back( xinput_button_to_enum{ XINPUT_GAMEPAD_DPAD_LEFT, xinput_inputs::button_dpad_left } ) ;
        button_to_enum.push_back( xinput_button_to_enum{ XINPUT_GAMEPAD_DPAD_RIGHT, xinput_inputs::button_dpad_right } ) ;
    }

    {
        input_to_id.reserve( size_t(xinput_inputs::num_inputs) ) ;
        input_to_id.push_back( xinput_input_to_id{xinput_inputs::button_back, 0, "back"} ) ;
        input_to_id.push_back( xinput_input_to_id{xinput_inputs::button_start, 1, "start"} ) ;
        input_to_id.push_back( xinput_input_to_id{xinput_inputs::button_a, 2, "a"} ) ;
        input_to_id.push_back( xinput_input_to_id{xinput_inputs::button_b, 3, "b"} ) ;
        input_to_id.push_back( xinput_input_to_id{xinput_inputs::button_x, 4, "x"} ) ;
        input_to_id.push_back( xinput_input_to_id{xinput_inputs::button_y, 5, "y"} ) ;
        input_to_id.push_back( xinput_input_to_id{xinput_inputs::button_dpad_up, 6, "dpad_up"} ) ;
        input_to_id.push_back( xinput_input_to_id{xinput_inputs::button_dpad_down, 7, "dpad_down"} ) ;
        input_to_id.push_back( xinput_input_to_id{xinput_inputs::button_dpad_left, 8, "dpad_left"} ) ;
        input_to_id.push_back( xinput_input_to_id{xinput_inputs::button_dpad_right, 9, "dpad_right"} ) ;
        input_to_id.push_back( xinput_input_to_id{xinput_inputs::button_thumb_left, 10, "thumb_left"} ) ;
        input_to_id.push_back( xinput_input_to_id{xinput_inputs::button_thumb_right, 11, "thumb_right"} ) ;
        input_to_id.push_back( xinput_input_to_id{xinput_inputs::button_shoulder_left, 12, "shoulder_left"} ) ;
        input_to_id.push_back( xinput_input_to_id{xinput_inputs::button_shoulder_right, 13, "shoulder_right"} ) ;
        input_to_id.push_back( xinput_input_to_id{xinput_inputs::stick_left, 14, "stick_left"} ) ;
        input_to_id.push_back( xinput_input_to_id{xinput_inputs::stick_right, 15, "stick_right"} ) ;
        input_to_id.push_back( xinput_input_to_id{xinput_inputs::trigger_left, 16, "trigger_left"} ) ;
        input_to_id.push_back( xinput_input_to_id{xinput_inputs::trigger_right, 17, "trigger_right"} ) ;
    }

    {
        output_to_id.reserve( size_t( xinput_outputs::num_outputs ) ) ;
        output_to_id.push_back( xinput_output_to_id{ xinput_outputs::motor_left, 18, "motor_left" } ) ;
        output_to_id.push_back( xinput_output_to_id{ xinput_outputs::motor_right, 19, "motor_right" } ) ;        
    }
}

//****************************************************************************************
xinput_mappings::xinput_mappings( this_rref_t rhv ) 
{
    *this = std::move( rhv ) ;
}

//****************************************************************************************
xinput_mappings::xinput_mappings( this_cref_t rhv )
{
    *this = rhv ;
}

//****************************************************************************************
xinput_mappings::~xinput_mappings( void_t )
{}

//****************************************************************************************
xinput_mappings::this_ref_t xinput_mappings::operator = ( this_rref_t rhv )
{
    button_to_enum = std::move( rhv.button_to_enum ) ;
    input_to_id = std::move( rhv.input_to_id ) ;
    output_to_id = std::move( rhv.output_to_id ) ;

    return *this ;
}

//****************************************************************************************
xinput_mappings::this_ref_t xinput_mappings::operator = ( this_cref_t rhv )
{
    button_to_enum = rhv.button_to_enum ;
    input_to_id = rhv.input_to_id ;
    output_to_id = rhv.output_to_id ;
    return *this ;
}

//****************************************************************************************
bool_t xinput_mappings::enum_for_button_id( DWORD button_id, xinput_inputs & enum_out )
{
    auto iter = std::find_if( button_to_enum.begin(), button_to_enum.end(), 
        [&]( xinput_button_to_enum const & item )
    {
        return item.xinput_button == button_id ;
    } ) ;

    if( iter == button_to_enum.end() )
        return false ;

    enum_out = iter->button_enum ;

    return true ;
}

//****************************************************************************************
bool_t xinput_mappings::id_for_enum( xinput_inputs enum_in, size_t & id_out ) 
{
    auto iter = std::find_if( input_to_id.begin(), input_to_id.end(),
        [&]( xinput_input_to_id const & item )
    {
        return item.input_enum == enum_in ;
    } ) ;

    if( iter == input_to_id.end() )
        return false ;
    
    id_out = iter->id ;

    return true ;
}

//****************************************************************************************
bool_t xinput_mappings::id_name_for_enum( xinput_inputs enum_in, 
    size_t & id_out, so_std::string_ref_t name_out ) 
{
    auto iter = std::find_if( input_to_id.begin(), input_to_id.end(),
        [&]( xinput_input_to_id const & item )
    {
        return item.input_enum == enum_in ;
    } ) ;

    if( iter == input_to_id.end() )
        return false ;

    id_out = iter->id ;
    name_out = iter->name ;

    return true ;
}

//****************************************************************************************
bool_t xinput_mappings::id_for_enum( xinput_outputs enum_in, size_t & id_out ) 
{
    auto iter = std::find_if( output_to_id.begin(), output_to_id.end(),
        [&]( xinput_output_to_id const & item )
    {
        return item.output_enum == enum_in ;
    } ) ;

    if( iter == output_to_id.end() )
        return false ;

    id_out = iter->id ;

    return true ;
}

//****************************************************************************************
bool_t xinput_mappings::id_name_for_enum( xinput_outputs enum_in, 
    size_t & id_out, so_std::string_ref_t name_out ) 
{
    auto iter = std::find_if( output_to_id.begin(), output_to_id.end(),
        [&]( xinput_output_to_id const & item )
    {
        return item.output_enum == enum_in ;
    } ) ;

    if( iter == output_to_id.end() )
        return false ;

    id_out = iter->id ;
    name_out = iter->name ;

    return true ;
}

//****************************************************************************************
bool_t xinput_mappings::enum_for_id( size_t id_in, xinput_outputs & enum_out ) 
{
    auto iter = std::find_if( output_to_id.begin(), output_to_id.end(), 
        [&]( xinput_output_to_id const & inner )
    {
        return inner.id == id_in ;
    } ) ;
    
    if( iter == output_to_id.end() ) return false ;

    enum_out = iter->output_enum ;

    return true ;
}


//****************************************************************************************
bool_t xinput_mappings::deadzone_for_stick( xinput_inputs enum_in, uint16_t & value_out ) 
{
    if( enum_in == xinput_inputs::stick_left )
    {
        value_out = uint16_t(XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) ;
        return true ;
    }
    else if( enum_in == xinput_inputs::stick_right )
    {
        value_out = uint16_t( XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE ) ;
        return true ;
    }

    return false ;
}

//****************************************************************************************
void_t xinput_mappings::for_each_xinput_button( xbutton_to_enum_funk_t funk ) 
{
    for( auto & item : button_to_enum )
    {
        funk( item ) ;
    }
}

//****************************************************************************************
void_t xinput_mappings::for_each_button( input_to_id_funk_t funk ) 
{
    for( size_t i=0; i<14; ++i )
    {
        funk( input_to_id[i] ) ;
    }
}

//****************************************************************************************
void_t xinput_mappings::for_each_stick( input_to_id_funk_t funk )
{
    for( size_t i = 14; i < 16; ++i )
    {
        funk( input_to_id[i] ) ;
    }
}

//****************************************************************************************
void_t xinput_mappings::for_each_trigger( input_to_id_funk_t funk )
{
    for( size_t i = 16; i < 18; ++i )
    {
        funk( input_to_id[i] ) ;
    }
}

//****************************************************************************************
void_t xinput_mappings::for_each_motor( output_to_id_funk_t funk )
{
    for( size_t i = 0; i < 2; ++i )
    {
        funk( output_to_id[i] ) ;
    }
}

//****************************************************************************************
