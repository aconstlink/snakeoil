//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_API_GL_GL_ANY_GL_CONVERT_TEXTURE_WRAP_H_
#define _SNAKEOIL_GPU_API_GL_GL_ANY_GL_CONVERT_TEXTURE_WRAP_H_

#include "../../../../enums/texture_wraps.h"
#include <snakeoil/gli/gl/gl.h>

namespace so_gpu
{
    namespace so_gl
    {
        namespace so_internal
        {
            static const GLenum wrap_modes_gl[] = {
                GL_TEXTURE_WRAP_S,
                GL_TEXTURE_WRAP_T,
                GL_TEXTURE_WRAP_R
            } ;

            static const GLenum wrap_types_gl[] = {
                GL_CLAMP_TO_BORDER,
                GL_REPEAT,
                GL_CLAMP_TO_BORDER,
                GL_CLAMP_TO_EDGE,
                GL_MIRRORED_REPEAT
            } ;
        }

        static GLenum convert( texture_wrap_mode const m ){
            return so_internal::wrap_modes_gl[(size_t)m] ;
        }

        static GLenum convert( texture_wrap_type const t ){
            return so_internal::wrap_types_gl[(size_t)t] ;
        }
    }
}

#endif

