//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_API_GL_GL_ANY_OBJECTS_GL_DRIVER_OBJECTS_IMAGE_H_
#define _SNAKEOIL_GPU_API_GL_GL_ANY_OBJECTS_GL_DRIVER_OBJECTS_IMAGE_H_

#include "../../../api_object.h"

#include <snakeoil/gli/gl/gl.h>

namespace so_gpu
{
    namespace so_gl
    {
        struct image : public api_object
        {
            image( GLuint id_ ) : id(id_) {}
            image( image && rhv ) 
            {
                id = rhv.id ;
                rhv.id = GLuint(-1) ;
            }
                
            GLenum image_target ;
            GLuint id = GLuint(-1) ;

            GLsizei width = 0 ;
            GLsizei height = 0 ;
        };
    }
}

#endif

