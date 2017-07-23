//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GFX_CONVERT_PRIMITIVE_TYPE_TO_GS_TYPE_H_
#define _SNAKEOIL_GFX_CONVERT_PRIMITIVE_TYPE_TO_GS_TYPE_H_

#include "../enums/primitive_types.h"
#include "../enums/geometry_shader_input_types.h"
#include "../enums/geometry_shader_output_types.h"

using namespace so_gfx::so_enums ;

namespace so_gfx
{
    namespace so_enums
    {
        static geometry_shader_input_type convert_to_gs_input_type( so_enums::primitive_type pt )
        {
            switch( pt )
            {
            case pt_undefined: return gsit_undefined ;
            case pt_points: return gsit_points ;
            case pt_point_sprite: return gsit_points ;
            case pt_lines: return gsit_lines ;
            case pt_line_strip: return gsit_lines ;
            case pt_line_loop: return gsit_lines ;
            case pt_triangles: return gsit_triangles ;
            case pt_triangle_strip: return gsit_triangles ;
            case pt_triangle_fan:return gsit_triangles ;
            case pt_patches: return gsit_undefined ;
            default: return gsit_undefined ;
            }

            return gsit_undefined ;
        }

        static geometry_shader_output_type convert_to_gs_output_type( so_enums::primitive_type pt )
        {
            switch( pt )
            {
            case pt_undefined: return gsot_undefined;
            case pt_points: return gsot_points ;
            case pt_point_sprite: return gsot_points ;
            case pt_lines: return gsot_line_strip ;
            case pt_line_strip: return gsot_line_strip ;
            case pt_line_loop: return gsot_line_strip ;
            case pt_triangles: return gsot_triangle_strip ;
            case pt_triangle_strip: return gsot_triangle_strip ;
            case pt_triangle_fan:return gsot_triangle_strip ;
            case pt_patches: return gsot_undefined ;
            default: return gsot_undefined ;
            }

            return gsot_undefined ;
        }
    }
    
}

#endif

