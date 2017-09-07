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


        // we need
        // 1. map for layer to render call instance
        // 2. vector containing all the glyph data

        struct per_layer_info
        {
            size_t layer_id ;
            size_t num_glyphs ;
        };
        so_typedef( per_layer_info ) ;
        so_typedefs( so_std::vector< per_layer_info_t >, per_layer_infos ) ;
        
        struct glyph_info
        {
            size_t offset ;
            so_math::vec2f_t pos ;
            so_math::vec3f_t color ;
        };
        so_typedef( glyph_info ) ;
        so_typedefs( so_std::vector< glyph_info >, glyph_infos ) ;

    public:

        /// knowns number of glyphs per layer
        per_layer_infos_t per_layer_infos ;

        /// all glyph to be rendered in a linear buffer
        glyph_infos_t glyph_infos ;

    };
    so_typedef( text_render_2d_shared_data ) ;
}

#endif