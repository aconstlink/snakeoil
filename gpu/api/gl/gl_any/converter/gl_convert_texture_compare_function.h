//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_API_GL_GL_ANY_GL_CONVERT_TEXTURE_COMPARE_FUNCTION_H_
#define _SNAKEOIL_GPU_API_GL_GL_ANY_GL_CONVERT_TEXTURE_COMPARE_FUNCTION_H_

#include "../../../../enums/texture_compare_functions.h"
#include <snakeoil/gli/gl/gl.h>

namespace so_gpu
{
    namespace so_gl
    {
        namespace so_internal
        {
            static const GLenum convert_texture_compare_func_array_gl[] =
            {
                GL_LEQUAL,
                GL_GEQUAL,
                GL_LESS,
                GL_GREATER,
                GL_EQUAL,
                GL_NOTEQUAL,
                GL_ALWAYS,
                GL_NEVER
            } ;

            static const size_t convert_texture_compare_func_array_gl_size =
                sizeof( convert_texture_compare_func_array_gl ) / sizeof( convert_texture_compare_func_array_gl[ 0 ] ) ;
        }

        static GLenum const & convert( texture_compare_function const tcf )
        {
            return so_internal::convert_texture_compare_func_array_gl[ size_t( tcf ) ] ;
        }
    }
}

#endif

