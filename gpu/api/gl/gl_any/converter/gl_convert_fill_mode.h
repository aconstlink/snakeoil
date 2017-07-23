//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_API_GL_GL_ANY_GL_CONVERT_FILL_MODE_H_
#define _SNAKEOIL_GPU_API_GL_GL_ANY_GL_CONVERT_FILL_MODE_H_

#include "../../../../enums/fill_modes.h"
#include <snakeoil/gli/gl/gl.h>

namespace so_gpu
{
    namespace so_gl
    {
        namespace so_internal
        {
            static const GLenum convert_fill_mode_gl[] = 
            {
                GL_POINT,
                GL_LINE,
                GL_FILL
            } ;
            static const size_t convert_fill_mode_gl_size =
                sizeof ( convert_fill_mode_gl) / sizeof ( convert_fill_mode_gl[0]) ;
        }

        static GLenum convert( fill_mode const fm )
        {
            return so_internal::convert_fill_mode_gl[size_t(fm)] ;
        }
    }
}

#endif

