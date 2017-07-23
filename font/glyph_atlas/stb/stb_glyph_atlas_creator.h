//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_FONT_GLYPH_ATLAS_STB_STB_GLYPH_ATLAS_CREATOR_H_
#define _SNAKEOIL_FONT_GLYPH_ATLAS_STB_STB_GLYPH_ATLAS_CREATOR_H_

#include "../glyph_atlas.h"

#include <snakeoil/io/typedefs.h>

namespace so_font
{
    namespace so_stb
    {
        struct SNAKEOIL_FONT_API glyph_atlas_creator
        {
            so_this_typedefs( glyph_atlas_creator ) ;

        public:

            typedef std::vector< std::pair< size_t, so_io::path_t>  > __id_and_paths_t ;
            so_typedefs( __id_and_paths_t, id_and_paths ) ;
            so_typedefs( so_std::vector< utf32_t >, code_points ) ;

            struct face_info
            {
                size_t id ;
                so_io::path_t path ;
            };
            so_typedefs( so_std::vector< face_info >, face_infos ) ;

            static glyph_atlas_ptr_t create_glyph_atlas( face_infos_cref_t, size_t const point_size,
                code_points_cref_t, size_t const max_width, size_t const max_height ) ;



        };
        so_typedef( glyph_atlas_creator ) ;
    }
}

#endif