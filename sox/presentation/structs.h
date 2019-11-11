#pragma once

#include "typedefs.h"

#include <snakeoil/gfx/protos.h>

namespace sox_presentation
{
    struct update_data
    {};
    so_typedef( update_data ) ;

    struct render_data
    {
        so_gfx::render_2d_ptr_t rnd_2d ;
        so_gfx::text_render_2d_ptr_t txt_rnd ;
        size_t layer_start = 0 ;
        size_t layer_end = 10 ;
    };
    so_typedef( render_data ) ;
}