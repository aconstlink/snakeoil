//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_API_GL_GL_ANY_GL_CONVERT_TEXTURE_FILTER_H_
#define _SNAKEOIL_GPU_API_GL_GL_ANY_GL_CONVERT_TEXTURE_FILTER_H_

#include "../../../../enums/texture_wraps.h"
#include "../../../../enums/texture_filters.h"
#include <snakeoil/gli/gl/gl.h>

namespace so_gpu
{
    namespace so_gl
    {
        namespace so_internal
        {
            static const GLenum filter_modes_gl[] = {
                GL_TEXTURE_MAG_FILTER,
                GL_TEXTURE_MIN_FILTER
            } ;

            static const GLenum filter_types_gl[] = {
                GL_NEAREST,
                GL_LINEAR
            } ;
        }

        static GLenum convert( texture_filter_mode const m )
        {
            return so_internal::filter_modes_gl[(size_t)m] ;
        }

        static GLenum convert( texture_filter_type const t )
        {
            return so_internal::filter_types_gl[(size_t)t] ;
        }
    }
}

#endif

