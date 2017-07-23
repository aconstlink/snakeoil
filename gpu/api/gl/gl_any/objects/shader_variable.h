//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_API_GL_GL_ANY_OBJECTS_GL_DRIVER_OBJECTS_SHADER_VARIABLE_H_
#define _SNAKEOIL_GPU_API_GL_GL_ANY_OBJECTS_GL_DRIVER_OBJECTS_SHADER_VARIABLE_H_

#include "../../../api_object.h"
#include <snakeoil/gli/gl/gl.h>

namespace so_gpu
{
    namespace so_gl
    {
        struct shader_variable : public api_object
        {
            shader_variable( GLuint id_ ) : id(id_) {}
            shader_variable( shader_variable && rhv ) 
            {
                id = rhv.id ;
                rhv.id = GLuint(-1) ;
            }

            /// it is actually the location.
            GLuint id = GLuint(-1) ;
        };
    }
}

#endif

