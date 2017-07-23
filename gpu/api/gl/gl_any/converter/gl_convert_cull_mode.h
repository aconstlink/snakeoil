//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_API_GL_GL_ANY_GL_CONVERT_CULL_MODE_H_
#define _SNAKEOIL_GPU_API_GL_GL_ANY_GL_CONVERT_CULL_MODE_H_

#include "../../../../enums/cull_modes.h"
#include "../../../../enums/render_states.h"
#include <snakeoil/gli/gl/gl.h>

namespace so_gpu
{
    namespace so_gl
    {
        namespace so_internal
        {
            static const GLenum convert_cull_mode_gl[] = 
            {
                GL_FRONT,
                GL_BACK,
                GL_FRONT_AND_BACK
            } ;

            static const size_t convert_cull_mode_gl_size =
                    sizeof ( convert_cull_mode_gl) / sizeof ( convert_cull_mode_gl[0]) ;
        }

        static GLenum convert( cull_mode const cm )
        {
            return so_internal::convert_cull_mode_gl[size_t(cm)] ;
        }
    }
}

#endif

