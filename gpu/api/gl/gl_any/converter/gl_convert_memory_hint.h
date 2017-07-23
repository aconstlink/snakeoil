//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_API_GL_GL_ANY_GL_CONVERT_MEMORY_HINT_H_
#define _SNAKEOIL_GPU_API_GL_GL_ANY_GL_CONVERT_MEMORY_HINT_H_

#include "../../../../enums/memory_hints.h"
#include <snakeoil/gli/gl/gl.h>

namespace so_gpu
{
    namespace so_gl
    {
        namespace so_internal 
        {
            static const GLenum convert_memory_hint_gl[] =
            {
                0,
                GL_STREAM_DRAW,
                GL_STREAM_READ,
                GL_STREAM_COPY,
                GL_STATIC_DRAW,
                GL_STATIC_READ,
                GL_STATIC_COPY,
                GL_DYNAMIC_DRAW,
                GL_DYNAMIC_READ,
                GL_DYNAMIC_COPY
            } ;

            static size_t convert_memory_hint_gl_array_size = 
                sizeof(convert_memory_hint_gl) / sizeof(convert_memory_hint_gl[0]) ;
        }

        static GLenum convert( memory_hint const mh )
        {
            return so_internal::convert_memory_hint_gl[
                (size_t)mh >= so_internal::convert_memory_hint_gl_array_size ? 0 : (size_t)mh
            ] ;
        }
    }
}

#endif

