//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "../typedefs.h"

#include <snakeoil/gpu/viewport/viewport_2d.h>
#include <snakeoil/imex/image/image.h>
#include <snakeoil/std/container/vector.hpp>

namespace so_gfx
{
    struct image_render_2d_shared_data
    {
        struct per_group_info
        {
            size_t group_id ;
            so_math::mat4f_t proj ;
            so_math::mat4f_t view ;

            // all sub-images
            size_t num_images ;

            
        };
        so_typedef( per_group_info ) ;
        so_typedefs( so_std::vector< per_group_info_t >, per_group_infos ) ;

        struct image_info
        {
            so_imex::image_cptr_t img_ptr ;
            so_math::vec4f_t texcoords ;

            bool_t dirty ;

            so_math::vec2f_t pos ;
            so_math::vec2f_t scale ;
            so_math::vec4f_t color ;
            float_t rot ;
            
        };
        so_typedef( image_info ) ;
        so_typedefs( so_std::vector< image_info >, image_infos ) ;

    public:

        per_group_infos_t per_group_infos ;
        image_infos_t image_infos ;
    };
    so_typedef( image_render_2d_shared_data ) ;
}