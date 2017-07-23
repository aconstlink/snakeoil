//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_API_GL_GL_ANY_GL_CONVERT_GEOMETRY_SHADER_TYPE_H_
#define _SNAKEOIL_GPU_API_GL_GL_ANY_GL_CONVERT_GEOMETRY_SHADER_TYPE_H_

#include "../../../../enums/geometry_shader_input_types.h"
#include "../../../../enums/geometry_shader_output_types.h"

#include <snakeoil/gli/gl/gl.h>

namespace so_gpu
{
    namespace so_gl
    {
        namespace so_internal
        {
            static const GLenum convert_geometry_shader_input_types_gl[] =
            {
                0,
                GL_POINTS,
                GL_LINES,
                GL_LINES_ADJACENCY,
                GL_TRIANGLES,
                GL_TRIANGLES_ADJACENCY
            } ;

            static const size_t convert_geometry_shader_input_types_gl_size =
                sizeof( convert_geometry_shader_input_types_gl ) / sizeof( convert_geometry_shader_input_types_gl[ 0 ] ) ;

            static const GLenum convert_geometry_shader_output_types_gl[] =
            {
                0,
                GL_POINTS,
                GL_LINE_STRIP,
                GL_TRIANGLE_STRIP
            } ;

            static const size_t convert_geometry_shader_output_types_gl_size =
                sizeof( convert_geometry_shader_output_types_gl ) / sizeof( convert_geometry_shader_output_types_gl[ 0 ] ) ;

        }

        static GLenum convert( so_gpu::geometry_shader_input_type const t )
        {
            return ( size_t ) t >= so_internal::convert_geometry_shader_input_types_gl_size ?
                0 : so_internal::convert_geometry_shader_input_types_gl[ ( unsigned int ) t ] ;
        }

        static GLenum convert( so_gpu::geometry_shader_output_type const t )
        {
            return ( size_t ) t >= so_internal::convert_geometry_shader_output_types_gl_size ?
                0 : so_internal::convert_geometry_shader_output_types_gl[ ( unsigned int ) t ] ;
        }


    }
}

#endif

