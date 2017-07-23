//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_API_GL_GL_ANY_GL_CONVERT_CLEAR_MODE_H_
#define _SNAKEOIL_GPU_API_GL_GL_ANY_GL_CONVERT_CLEAR_MODE_H_

#include "../../../../enums/clear_modes.h"
#include <snakeoil/gli/gl/gl.h>

namespace so_gpu
{
    namespace so_gl
    {
        namespace so_internal
        {
            static const GLenum convert_clear_mode_gl[] = 
            {
                GL_COLOR_BUFFER_BIT,
                GL_DEPTH_BUFFER_BIT,
                GL_STENCIL_BUFFER_BIT
            } ;
            static const size_t num_convert_clear_mode_gl =
                    sizeof ( convert_clear_mode_gl) / sizeof ( convert_clear_mode_gl[0]) ;
        }

        static GLbitfield convert_to_clear_bit( bool const cb, bool const db, bool const sb )
        {
            GLbitfield mode = 0 ;
            mode |= cb ? GL_COLOR_BUFFER_BIT : 0 ;
            mode |= db ? GL_DEPTH_BUFFER_BIT : 0 ;
            mode |= sb ? GL_STENCIL_BUFFER_BIT : 0 ;
            return mode ;
        }
    }
}

#endif

