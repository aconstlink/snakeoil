//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "../api.h"
#include "../protos.h"
#include "../typedefs.h"

#include <snakeoil/gpx/protos.h>
#include <snakeoil/gpx/technique/technique_id.h>

#include <snakeoil/device/protos.h>

#include <imgui/imgui.h>

namespace so_imgui
{
    class SNAKEOIL_IMGUI_API system
    {
        so_this_typedefs( system ) ;

    public:

        typedef std::function< void_t ( ImGuiContext * ) > imgui_funk_t ;

    private:

        so_gpx::render_system_ptr_t _rs = nullptr ;
        so_imgui::imgui_ptr_t _imgui = nullptr ;

        so_device::three_button_mouse_ptr_t _mouse = nullptr ;
        so_device::ascii_keyboard_ptr_t _keyboard = nullptr ;

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
        void_t begin_draw( double_t const dt, size_t const dw, size_t const dh, size_t const fbw, size_t const fbh ) ;
        void_t draw( imgui_funk_t ) ;
        void_t render( void_t ) ;
    };
    so_typedef( system ) ;
}