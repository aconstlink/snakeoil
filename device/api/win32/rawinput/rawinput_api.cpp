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

#include <snakeoil/math/vector/vector2.hpp>
#include <snakeoil/log/global.h>

using namespace so_device ;
using namespace so_device::so_win32 ;

//*****************************************************************************************
rawinput_api::rawinput_api( void_t )
{
    RAWINPUTDEVICE rid[ 2 ] ;

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

    so_log::global::error(
        RegisterRawInputDevices( rid, 2, sizeof( RAWINPUTDEVICE ) ) == FALSE,
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
}

//*****************************************************************************************
rawinput_api::rawinput_api( this_rref_t rhv  )
{
    _raw_input_data_size = rhv._raw_input_data_size ;
    so_move_member_ptr( _raw_input_buffer, rhv ) ;

    _ascii_keyboards = std::move( rhv._ascii_keyboards ) ;
    _three_button_mice = std::move( rhv._three_button_mice ) ;
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
    }

    _three_button_items.clear() ;
    _pointer_coords_global.clear() ;
    _pointer_coords_local.clear() ;
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

    //DefRawInputProc( &raw, sib/sizeof(RAWINPUT), sizeof(RAWINPUTHEADER) ) ;

    return true ;
}