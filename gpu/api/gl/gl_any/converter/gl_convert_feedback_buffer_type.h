//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_API_GL_GL_ANY_GL_CONVERT_FEEDBACK_BUFFER_TYPE_H_
#define _SNAKEOIL_GPU_API_GL_GL_ANY_GL_CONVERT_FEEDBACK_BUFFER_TYPE_H_

#include "../../../../enums/feedback_buffer_types.h"
#include <snakeoil/gli/gl/gl.h>

namespace so_gpu
{
    namespace so_gl
    {
        namespace so_internal
        {
            static const GLenum __feedback_buffer_type_array_gl[] = 
            {
                0,
                GL_INTERLEAVED_ATTRIBS,
                GL_SEPARATE_ATTRIBS
            } ;
        }

        static GLenum convert( feedback_buffer_type const fbt )
        {
            return so_internal::__feedback_buffer_type_array_gl[size_t(fbt)] ;
        }
    }
}

#endif

