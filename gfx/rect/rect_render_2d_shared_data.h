//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "../typedefs.h"

#include <snakeoil/std/container/vector.hpp>

namespace so_gfx
{
    struct rect_render_2d_shared_data
    {
        struct per_group_info
        {
            size_t group_id ;
            size_t num_rects ;
            so_math::mat4f_t proj ;
            so_math::mat4f_t view ;
        };
        so_typedef( per_group_info ) ;
        so_typedefs( so_std::vector< per_group_info_t >, per_group_infos ) ;

        struct rect_info
        {
            /// center position
            so_math::vec2f_t pos ;
            /// scaling
            so_math::vec2f_t scale ;
            so_math::vec2f_t pivot ;
            /// per rect color
            so_math::vec4f_t color ;
        };
        so_typedef( rect_info ) ;
        so_typedefs( so_std::vector< rect_info >, rect_infos ) ;

    public:

        so_math::mat4f_t proj ;
        so_math::mat4f_t view ;

        /// known number of rects per group
        per_group_infos_t per_group_infos ;

        /// all rects to be rendered in a linear buffer
        rect_infos_t rect_infos ;
    };
    so_typedef( rect_render_2d_shared_data ) ;
}