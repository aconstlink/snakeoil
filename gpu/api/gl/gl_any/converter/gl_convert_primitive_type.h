//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_API_GL_GL_ANY_GL_CONVERT_PRIMITIVE_TYPE_H_
#define _SNAKEOIL_GPU_API_GL_GL_ANY_GL_CONVERT_PRIMITIVE_TYPE_H_

#include "../../../../enums/primitive_types.h"
#include <snakeoil/gli/gl/gl.h>

namespace so_gpu
{
    namespace so_gl
    {
        namespace so_internal 
        {
            static const GLenum convert_prim_type_gl[] = 
            {
                0,
                GL_POINTS,
                //GL_POINT_SPRITE,
                GL_LINES, GL_LINE_STRIP, GL_LINE_LOOP,
                GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN,
                GL_PATCHES
            } ;
            static const size_t convert_prim_type_gl_size = 
                sizeof( convert_prim_type_gl ) / sizeof( convert_prim_type_gl[0] ) ;
        }

        static GLenum convert( primitive_type const pt )
        {
            return so_internal::convert_prim_type_gl[size_t(pt)] ;
        }
    }
}

#endif

