//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_API_GL_GL_ANY_GL_CONVERT_FRONT_FACE_H_
#define _SNAKEOIL_GPU_API_GL_GL_ANY_GL_CONVERT_FRONT_FACE_H_

#include "../../../../enums/render_states.h"
#include "../../../../enums/front_faces.h"
#include <snakeoil/gli/gl/gl.h>

namespace so_gpu
{
    namespace so_gl
    {
        namespace so_internal
        {
            static const GLenum convert_front_face_gl[] =
            {
                GL_CW,
                GL_CCW
            } ;
            static const size_t convert_front_face_gl_size =
                sizeof ( convert_front_face_gl ) / sizeof ( convert_front_face_gl[ 0 ] ) ;
        }

        static GLenum convert( front_face const ff )
        {
            return so_internal::convert_front_face_gl[ size_t( ff ) ] ;
        }
    }
}

#endif

