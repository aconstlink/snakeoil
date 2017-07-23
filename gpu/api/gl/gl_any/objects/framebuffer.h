//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_API_GL_GL_ANY_OBJECTS_GL_DRIVER_OBJECTS_FRAMEBUFFER_H_
#define _SNAKEOIL_GPU_API_GL_GL_ANY_OBJECTS_GL_DRIVER_OBJECTS_FRAMEBUFFER_H_

#include "../../../api_object.h"

#include <snakeoil/gli/gl/gl.h>

namespace so_gpu
{
    namespace so_gl
    {
        struct framebuffer : public api_object
        {
            framebuffer( GLuint id_ ) : id( id_ ){}
            framebuffer( framebuffer && rhv )
            {
                id = rhv.id ;
                rhv.id = GLuint( -1 ) ;
            }

            GLuint id = GLuint( -1 ) ;
        };
    }
}

#endif

