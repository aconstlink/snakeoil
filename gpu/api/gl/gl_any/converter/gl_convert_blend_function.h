//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_API_GL_GL_ANY_GL_CONVERT_BLEND_FUNCTION_H_
#define _SNAKEOIL_GPU_API_GL_GL_ANY_GL_CONVERT_BLEND_FUNCTION_H_

#include "../../../../enums/blend_functions.h"
#include <snakeoil/gli/gl/gl.h>

namespace so_gpu
{
    namespace so_gl
    {
        namespace so_internal
        {
            static const GLenum convert_blend_func_array_gl[] = 
            {
                GL_FUNC_ADD,
                GL_FUNC_SUBTRACT,
                GL_FUNC_REVERSE_SUBTRACT,
                GL_MIN,
                GL_MAX,
                GL_LOGIC_OP
            } ;

            static const size_t convert_blend_func_array_gl_size = 
                sizeof(convert_blend_func_array_gl) / sizeof(convert_blend_func_array_gl[0]) ;
        }

        static GLenum const & convert( blend_function const bf )
        {
            return so_internal::convert_blend_func_array_gl[size_t(bf)] ;
        }
    }
}

#endif

