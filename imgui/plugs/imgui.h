//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "../api.h"
#include "../protos.h"
#include "../typedefs.h"

#include <snakeoil/gpx/system/render_system.h>
#include <snakeoil/std/string/string.hpp>
#include <snakeoil/math/matrix/matrix4.hpp>

#include <imgui/imgui.h>

namespace so_imgui
{
    class SNAKEOIL_IMGUI_API imgui
    {
        so_this_typedefs( imgui ) ;

    public:

        struct shared_data
        {
            so_std::string_t name ;

            size_t width ;
            size_t height ;
            uchar_ptr_t pixels ;

            ImDrawData * dd ;
        };
        so_typedef( shared_data ) ;

    private:

        shared_data_ptr_t _sd ;

        so_gpx::technique_id_t _tid ;
        so_gpx::render_system_ptr_t _rs ;

    public:

        imgui( so_std::string_cref_t, so_gpx::render_system_ptr_t ) ;
        imgui( this_cref_t ) = delete ;
        imgui( this_rref_t ) ;
        ~imgui( void_t ) ;

    public:

        static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
        static void_t destroy( this_ptr_t ) ;

    public:

        void_t init( void_t ) ;
        void_t deinit( void_t ) ;
        void_t schedule( ImDrawData * ) ;

        void_t add_texture( so_std::string_cref_t ) ;
    };
    so_typedef( imgui ) ;
}