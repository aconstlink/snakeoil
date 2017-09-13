//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GFX_TEXT_TEXT_RENDER_2D_SHARED_DATA_H_
#define _SNAKEOIL_GFX_TEXT_TEXT_RENDER_2D_SHARED_DATA_H_

#include "../typedefs.h"

#include <snakeoil/font/protos.h>
#include <snakeoil/std/container/vector.hpp>

namespace so_gfx
{
    struct text_render_2d_shared_data
    {
        so_font::glyph_atlas_ptr_t ga ;

        so_math::vec2f_t dim_scale ;

        // we need
        // 1. map for layer to render call instance
        // 2. vector containing all the glyph data

        struct per_group_info
        {
            size_t group_id ;
            size_t num_glyphs ;
        };
        so_typedef( per_group_info ) ;
        so_typedefs( so_std::vector< per_group_info_t >, per_group_infos ) ;
        
        struct glyph_info
        {
            size_t offset ;
            so_math::vec2f_t pos ;
            so_math::vec3f_t color ;
        };
        so_typedef( glyph_info ) ;
        so_typedefs( so_std::vector< glyph_info >, glyph_infos ) ;

    public:

        /// known number of glyphs per layer
        per_group_infos_t per_group_infos ;

        /// all glyph to be rendered in a linear buffer
        glyph_infos_t glyph_infos ;

    };
    so_typedef( text_render_2d_shared_data ) ;
}

#endif