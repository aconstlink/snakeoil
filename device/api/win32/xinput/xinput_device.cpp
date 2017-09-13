//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "xinput_device.h"

using namespace so_device ;
using namespace so_device::so_win32 ;

//****************************************************************************************
xinput_device::xinput_device( void_t ) 
{
    memset( &_state, 0, sizeof(XINPUT_STATE) ) ;
}

//****************************************************************************************
xinput_device::xinput_device( this_rref_t rhv ) 
{
    _id = rhv._id ;
    rhv._id = DWORD(-1) ;

    memcpy( &_state, &rhv._state, sizeof(XINPUT_STATE) ) ;
}

//****************************************************************************************
xinput_device::xinput_device( DWORD id )
{
    _id = id ;
    memset( &_state, 0, sizeof(XINPUT_STATE) ) ;
}

//****************************************************************************************
xinput_device::this_ptr_t xinput_device::create( this_rref_t rhv, so_memory::purpose_cref_t p ) 
{
    return so_device::memory::alloc( std::move(rhv), p ) ;
}

//****************************************************************************************
void_t xinput_device::destroy( this_ptr_t ptr ) 
{
    so_device::memory::dealloc( ptr ) ;
}

//****************************************************************************************
DWORD xinput_device::get_id( void_t )
{
    return _id ;
}

//****************************************************************************************
bool_t xinput_device::is_pressed( XINPUT_STATE const & new_state, DWORD button ) const
{
    bool_t const old_button = bool_t( ( _state.Gamepad.wButtons & button ) != 0 ) ;
    bool_t const new_button = bool_t( ( new_state.Gamepad.wButtons & button ) != 0 ) ;

    return !old_button && new_button ;
}

//****************************************************************************************
bool_t xinput_device::is_pressing( XINPUT_STATE const & new_state, DWORD button ) const
{
    bool_t const old_button = bool_t( ( _state.Gamepad.wButtons & button ) != 0 ) ;
    bool_t const new_button = bool_t( ( new_state.Gamepad.wButtons & button ) != 0 ) ;

    return old_button && new_button ;
}

//****************************************************************************************
bool_t xinput_device::is_released( XINPUT_STATE const & new_state, DWORD button ) const
{
    bool_t const old_button = bool_t( ( _state.Gamepad.wButtons & button ) != 0 ) ;
    bool_t const new_button = bool_t( ( new_state.Gamepad.wButtons & button ) != 0 ) ;

    return old_button && !new_button ;
}

//****************************************************************************************
button_state xinput_device::check_left_trigger(
    XINPUT_STATE const & new_state, uint16_t & intensity_out ) const
{
    bool_t const old_press = bool_t( _state.Gamepad.bLeftTrigger != 0 ) ;
    bool_t const new_press = bool_t( new_state.Gamepad.bLeftTrigger != 0 ) ;

    intensity_out = uint16_t( new_state.Gamepad.bLeftTrigger ) ;

    if( new_press ) return so_device::button_state::pressed ;
    if( old_press && !new_press ) return so_device::button_state::released ;

    return so_device::button_state::none ;
}

//****************************************************************************************
button_state xinput_device::check_right_trigger(
    XINPUT_STATE const & new_state, uint16_t & intensity_out ) const
{
    bool_t const old_press = bool_t( _state.Gamepad.bRightTrigger != 0 ) ;
    bool_t const new_press = bool_t( new_state.Gamepad.bRightTrigger != 0 ) ;

    intensity_out = uint16_t( new_state.Gamepad.bRightTrigger ) ;

    if( new_press ) return so_device::button_state::pressed ;
    if( old_press && !new_press ) return so_device::button_state::released ;
    return so_device::button_state::none ;
}

//****************************************************************************************
stick_state xinput_device::check_left_stick(
    XINPUT_STATE const & new_state, so_math::vec2f_t & nnc_out ) const
{
    so_math::vec2b_t const old_tilt = so_math::vec2b_t(
        _state.Gamepad.sThumbLX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE || _state.Gamepad.sThumbLX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE,
        _state.Gamepad.sThumbLY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE || _state.Gamepad.sThumbLY < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ) ;
    so_math::vec2b_t const new_tilt = so_math::vec2b_t(
        new_state.Gamepad.sThumbLX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE || new_state.Gamepad.sThumbLX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE,
        new_state.Gamepad.sThumbLY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE || new_state.Gamepad.sThumbLY < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ) ;

    // according to ms doc, max range is in [0, 1<<16]
    // using range [0,1<<15] to get [-1,1] value instead of [-0.5,0.5] values.
    so_math::vec2f_t const inv_width = 
        so_math::vec2f_t( 1.0f / float_t( ( 1 << 15 ) - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ) ) ;

    so_math::vector2< SHORT > const tmp = so_math::vector2< SHORT >(
        new_state.Gamepad.sThumbLX, new_state.Gamepad.sThumbLY ).
        dead_zone( XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ) ;

    SHORT X = tmp.x() ;
    SHORT Y = tmp.y() ;

    X -= X > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ? XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE : 0 ;
    X += X < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ? XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE : 0 ;

    Y -= Y > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ? XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE : 0 ;
    Y += Y < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ? XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE : 0 ;

    nnc_out = so_math::vec2f_t( float_t( X ), float_t( Y ) ) * inv_width ;

    if( !old_tilt.any() && new_tilt.any() ) return so_device::stick_state::tilted ;
    if( old_tilt.any() && new_tilt.any() ) return so_device::stick_state::tilted ;
    if( old_tilt.any() && !new_tilt.any() ) return so_device::stick_state::untilted ;

    return so_device::stick_state::none ;
}

//****************************************************************************************
stick_state xinput_device::check_left_stick(
    XINPUT_STATE const & new_state, so_math::vector2<int16_t> & val_out ) const 
{
    so_math::vector2<int16_t> old_xy(
        _state.Gamepad.sThumbLX, _state.Gamepad.sThumbLY ) ;

    so_math::vector2<int16_t> new_xy(
        new_state.Gamepad.sThumbLX, new_state.Gamepad.sThumbLY ) ;

    so_math::vector2<int16_t> const dead_zone_xy(
        XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ) ;
    
    so_math::vector2<int16_t> const new_xy_dif = new_xy.clamped( dead_zone_xy.negated(), dead_zone_xy ) ;
    so_math::vector2<int16_t> const old_xy_dif = old_xy.clamped( dead_zone_xy.negated(), dead_zone_xy ) ;
    
    old_xy = old_xy + old_xy_dif.negated() ;
    new_xy = new_xy + new_xy_dif.negated() ;    
    
    val_out = new_xy ;

    so_math::vec2b_t const old_tilt = old_xy.absed().greater_than( so_math::vec2i16_t( 0 ) ) ;
    so_math::vec2b_t const new_tilt = new_xy.absed().greater_than( so_math::vec2i16_t( 0 ) ) ;

    if( new_tilt.any() ) return so_device::stick_state::tilted ;
    if( old_tilt.any() && !new_tilt.any() ) return so_device::stick_state::untilted ;

    return so_device::stick_state::none ;
}

//****************************************************************************************
stick_state xinput_device::check_right_stick(
    XINPUT_STATE const & new_state, so_math::vec2f_t & nnc_out ) const
{
    so_math::vec2b_t const old_tilt = so_math::vec2b_t(
        _state.Gamepad.sThumbRX > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE || _state.Gamepad.sThumbRX < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE,
        _state.Gamepad.sThumbRY > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE || _state.Gamepad.sThumbRY < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE ) ;
    so_math::vec2b_t const new_tilt = so_math::vec2b_t(
        new_state.Gamepad.sThumbRX > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE || new_state.Gamepad.sThumbRX < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE,
        new_state.Gamepad.sThumbRY > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE || new_state.Gamepad.sThumbRY < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE ) ;

    // according to ms doc, max range is in [0, 1<<16]
    // using range [0,1<<15] to get [-1,1] value instead of [-0.5,0.5] values.
    so_math::vec2f_t const inv_width = 
        so_math::vec2f_t( 1.0f / float_t( ( 1 << 15 ) - XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE ) ) ;

    so_math::vector2< SHORT > const tmp = so_math::vector2< SHORT >(
        new_state.Gamepad.sThumbRX, new_state.Gamepad.sThumbRY ).dead_zone( XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE ) ;

    SHORT X = tmp.x() ;
    SHORT Y = tmp.y() ;

    X -= X > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE ? XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE : 0 ;
    X += X < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE ? XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE : 0 ;

    Y -= Y > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE ? XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE : 0 ;
    Y += Y < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE ? XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE : 0 ;

    nnc_out = so_math::vec2f_t( float_t( X ), float_t( Y ) ) * inv_width ;

    if( !old_tilt.any() && new_tilt.any() ) return so_device::stick_state::tilted ;
    if( old_tilt.any() && new_tilt.any() ) return so_device::stick_state::tilted ;
    if( old_tilt.any() && !new_tilt.any() ) return so_device::stick_state::untilted ;
    return so_device::stick_state::none ;
}

//****************************************************************************************
stick_state xinput_device::check_right_stick(
    XINPUT_STATE const & new_state, so_math::vector2<int16_t> & val_out ) const 
{
    so_math::vector2<int16_t> old_xy(
        _state.Gamepad.sThumbRX, _state.Gamepad.sThumbRY ) ;

    so_math::vector2<int16_t> new_xy(
        new_state.Gamepad.sThumbRX, new_state.Gamepad.sThumbRY ) ;

    so_math::vector2<int16_t> const dead_zone_xy(
        XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE ) ;

    so_math::vector2<int16_t> const new_xy_dif = new_xy.clamped( dead_zone_xy.negated(), dead_zone_xy ) ;
    so_math::vector2<int16_t> const old_xy_dif = old_xy.clamped( dead_zone_xy.negated(), dead_zone_xy ) ;

    old_xy = old_xy + old_xy_dif.negated() ;
    new_xy = new_xy + new_xy_dif.negated() ;

    val_out = new_xy ;

    so_math::vec2b_t const old_tilt = old_xy.absed().greater_than( so_math::vec2i16_t( 0 ) ) ;
    so_math::vec2b_t const new_tilt = new_xy.absed().greater_than( so_math::vec2i16_t( 0 ) ) ;

    if( new_tilt.any() ) return so_device::stick_state::tilted ;
    if( old_tilt.any() && !new_tilt.any() ) return so_device::stick_state::untilted ;

    return so_device::stick_state::none ;
}

//****************************************************************************************
void_t xinput_device::exchange_state( XINPUT_STATE const & new_state ) 
{
    _state = new_state ;
}
