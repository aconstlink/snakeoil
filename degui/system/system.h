//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "../api.h"
#include "../protos.h"
#include "../typedefs.h"

#include <snakeoil/gpx/protos.h>
#include <snakeoil/gpx/window/window_id.h>
#include <snakeoil/gpx/technique/technique_id.h>

#include <snakeoil/device/protos.h>
#include <snakeoil/std/string/string.hpp>
#include <snakeoil/std/container/map.hpp>

#include <imgui/imgui.h>

#include <functional>

namespace so_imgui
{
    class SNAKEOIL_DEGUI_API system
    {
        so_this_typedefs( system ) ;

    public:

        typedef std::function< void_t ( ImGuiContext *, this_ptr_t ) > imgui_funk_t ;

        struct imgui_texture_data
        {
            so_std::string_t name ;
        };
        so_typedef( imgui_texture_data ) ;

    private:

        so_gpx::render_system_ptr_t _rs = nullptr ;
        so_imgui::imgui_ptr_t _imgui = nullptr ;

        so_device::three_button_mouse_ptr_t _mouse = nullptr ;
        so_device::ascii_keyboard_ptr_t _keyboard = nullptr ;

        ImGuiContext * _ctx = nullptr ;

    private:

        typedef so_std::map< so_std::string_t, imgui_texture_data_ptr_t > __name_to_texdata_t ;
        so_typedefs( __name_to_texdata_t, name_to_texdata ) ;

        name_to_texdata_t _name_to_texdata ;
        
    public:

        system( so_gpx::render_system_ptr_t ) ;
        system( this_cref_t ) = delete ;
        system( this_rref_t ) ; 
        ~system( void_t ) ;

    public:

        static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
        static void_t destroy( this_ptr_t ) ;

    public:

        void_t init( void_t ) ;
        void_t release( void_t ) ;
        void_t check_status( imgui_funk_t ) ;
        void_t begin_draw( double_t const dt, size_t const dw, size_t const dh, size_t const fbw, size_t const fbh ) ;
        void_t draw( imgui_funk_t ) ;
        void_t render( so_gpx::window_id_t ) ;

    public:

        ImTextureID create_texture_id( so_std::string_cref_t ) ;

    };
    so_typedef( system ) ;
}