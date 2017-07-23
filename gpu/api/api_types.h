//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_API_API_TYPES_H_
#define _SNAKEOIL_GPU_API_API_TYPES_H_

#include <snakeoil/std/string/string.hpp>

namespace so_gpu
{
    enum class api_type
    {
        null,
        general,
        gl_20,
        gl_33,
        gl_45,
        gles_20,
        gles_32,
        d3d_10,
        d3d_11,
        d3d_12,
        vulkan,
        custom
    };

    namespace so_internal
    {
        static const so_std::string_t api_type_short_names[] =
        {
            "null", "general", "gl_20", "gl_33", "gl_45", 
            "gles_20", "gles_32", "d3d_10", "d3d_11", "d3d_12",
            "vulkan", "custom"
        } ;

        static const so_std::string_t api_type_long_names[] =
        {
            "NULL", "general", "OpenGL 2.0", "OpenGL 3.3", "OpenGL 4.5",
            "OpenGL ES 2.0", "OpenGL ES 3.2", "Direct3D 10", "Direct3D 11", "Direct3D 12",
            "Vulkan", "Custom API"
        } ;
    }

    static so_std::string_cref_t to_string( api_type const t )
    {
        return so_internal::api_type_short_names[ size_t(t) ] ;
    }

    static so_std::string_cref_t to_long_string( api_type const t )
    {
        return so_internal::api_type_long_names[ size_t( t ) ] ;
    }

    
}

#endif