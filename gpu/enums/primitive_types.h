//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_PRIMITIVE_TYPES_H_
#define _SNAKEOIL_GPU_PRIMITIVE_TYPES_H_

#include <string>

namespace so_gpu
{
    enum class primitive_type
    {
        undefined,
        points,
        //point_sprite,
        lines,
        line_strip,
        line_loop,
        triangles,
        triangle_strip,
        triangle_fan,
        patches
    } ;

    namespace so_internal
    {
        static const std::string primitive_type_string_array[] = 
        {
            "undefined", "points", /*"point_sprite",*/ "lines", "line_strip",
            "line_loop", "triangles", "triangle_strip", "triangle_fan","patches"
        } ;

        static const size_t vertex_count_for_primitive_type[] = 
        {
            0,
            1,
            1,
            2,
            2,
            2,
            3,
            3,
            3,
            0
        } ;
    }

    static std::string const & to_string( primitive_type pt )
    {
        return so_internal::primitive_type_string_array[size_t(pt)] ;
    }

    static size_t num_vertices( primitive_type pt )
    {
        return so_internal::vertex_count_for_primitive_type[size_t(pt)] ;
    }
} 

#endif

