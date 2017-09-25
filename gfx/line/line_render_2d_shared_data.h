//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "../typedefs.h"

#include <snakeoil/std/container/vector.hpp>

namespace so_gfx
{
    struct line_render_2d_shared_data
    {
        struct per_group_info
        {
            size_t group_id ;
            size_t num_lines ;
            so_math::mat4f_t proj ;
            so_math::mat4f_t view ;
        };
        so_typedef( per_group_info ) ;
        so_typedefs( so_std::vector< per_group_info_t >, per_group_infos ) ;

        struct line_info
        {
            /// where to start the line
            so_math::vec2f_t pos0 ;
            /// where to end the line
            so_math::vec2f_t pos1 ;
            /// per line color
            so_math::vec4f_t color ;
        };
        so_typedef( line_info ) ;
        so_typedefs( so_std::vector< line_info >, line_infos ) ;

    public:

        /// known number of glyphs per layer
        per_group_infos_t per_group_infos ;

        /// all lines to be rendered in a linear buffer
        line_infos_t line_infos ;
    };
    so_typedef( line_render_2d_shared_data ) ;
}