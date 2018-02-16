//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "system.h"
#include "../plugs/imgui.h"

#include <snakeoil/device/global.h>
#include <snakeoil/device/system/device_system.h>
#include <snakeoil/device/devices/mouse/three_button_mouse.h>
#include <snakeoil/device/devices/keyboard/ascii_keyboard.h>

#include <snakeoil/log/global.h>

using namespace so_imgui ;

//************************************************************************************
system::system( so_gpx::render_system_ptr_t rs ) : _rs( rs )
{
    so_log::global_t::critical( so_core::is_nullptr( rs ), 
        "[system::system] : render system must not be nullptr" ) ;

    _ctx = ImGui::CreateContext() ;

    _imgui = so_imgui::imgui_t::create( so_imgui::imgui_t( "", _rs ), 
        "[so_imgui::system::system] : imgui" ) ;
}

//************************************************************************************
system::system( this_rref_t rhv )
{
    so_move_member_ptr( _rs, rhv ) ;
    so_move_member_ptr( _imgui, rhv ) ;
    so_move_member_ptr( _ctx, rhv ) ;
    ImGui::SetCurrentContext( _ctx ) ;

    _name_to_texdata = std::move( _name_to_texdata ) ;
}

//************************************************************************************
system::~system( void_t )
{
    if( so_core::is_not_nullptr( _ctx ) )
        ImGui::DestroyContext( _ctx ) ;

    imgui_t::destroy( _imgui ) ;

    for( auto item : _name_to_texdata )
    {
        so_imgui::memory::dealloc( item.second ) ;
    }
}

//************************************************************************************
system::this_ptr_t system::create( this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_imgui::memory::alloc( std::move( rhv ), p ) ;
}

//************************************************************************************
void_t system::destroy( this_ptr_t ptr )
{
    so_imgui::memory::dealloc( ptr ) ;
}

//************************************************************************************
void_t system::init( void_t )
{
    _keyboard = so_device::global_t::get()->device_system()->find_ascii_keyboard() ;
    _mouse = so_device::global_t::get()->device_system()->find_three_button_mouse() ;

    _imgui->init() ;

    ImGuiIO& io = ImGui::GetIO();
    io.KeyMap[ ImGuiKey_Tab ] = size_t( so_device::ascii_key::tab ) ;
    io.KeyMap[ ImGuiKey_LeftArrow ] = size_t( so_device::ascii_key::arrow_left );
    io.KeyMap[ ImGuiKey_RightArrow ] = size_t( so_device::ascii_key::arrow_right );
    io.KeyMap[ ImGuiKey_UpArrow ] = size_t( so_device::ascii_key::arrow_up );
    io.KeyMap[ ImGuiKey_DownArrow ] = size_t( so_device::ascii_key::arrow_down );
    io.KeyMap[ ImGuiKey_PageUp ] = size_t( so_device::ascii_key::one_up );
    io.KeyMap[ ImGuiKey_PageDown ] = size_t( so_device::ascii_key::one_down );
    //io.KeyMap[ ImGuiKey_Home ] = size_t( so_device::ascii_key::ho );
    io.KeyMap[ ImGuiKey_End ] = size_t( so_device::ascii_key::sk_end );
    io.KeyMap[ ImGuiKey_Insert ] = size_t( so_device::ascii_key::insert );
    io.KeyMap[ ImGuiKey_Delete ] = size_t( so_device::ascii_key::k_delete );
    io.KeyMap[ ImGuiKey_Backspace ] = size_t( so_device::ascii_key::back_space );
    io.KeyMap[ ImGuiKey_Enter ] = size_t( so_device::ascii_key::k_return );
    io.KeyMap[ ImGuiKey_Escape ] = size_t( so_device::ascii_key::escape );
    io.KeyMap[ ImGuiKey_A ] = size_t( so_device::ascii_key::a );
    io.KeyMap[ ImGuiKey_C ] = size_t( so_device::ascii_key::c );
    io.KeyMap[ ImGuiKey_V ] = size_t( so_device::ascii_key::v );
    io.KeyMap[ ImGuiKey_X ] = size_t( so_device::ascii_key::x );
    io.KeyMap[ ImGuiKey_Y ] = size_t( so_device::ascii_key::y );
    io.KeyMap[ ImGuiKey_Z ] = size_t( so_device::ascii_key::z );

    ImGui::StyleColorsClassic();
}

//************************************************************************************
void_t system::release( void_t )
{
    _imgui->deinit() ;
}

//************************************************************************************
void_t system::check_status( imgui_funk_t funk )
{
    funk( ImGui::GetCurrentContext(), this ) ;
}

//************************************************************************************
void_t system::begin_draw( double_t const dt,
    size_t const dw, size_t const dh, size_t const fbw, size_t const fbh )
{
    ImGuiIO& io = ImGui::GetIO();

    io.DisplaySize = ImVec2( ( float ) fbw, ( float ) fbh );
    io.DisplayFramebufferScale = 
        ImVec2( fbw > 0 ? ( ( float_t ) dw / fbw ) : 0,
        fbh > 0 ? ( ( float_t ) dh / fbh ) : 0 );

    io.DeltaTime = float_t(dt) ;
    io.MousePos = ImVec2( -FLT_MAX, -FLT_MAX );

    // handle keyboard
    if( so_core::is_not_nullptr( _keyboard ) )
    {
        _keyboard->for_each_key( [&] ( so_device::ascii_key const k, so_device::key_state const ks )
        {
            char_t c ;
            if( so_device::convert_key_to_ascii_char( k, c ) )
            {
                io.KeysDown[ size_t( k ) ] = ks == so_device::key_state::released ? false : true ;
                if( !io.KeysDown[ size_t( k ) ] ) io.AddInputCharacter( c ) ;
            }
            else if( so_device::convert_key_to_ascii_number( k, c ) )
            {
                io.KeysDown[ size_t( k ) ] = ks == so_device::key_state::released ? false : true ;
                if( !io.KeysDown[ size_t( k ) ] ) io.AddInputCharacter( c ) ;
            }
            else
            {
                io.KeysDown[ size_t( k ) ] = _keyboard->is_pressing( k ) == true ;
            }
        } ) ;

        io.KeyCtrl = io.KeysDown[ size_t( so_device::ascii_key::ctrl_left ) ] ||
            io.KeysDown[ size_t( so_device::ascii_key::ctrl_right ) ];

        io.KeyShift = io.KeysDown[ size_t( so_device::ascii_key::shift_left ) ] ||
            io.KeysDown[ size_t( so_device::ascii_key::shift_right ) ];

        io.KeyAlt = io.KeysDown[ size_t( so_device::ascii_key::alt_left ) ] ||
            io.KeysDown[ size_t( so_device::ascii_key::alt_right ) ];

        io.KeySuper = io.KeysDown[ size_t( so_device::ascii_key::win_win_left ) ] ||
            io.KeysDown[ size_t( so_device::ascii_key::win_win_right ) ] ;
    }

    // handle mouse
    if( so_core::is_not_nullptr( _mouse ) )
    {
        // 1. mouse position
        {
            if( io.WantMoveMouse )
            {
                // @todo set mouse cursor
            }
            else
            {
                so_math::vec2f_t pos = _mouse->get_local_position( so_math::vec2f_t( 0.0f,1.0f ) ) *
                    so_math::vec2f_t( 1.0f, -1.0f );

                //starts to left, positive downwards
                pos = pos * so_math::vec2f_t( float_t(dw), float_t(dh) ) ;
                io.MousePos = ImVec2( pos.x(), pos.y() ) ;
            }
        }

        // 2. buttons
        {
            for( size_t i = 0; i < 3; ++i )
            {
                io.MouseDown[ i ] = false ;
            }

            if( _mouse->is_pressing( so_device::three_button::left ) )
            {
                io.MouseDown[ 0 ] = true ;
            }
            if( _mouse->is_pressing( so_device::three_button::right ) )
            {
                io.MouseDown[ 1 ] = true ;
            }
            if( _mouse->is_pressing( so_device::three_button::middle ) )
            {
                io.MouseDown[ 2 ] = true ;
            }
        }

        // 3. wheel
        {
            float_t const m = 1.0f ; // io.KeyCtrl ? 1.0f : 2.5f ;
            float_t const s = float_t(_mouse->get_scroll()) / m ;
            
            if( io.KeyShift ) io.MouseWheelH = s ;
            else io.MouseWheel = s ;
        }
    }
    

    ImGui::NewFrame();
}

//************************************************************************************
void_t system::draw( imgui_funk_t funk )
{
    funk( ImGui::GetCurrentContext(), this ) ;
}

//************************************************************************************
void_t system::render( so_gpx::window_id_t wid )
{
    ImGui::Render() ;

    _imgui->schedule( wid, ImGui::GetDrawData() ) ;
}

//************************************************************************************
ImTextureID system::create_texture_id( so_std::string_cref_t name_in )
{
    {
        auto const iter = _name_to_texdata.find( name_in ) ;
        if( iter != _name_to_texdata.end() )
        {
            return iter->second ;
        }
    }

    auto * ptr = so_imgui::memory::alloc( imgui_texture_data_t { name_in }, 
        "[system::create_texture_id] : texture_data" ) ;

    _name_to_texdata[ name_in ] = ptr ;

    return ptr ;
}