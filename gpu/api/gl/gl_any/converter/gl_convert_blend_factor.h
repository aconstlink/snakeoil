//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_API_GL_GL_ANY_GL_CONVERT_BLEND_FACTOR_H_
#define _SNAKEOIL_GPU_API_GL_GL_ANY_GL_CONVERT_BLEND_FACTOR_H_

#include "../../../../enums/blend_factors.h"
#include <snakeoil/gli/gl/gl.h>

namespace so_gpu
{
    namespace so_gl
    {
        namespace so_internal
        {
            static const GLenum convert_blend_factor_gl[] = 
            {
                GL_ZERO,
                GL_ONE, 
                GL_SRC_COLOR,
                GL_ONE_MINUS_SRC_COLOR,
                GL_DST_COLOR,
                GL_ONE_MINUS_DST_COLOR,
                GL_SRC_ALPHA,
                GL_ONE_MINUS_SRC_ALPHA,
                GL_DST_ALPHA,
                GL_ONE_MINUS_DST_ALPHA,
                GL_CONSTANT_COLOR,
                GL_ONE_MINUS_CONSTANT_COLOR,
                GL_CONSTANT_ALPHA,
                GL_ONE_MINUS_CONSTANT_ALPHA,
                GL_SRC_ALPHA_SATURATE
            } ;

            static const size_t convert_blend_factors_gl_size = 
                sizeof(convert_blend_factor_gl) / sizeof(convert_blend_factor_gl[0]) ;
        }

        static GLenum const & convert( blend_factor const bf )
        {
            return so_internal::convert_blend_factor_gl[size_t(bf)] ;
        }
    }
}

#endif

