//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "rawinput_api.h"
#include "rawinput_button_mapping.h"

#include "../../../devices/mouse/three_button_mouse.h"
#include "../../../devices/keyboard/ascii_keyboard.h"
#include "../../../devices/mouse/three_button_mouse.h"

#include "../map_virtual_key_to_ascii_key.h"

#include "../../../component/buttons/button_state.h"
#include "../../../component/buttons/three_button.h"
#include "../../../component/buttons/three_button_mapping.h"

#include "../../gamepad_notify.h"

#include <snakeoil/memory/guards/malloc_guard.hpp>
#include <snakeoil/math/vector/vector2.hpp>
#include <snakeoil/log/global.h>


#include <hidsdi.h>
#include <Hidpi.h>

using namespace so_device ;
using namespace so_device::so_win32 ;

//*****************************************************************************************
rawinput_api::rawinput_api( void_t )
{
    RAWINPUTDEVICE rid[ 3 ] ;

    // mouse
    rid[ 0 ].usUsagePage = 0x01 ;
    rid[ 0 ].usUsage = 0x02 ;
    rid[ 0 ].hwndTarget = NULL ;
    rid[ 0 ].dwFlags = 0;//RIDEV_CAPTUREMOUSE | RIDEV_NOLEGACY ;

    // keyboard
    rid[ 1 ].usUsagePage = 0x01 ;
    rid[ 1 ].usUsage = 0x06 ;
    rid[ 1 ].hwndTarget = NULL ;
    rid[ 1 ].dwFlags = 0;//RIDEV_NOLEGACY ;

    // joystick
    rid[ 2 ].usUsagePage = 0x01 ;
    rid[ 2 ].usUsage = 0x04 ;
    rid[ 2 ].hwndTarget = NULL ;
    rid[ 2 ].dwFlags = 0;//RIDEV_NOLEGACY ;

    so_log::global::error(
        RegisterRawInputDevices( rid, 3, sizeof( RAWINPUTDEVICE ) ) == FALSE,
        "[rawinput::initialize] : need raw input support."
    ) ;

    {
        so_device::ascii_keyboard_t ak ;
        _ascii_keyboards.push_back( so_device::ascii_keyboard_t::create( std::move( ak ), 
            "[rawinput_api::rawinput_api] : ascii_keyboard" ) ) ;
    }

    {
        so_device::three_button_mouse_t m ;
        _three_button_mice.push_back( so_device::three_button_mouse_t::create( std::move( m ),
            "[rawinput_api::rawinput_api] : three_button_mouse" ) ) ;
    }

    // temp: for fast generic usb gamepad device acces
    {

    }
}

//*****************************************************************************************
rawinput_api::rawinput_api( this_rref_t rhv  )
{
    _raw_input_data_size = rhv._raw_input_data_size ;
    so_move_member_ptr( _raw_input_buffer, rhv ) ;

    _ascii_keyboards = std::move( rhv._ascii_keyboards ) ;
    _three_button_mice = std::move( rhv._three_button_mice ) ;
    _gamepad_notifies = std::move( rhv._gamepad_notifies ) ;
}

//*****************************************************************************************
rawinput_api::~rawinput_api( void_t )
{
    so_device::memory::dealloc( _raw_input_buffer ) ;

    for( auto * ptr : _ascii_keyboards )
    {
        so_device::ascii_keyboard_t::destroy( ptr ) ;
    }

    for( auto * ptr : _three_button_mice )
    {
        so_device::three_button_mouse_t::destroy( ptr ) ;
    }

    for( auto * ptr : _gamepad_notifies )
    {
        ptr->destroy() ;
    }
}

//*****************************************************************************************
rawinput_api::this_ptr_t rawinput_api::create( so_memory::purpose_cref_t p )
{
    return this_t::create( this_t(), p ) ;
}

//*****************************************************************************************
rawinput_api::this_ptr_t rawinput_api::create( this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_device::memory::alloc( std::move( rhv ), p ) ;
}

//*****************************************************************************************
void_t rawinput_api::destroy( this_ptr_t ptr )
{
    so_device::memory::dealloc( ptr ) ;
}

//*****************************************************************************************
so_device::ascii_keyboard_ptr_t rawinput_api::find_ascii_keyboard( void_t )
{
    if( _ascii_keyboards.size() == 0 )
        return nullptr ;

    return _ascii_keyboards[ 0 ] ;
}

//*****************************************************************************************
so_device::three_button_mouse_ptr_t rawinput_api::find_three_button_mouse( void_t )
{
    if( _three_button_mice.size() == 0 )
        return nullptr ;

    return _three_button_mice[ 0 ] ;
}

//*****************************************************************************************
void_t rawinput_api::create_devices( so_device::igamepad_module_ptr_t )
{

}

//*****************************************************************************************
so_device::result rawinput_api::register_device( so_device::key_cref_t,
    so_device::gamepad_device_ptr_t )
{
    return so_device::not_implemented ;
}

//*****************************************************************************************
so_device::result rawinput_api::unregister_device( so_device::key_cref_t )
{
    return so_device::not_implemented ;
}

//*****************************************************************************************
so_device::gamepad_device_ptr_t rawinput_api::find_any_device( void_t )
{
    return nullptr ;
}

//*****************************************************************************************
so_device::so_vgamepad::xbox_360_ptr_t rawinput_api::find_device( size_t const )
{
    return nullptr ;
}

//*****************************************************************************************
bool_t rawinput_api::register_for_any_device( so_device::so_vdev::ivdev_ptr_t )
{
    return false ;
}

//*****************************************************************************************
bool_t rawinput_api::register_device( size_t const, so_device::so_vdev::ivdev_ptr_t )
{
    return false ;
}

//*****************************************************************************************
bool_t rawinput_api::unregister_virtual_device( so_device::so_vdev::ivdev_ptr_t )
{
    return false ;
}

//*****************************************************************************************
void_t rawinput_api::install_gamepad_notify( so_device::igamepad_notify_ptr_t nptr )
{
    auto const iter = std::find( _gamepad_notifies.begin(), _gamepad_notifies.end(), nptr ) ;
    if( iter != _gamepad_notifies.end() ) return ;

    _gamepad_notifies.push_back( nptr ) ;
}

//*****************************************************************************************
void_t rawinput_api::update_mouse( void_t )
{
    so_thread::lock_t lk( _buffer_mtx ) ;

    for( auto * kptr : _three_button_mice )
    {
        // 1. promote all already inserted keys
        kptr->update() ;

        // 2. insert new keys
        for( auto const & item : _three_button_items )
        {
            kptr->add_mouse_event( item.first, item.second ) ;
        }
        
        if( _pointer_coords_global.size() > 0 )
            kptr->set_global_position( _pointer_coords_global.back() ) ;

        if( _pointer_coords_local.size() > 0 )
            kptr->set_local_position( _pointer_coords_local.back() ) ;
        
        if( _scroll_items.size() > 0 )
        {
            if( _scroll_items.back() == 65416 ) kptr->scroll_down() ;
            else if( _scroll_items.back() == 120 ) kptr->scroll_up() ;
            else kptr->scroll_reset() ;
        }
    }

    _three_button_items.clear() ;
    _pointer_coords_global.clear() ;
    _pointer_coords_local.clear() ;
    _scroll_items.clear() ;
}

//*****************************************************************************************
void_t rawinput_api::update_keyboard( void_t )
{
    so_thread::lock_t lk( _buffer_mtx ) ;

    for( auto * kptr : _ascii_keyboards )
    {
        // 1. promote all already inserted keys
        kptr->update() ;

        // 2. insert new keys
        for( auto const & item : _ascii_keyboard_keys )
        {
            kptr->add_key_event( item.first, item.second ) ;
        }
        _ascii_keyboard_keys.clear() ;
    }
}

//*****************************************************************************************
void_t rawinput_api::update_gamepad( void_t ) 
{

}

//*****************************************************************************************
void_t rawinput_api::destroy( void_t )
{
    this_t::destroy( this ) ;
}

//*****************************************************************************************
bool_t rawinput_api::handle_input_event( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    if( msg != WM_INPUT )
        return false ;

    //
    // 1. get the input data from the api
    //
    UINT sib ;
    GetRawInputData( ( HRAWINPUT ) lParam, RID_INPUT, NULL, &sib, sizeof( RAWINPUTHEADER ) ) ;
    if( sib > _raw_input_data_size )
    {
        so_device::memory::dealloc( _raw_input_buffer ) ;
        _raw_input_buffer = so_device::memory::alloc<BYTE>( sib ) ;
        _raw_input_data_size = sib ;
    }

    if( GetRawInputData( ( HRAWINPUT ) lParam, RID_INPUT,
        _raw_input_buffer, &sib, sizeof( RAWINPUTHEADER ) ) != sib )
        return true ;
    
    RAWINPUT * raw = ( RAWINPUT* ) _raw_input_buffer ;

    if( raw->header.dwType == RIM_TYPEMOUSE )
    {
        bool_t outside = false ;

        POINT pt, client_pt ;
        GetCursorPos( &pt ) ;

        client_pt = pt ;
        ScreenToClient( hwnd, &client_pt ) ;

        // check if the event occurred within
        // the client area.
        // there is a problem with rawinput mice.
        // if a window is focused and the user presses 
        // another window, the original focused window does
        // NOT receive a release event! But it will receive the 
        // press event of the newly focused window!
        {
            RECT r ;
            GetClientRect( hwnd, &r ) ;
            so_math::vec2f_t window_dim(
                float_t( r.right - r.left ),
                float_t( r.bottom - r.top )
            ) ;

            if( client_pt.x < r.left || client_pt.x > r.right ||
                client_pt.y < r.top || client_pt.y > r.bottom )
            {
                outside = true ;
            }
        }

        so_device::button_state bs = so_device::button_state::none ;
        POINT last_pt = { raw->data.mouse.lLastX, raw->data.mouse.lLastY } ;

        if( raw->data.mouse.usFlags == MOUSE_MOVE_RELATIVE )
        {
            bs = ( last_pt.x == 0 && last_pt.y == 0 ) ?
                so_device::button_state::pressed :
                so_device::button_state::none ;
        }
        else if( raw->data.mouse.usFlags == MOUSE_MOVE_ABSOLUTE )
        {
            bs = ( pt.x == last_pt.x && pt.y == last_pt.y ) ?
                so_device::button_state::pressed :
                so_device::button_state::none ;
        }

        so_thread::lock_t lk( _buffer_mtx ) ;

        // do buttons and state
        {
            using so_device::button_state ;
            using so_device::three_button ;

            // left
            {
                const USHORT bflags = raw->data.mouse.usButtonFlags ;
                const button_state lbs = map_raw_button_state_left_to_button_state( bflags ) ;
                const three_button tb = map_raw_button_left_to_three_button( bflags ) ;

                // we check if the mouse is 
                // outside the client area for the time of clicking. if so,
                // we just omit the press event. Because the press event is 
                // issued to the original focused window, and not to the newly one.
                if( lbs == button_state::pressed && !outside )
                {
                    _three_button_items.push_back( mouse_button_item_t( tb, lbs ) ) ;
                }
                else if( lbs == button_state::released )
                {
                    _three_button_items.push_back( mouse_button_item_t( tb, lbs ) ) ;
                }
            }

            // right
            {
                const USHORT bflags = raw->data.mouse.usButtonFlags ;
                const button_state lbs = map_raw_button_state_right_to_button_state( bflags ) ;
                const three_button tb = map_raw_button_right_to_three_button( bflags ) ;

                _three_button_items.push_back( mouse_button_item_t( tb, lbs ) ) ;
            }

            // middle
            {
                const USHORT bflags = raw->data.mouse.usButtonFlags ;
                const button_state lbs = map_raw_button_state_middle_to_button_state( bflags ) ;
                const three_button tb = map_raw_button_middle_to_three_button( bflags ) ;

                _three_button_items.push_back( mouse_button_item_t( tb, lbs ) ) ;
            }

            // wheel
            {
                const USHORT bflags = raw->data.mouse.usButtonFlags ;
                if( bflags == RI_MOUSE_WHEEL )
                {
                    //so_log::global_t::status( std::to_string( short_t(raw->data.mouse.usButtonData) ) ) ;
                    auto const v = raw->data.mouse.usButtonData ;
                    _scroll_items.push_back( v ) ;
                }
            }
        }

        // do global
        {
            so_math::vec2f_t const dim(
                float_t( GetSystemMetrics( SM_CXSCREEN ) ),
                float_t( GetSystemMetrics( SM_CYSCREEN ) )
            ) ;

            so_math::vec2f_t const v = so_math::vec2f_t( float_t( pt.x ), float_t( dim.y() - pt.y ) ) / dim ;
            _pointer_coords_global.push_back( v ) ;
        }

        // do local
        {
            RECT r ;
            GetClientRect( hwnd, &r ) ;
            so_math::vec2f_t const dim(
                float_t( r.right - r.left ),
                float_t( r.bottom - r.top )
            ) ;

            so_math::vec2f_t const v = so_math::vec2f_t( float_t( client_pt.x ), float_t( dim.y() - client_pt.y ) ) / dim ;
            _pointer_coords_local.push_back( v ) ;
        }

        return true ;
    }
    else if( raw->header.dwType == RIM_TYPEKEYBOARD )
    {
        so_thread::lock_t lk( _buffer_mtx ) ;

        _ascii_keyboard_keys.push_back( ascii_keyboard_key_item_t(
            so_device::so_win32::map_virtual_keycode_to_ascii_key( raw->data.keyboard.VKey ),
            so_device::so_win32::map_key_state_raw_wm_for_vzt_device( raw->data.keyboard.Message )
        ) ) ;

        return true ;
    }
    // some other device
    #if 0
    else
    {
        uint_t num_devices = 0 ;
        RID_DEVICE_INFO di[1000] ;
        for( size_t i = 0; i < 1000; ++i )
        {
            ZeroMemory( &di[i], sizeof( RID_DEVICE_INFO ) ) ;
            di[i].cbSize = sizeof( RID_DEVICE_INFO ) ;
        }

        {
            uint_t buffer_size = sizeof( RID_DEVICE_INFO ) * 1000  ;

            
            auto ires = GetRawInputDeviceInfo( raw->header.hDevice, RIDI_DEVICEINFO, NULL, &buffer_size ) ;
            so_log::global::error( ires == -1, "[rawinput_api::] : GetRawInputDeviceInfo" ) ;

            ires = GetRawInputDeviceInfo( raw->header.hDevice, RIDI_DEVICEINFO, di, &buffer_size ) ;
            so_log::global::error( ires == -1, "[rawinput_api::] : GetRawInputDeviceInfo" ) ;
            
            if( ires != uint_t(-1) )
                num_devices = buffer_size / sizeof( RID_DEVICE_INFO ) ;
        }
        
        for( uint_t i = 0; i < num_devices; ++i )
        {
            if( di[ i ].hid.usUsage != 4 ) continue ;

            uint_t joystick_id = uint_t( 0 ) ;

            //so_log::global_t::status( "Type : " + std::to_string( di[i].hid.usUsage ) ) ;

            {
                uint_t buffer_size = 0 ;
                
                {
                    auto ires = GetRawInputDeviceInfo( raw->header.hDevice, RIDI_DEVICENAME, NULL, &buffer_size ) ;
                    so_log::global::error( ires == -1, "[rawinput_api::] : GetRawInputDeviceInfo" ) ;

                    if( ires == -1 ) continue ;
                }


                so_memory::malloc_guard<char_t> device_name( buffer_size ) ;

                {
                    auto ires = GetRawInputDeviceInfo( raw->header.hDevice, RIDI_DEVICENAME, device_name, &buffer_size ) ;
                    so_log::global::error( ires == -1, "[rawinput_api::] : GetRawInputDeviceInfo" ) ;

                    if( ires == -1 ) continue ;
                }

                //so_log::global_t::status( "Name : " + so_std::string_t( device_name ) ) ;
                {
                    auto const iter = _gamepad_ids.find( so_std::string_t( device_name ) ) ;
                    if( iter == _gamepad_ids.end() )
                    {
                        _gamepad_ids[ so_std::string_t( device_name ) ] = uint_t(_gamepad_ids.size()) ;
                    }

                    joystick_id = _gamepad_ids[ so_std::string_t( device_name ) ] ;
                }
                
            }

            {
                uint_t buffer_size = 0 ;

                {
                    auto ires = GetRawInputDeviceInfo( raw->header.hDevice, RIDI_PREPARSEDDATA, NULL, &buffer_size ) ;
                    //so_log::global::error( ires == -1, "[rawinput_api::] : GetRawInputDeviceInfo" ) ;
                    if( ires == -1 ) continue ;
                }

                so_memory::malloc_guard<byte_t> preparsed_data( buffer_size ) ;

                {
                    auto ires = GetRawInputDeviceInfo( raw->header.hDevice, RIDI_PREPARSEDDATA, preparsed_data, &buffer_size ) ;
                    //so_log::global::error( ires == -1, "[rawinput_api::] : GetRawInputDeviceInfo" ) ;
                    if( ires == -1 ) continue ;
                }

                HIDP_CAPS caps ;
                {
                    auto const ires = HidP_GetCaps( *((PHIDP_PREPARSED_DATA*)&preparsed_data), &caps ) ;
                    if( ires != HIDP_STATUS_SUCCESS ) continue ;
                }

                uint_t num_buttons = 0 ;

                so_memory::malloc_guard<HIDP_BUTTON_CAPS> bcaps( caps.NumberInputButtonCaps ) ;
                so_memory::malloc_guard<HIDP_VALUE_CAPS> vcaps( caps.NumberInputValueCaps ) ;

                // button caps
                {
                    auto length = caps.NumberInputButtonCaps ;
                    auto const ires = HidP_GetButtonCaps( HidP_Input, bcaps,
                        &length, *( ( PHIDP_PREPARSED_DATA* ) &preparsed_data ) ) ;
                    if( ires != HIDP_STATUS_SUCCESS ) continue ;

                    /*for( size_t c = 0; c < size_t( length ); ++c )
                    {
                        num_buttons = bcaps[ c ].Range.UsageMax - bcaps[ c ].Range.UsageMin + 1;
                        //so_log::global_t::status( "Num Buttons : " + std::to_string( num_buttons ) ) ;
                    }*/
                    num_buttons = bcaps[ 0 ].Range.UsageMax - bcaps[ 0 ].Range.UsageMin + 1;
                }

                // value caps
                {
                    auto length = caps.NumberInputValueCaps ;
                    auto const ires = HidP_GetValueCaps( HidP_Input, vcaps,
                        &length, *( ( PHIDP_PREPARSED_DATA* ) &preparsed_data ) ) ;
                    if( ires != HIDP_STATUS_SUCCESS ) continue ;

                    for( size_t c = 0; c < size_t( length ); ++c )
                    {

                    }
                }

                {
                    so_memory::malloc_guard<USAGE> usage( num_buttons ) ;
                    ULONG usageLength = num_buttons;
                    auto const ires = HidP_GetUsages( HidP_Input, bcaps[0].UsagePage, 0, usage,
                        &usageLength, *( ( PHIDP_PREPARSED_DATA* ) &preparsed_data ),
                        ( PCHAR ) raw->data.hid.bRawData, raw->data.hid.dwSizeHid
                    ) ;
                    if( ires != HIDP_STATUS_SUCCESS ) continue ;

                    so_memory::malloc_guard<BOOL> button_states( num_buttons ) ;
                    
                    ZeroMemory( button_states, sizeof( button_states ) );
                    for( i = 0; i < usageLength; i++ )
                    {
                        auto const index = usage[ i ] - bcaps[ 0 ].Range.UsageMin ;
                        button_states[ index ] = TRUE;

                        //so_log::global_t::status("button : " + std::to_string(index)) ;

                        for( auto * nptr : _gamepad_notifies )
                        {
                            nptr->on_button_press( "rawinput_deivce_" + std::to_string( joystick_id ), joystick_id, index ) ;
                        }
                    }
                }

                {
                    for( size_t v = 0; v < caps.NumberInputValueCaps; v++ )
                    {
                        ULONG value = 0 ;
                        LONG xaxis = 0, yaxis = 0 ;

                        auto const ires = HidP_GetUsageValue( HidP_Input, vcaps[ v ].UsagePage, 0,
                            vcaps[ v ].Range.UsageMin, &value, *( ( PHIDP_PREPARSED_DATA* ) &preparsed_data ),
                            ( PCHAR ) raw->data.hid.bRawData, raw->data.hid.dwSizeHid
                        ) ;

                        if( ires != HIDP_STATUS_SUCCESS ) continue ;

                        switch( vcaps[ v ].Range.UsageMin )
                        {
                        case 0x30:    // X-axis
                            xaxis = ( LONG ) value - 127;
                            break;

                        case 0x31:    // Y-axis
                            yaxis = ( LONG ) value - 127;
                            break;
                            /*
                        case 0x32: // Z-axis
                            lAxisZ = ( LONG ) value - 128;
                            break;

                        case 0x35: // Rotate-Z
                            lAxisRz = ( LONG ) value - 128;
                            break;

                        case 0x39:    // Hat Switch
                            lHat = value;
                            break;*/
                        }

#if 0                   
                        if( xaxis != 0 )
                            so_log::global_t::status( "xaxis : " + std::to_string( xaxis ) ) ;

                        if( yaxis != 0 )
                            so_log::global_t::status( "yaxis : " + std::to_string( yaxis ) ) ;
#endif

                        if( xaxis != 0 || yaxis != 0 )
                        {
                            for( auto * nptr : _gamepad_notifies )
                            {
                                nptr->on_dpad_press( "rawinput_deivce_" + std::to_string( joystick_id ), joystick_id, xaxis, yaxis ) ;
                            }
                        }
                    }
                }

                
            }
        }
    }
    #endif
    //DefRawInputProc( &raw, sib/sizeof(RAWINPUT), sizeof(RAWINPUTHEADER) ) ;

    return true ;
}