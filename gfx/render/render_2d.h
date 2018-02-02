//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "../typedefs.h"
#include "../protos.h"

#include <snakeoil/gpx/protos.h>

namespace so_gfx
{
    class render_2d
    {
        so_this_typedefs( render_2d ) ;

    private:

        so_gpx::render_system_ptr_t _rs_ptr ;

        so_gfx::rect_render_2d_ptr_t _rr_ptr ;
        so_gfx::circle_render_2d_ptr_t _cr_ptr ;
        so_gfx::image_render_2d_ptr_t _ir_ptr ;
        so_gfx::line_render_2d_ptr_t _lr_ptr ;

    public:

        render_2d( so_gpx::render_system_ptr_t ) ;
        render_2d( this_cref_t ) = delete ;
        render_2d( this_rref_t ) ;
        ~render_2d( void_t ) ;

    public:

        static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
        static void_t destroy( this_ptr_t ) ;

    public:

        so_gfx::rect_render_2d_ptr_t get_rect_render_2d( void_t ) ;
        so_gfx::circle_render_2d_ptr_t get_circle_render_2d( void_t ) ;
        so_gfx::image_render_2d_ptr_t get_image_render_2d( void_t ) ;
        so_gfx::line_render_2d_ptr_t get_line_render_2d( void_t ) ;

    };
}