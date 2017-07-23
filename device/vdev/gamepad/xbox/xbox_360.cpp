//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "xbox_360.h"

using namespace so_device ;
using namespace so_device::so_vgamepad ;

//****************************************************************************************
xbox_360::this_ptr_t xbox_360::create( this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_device::memory::alloc( std::move( rhv ), p ) ;
}

//****************************************************************************************
void_t xbox_360::destroy( this_ptr_t ptr )
{
    so_device::memory::dealloc( ptr ) ;
}

//****************************************************************************************
bool_t xbox_360::is_pressed( so_device::so_vgamepad::xbox_button b ) const
{
    auto const iter = _button_states.find( b ) ;
    if( iter == _button_states.end() )
        return false ;

    return iter->second == so_device::so_vgamepad::xbox_button_state::pressed ;
}

//****************************************************************************************
bool_t xbox_360::is_pressing( so_device::so_vgamepad::xbox_button b ) const
{
    auto const iter = _button_states.find( b ) ;
    if( iter == _button_states.end() )
        return false ;

    return iter->second == so_device::so_vgamepad::xbox_button_state::pressing ;
}

//****************************************************************************************
bool_t xbox_360::is_released( so_device::so_vgamepad::xbox_button b ) const
{
    auto const iter = _button_states.find( b ) ;
    if( iter == _button_states.end() )
        return false ;

    return iter->second == so_device::so_vgamepad::xbox_button_state::released ;
}

//****************************************************************************************
bool_t xbox_360::is_pressed( so_device::so_vgamepad::xbox_shoulder b ) const
{
    auto const iter = _shoulder_states.find( b ) ;
    if( iter == _shoulder_states.end() )
        return false ;

    return iter->second == so_device::so_vgamepad::xbox_button_state::pressed ;
}

//****************************************************************************************
bool_t xbox_360::is_pressing( so_device::so_vgamepad::xbox_shoulder b ) const
{
    auto const iter = _shoulder_states.find( b ) ;
    if( iter == _shoulder_states.end() )
        return false ;

    return iter->second == so_device::so_vgamepad::xbox_button_state::pressing ;
}

//****************************************************************************************
bool_t xbox_360::is_released( so_device::so_vgamepad::xbox_shoulder b ) const
{
    auto const iter = _shoulder_states.find( b ) ;
    if( iter == _shoulder_states.end() )
        return false ;

    return iter->second == so_device::so_vgamepad::xbox_button_state::released ;
}

//****************************************************************************************
bool_t xbox_360::is_pressed( so_device::so_vgamepad::xbox_dpad b ) const
{
    auto const iter = _dpad_states.find( b ) ;
    if( iter == _dpad_states.end() )
        return false ;

    return iter->second == so_device::so_vgamepad::xbox_button_state::pressed ;
}

//****************************************************************************************
bool_t xbox_360::is_pressing( so_device::so_vgamepad::xbox_dpad b ) const
{
    auto const iter = _dpad_states.find( b ) ;
    if( iter == _dpad_states.end() )
        return false ;

    return iter->second == so_device::so_vgamepad::xbox_button_state::pressing ;
}

//****************************************************************************************
bool_t xbox_360::is_released( so_device::so_vgamepad::xbox_dpad b ) const
{
    auto const iter = _dpad_states.find( b ) ;
    if( iter == _dpad_states.end() )
        return false ;

    return iter->second == so_device::so_vgamepad::xbox_button_state::released ;
}

//****************************************************************************************
bool_t xbox_360::any_trigger_changed( void_t ) const
{
    {
        auto const iter = _trigger_states.find( so_device::so_vgamepad::xbox_trigger::left ) ;
        if( iter != _trigger_states.end() )
            return true ;
    }

    {
        auto const iter = _trigger_states.find( so_device::so_vgamepad::xbox_trigger::right ) ;
        if( iter != _trigger_states.end() )
            return true ;
    }

    return false ;
}

//****************************************************************************************
bool_t xbox_360::is_pressed( so_device::so_vgamepad::xbox_trigger b, float_out_t v ) const
{
    auto const iter = _trigger_states.find( b ) ;
    if( iter == _trigger_states.end() )
        return false ;

    v = iter->second.value ;
    return iter->second.s == so_device::so_vgamepad::xbox_button_state::pressed ;
}

//****************************************************************************************
bool_t xbox_360::is_pressing( so_device::so_vgamepad::xbox_trigger b, float_out_t v ) const
{
    auto const iter = _trigger_states.find( b ) ;
    if( iter == _trigger_states.end() )
        return false ;

    v = iter->second.value ;
    return iter->second.s == so_device::so_vgamepad::xbox_button_state::pressing ;
}

//****************************************************************************************
bool_t xbox_360::is_released( so_device::so_vgamepad::xbox_trigger b ) const
{
    auto const iter = _trigger_states.find( b ) ;
    if( iter == _trigger_states.end() )
        return false ;

    return iter->second.s == so_device::so_vgamepad::xbox_button_state::released ;
}

//****************************************************************************************
bool_t xbox_360::any_tilt_changed( void_t ) const
{
    {
        auto const iter = _stick_states.find( so_device::so_vgamepad::xbox_stick::left ) ;
        if( iter != _stick_states.end() )
            return true ;
    }

    {
        auto const iter = _stick_states.find( so_device::so_vgamepad::xbox_stick::right ) ;
        if( iter != _stick_states.end() )
            return true ;
    }

    return false ;
}

//****************************************************************************************
bool_t xbox_360::is_tilt( so_device::so_vgamepad::xbox_stick b, so_math::vec2f_out_t v ) const
{
    auto const iter = _stick_states.find( b ) ;
    if( iter == _stick_states.end() )
        return false ;

    v = iter->second.value ;
    return iter->second.s == so_device::so_vgamepad::xbox_stick_state::tilt ;
}

//****************************************************************************************
bool_t xbox_360::is_tilting( so_device::so_vgamepad::xbox_stick b, so_math::vec2f_out_t v) const
{
    auto const iter = _stick_states.find( b ) ;
    if( iter == _stick_states.end() )
        return false ;

    v = iter->second.value ;
    return iter->second.s == so_device::so_vgamepad::xbox_stick_state::tilting ;
}

//****************************************************************************************
bool_t xbox_360::is_untilt( so_device::so_vgamepad::xbox_stick b ) const
{
    auto const iter = _stick_states.find( b ) ;
    if( iter == _stick_states.end() )
        return false ;

    return iter->second.s == so_device::so_vgamepad::xbox_stick_state::untilt ;
}

//****************************************************************************************
bool_t xbox_360::is_pressed( so_device::so_vgamepad::xbox_stick b ) const
{
    auto const iter = _stick_states.find( b ) ;
    if( iter == _stick_states.end() )
        return false ;

    return iter->second.bs == so_device::so_vgamepad::xbox_button_state::pressed ;
}

//****************************************************************************************
bool_t xbox_360::is_pressing( so_device::so_vgamepad::xbox_stick b ) const
{
    auto const iter = _stick_states.find( b ) ;
    if( iter == _stick_states.end() )
        return false ;

    return iter->second.bs == so_device::so_vgamepad::xbox_button_state::pressing ;
}

//****************************************************************************************
bool_t xbox_360::is_released( so_device::so_vgamepad::xbox_stick b ) const
{
    auto const iter = _stick_states.find( b ) ;
    if( iter == _stick_states.end() )
        return false ;

    return iter->second.bs == so_device::so_vgamepad::xbox_button_state::released ;
}

//****************************************************************************************
bool_t xbox_360::set_motor_on( so_device::so_vgamepad::xbox_motor m, float_t ) 
{
    // need device
    switch(m)
    {
        case so_device::so_vgamepad::xbox_motor::invalid:
            break;
        case so_device::so_vgamepad::xbox_motor::left:
            break;
        case so_device::so_vgamepad::xbox_motor::right:
            break;
        default:
            break;
    }
    return true ;
}

//****************************************************************************************
bool_t xbox_360::set_motor_off( so_device::so_vgamepad::xbox_motor m ) 
{
    // need device
    switch(m)
    {
        case so_device::so_vgamepad::xbox_motor::invalid:
            break;
        case so_device::so_vgamepad::xbox_motor::left:
            break;
        case so_device::so_vgamepad::xbox_motor::right:
            break;
        default:
            break;
    }
    return true ;
}

//****************************************************************************************
bool_t xbox_360::is_active( void_t ) const
{
    // @todo plug and play here!
    return true ;
}

//****************************************************************************************
bool_t xbox_360::set_shoulder_state( so_device::so_vgamepad::xbox_shoulder b,
    so_device::so_vgamepad::xbox_button_state s )
{
    if( s == so_device::so_vgamepad::xbox_button_state::invalid )
        return false ;

    _shoulder_states[ b ] = s ;
    return true ;
}

//****************************************************************************************
bool_t xbox_360::set_button_state( so_device::so_vgamepad::xbox_button b,
    so_device::so_vgamepad::xbox_button_state s )
{
    if( s == so_device::so_vgamepad::xbox_button_state::invalid )
        return false ;

    _button_states[ b ] = s ;
    return true ;
}

//****************************************************************************************
bool_t xbox_360::set_dpad_state( so_device::so_vgamepad::xbox_dpad b,
    so_device::so_vgamepad::xbox_button_state s )
{
    if( s == so_device::so_vgamepad::xbox_button_state::invalid )
        return false ;

    _dpad_states[ b ] = s ;
    return true ;
}

//****************************************************************************************
bool_t xbox_360::set_stick_state( so_device::so_vgamepad::xbox_stick b,
    so_device::so_vgamepad::xbox_stick_state s, so_math::vec2f_in_t v )
{
    if( s == so_device::so_vgamepad::xbox_stick_state::invalid )
        return false ;

    so_device::so_vgamepad::xbox_button_state bs = 
        so_device::so_vgamepad::xbox_button_state::invalid ;

    auto iter = _stick_states.find( b ) ;
    if( iter != _stick_states.end() )
    {
        bs = iter->second.bs ;
    }

    _stick_states[b] = stick_data{ s, bs, v } ;

    return true ;
}

//****************************************************************************************
bool_t xbox_360::set_stick_state( so_device::so_vgamepad::xbox_stick b,
    so_device::so_vgamepad::xbox_button_state bs )
{
    if( bs == so_device::so_vgamepad::xbox_button_state::invalid )
        return false ;

    so_device::so_vgamepad::xbox_stick_state ss =
        so_device::so_vgamepad::xbox_stick_state::invalid ;

    so_math::vec2f_t v ;

    auto iter = _stick_states.find( b ) ;
    if( iter != _stick_states.end() )
    {
        ss = iter->second.s ;
        v = iter->second.value ;
    }

    _stick_states[ b ] = stick_data { ss, bs, v } ;

    return true ;
}

//****************************************************************************************
bool_t xbox_360::set_trigger_state( so_device::so_vgamepad::xbox_trigger b,
    so_device::so_vgamepad::xbox_button_state s, float_t v )
{
    if( s == so_device::so_vgamepad::xbox_button_state::invalid )
        return false ;

    _trigger_states[ b ] = trigger_data { s, v } ;
    return true ;
}

//****************************************************************************************
void_t xbox_360::reset( void_t )
{
    _button_states.clear() ;
    _shoulder_states.clear() ;
    _dpad_states.clear() ;
    _stick_states.clear() ;
    _trigger_states.clear() ;
}

//****************************************************************************************
void_t xbox_360::destroy( void_t )
{
    this_t::destroy( this ) ;
}

//****************************************************************************************
