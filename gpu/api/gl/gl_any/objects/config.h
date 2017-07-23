//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_API_GL_GL_ANY_OBJECTS_GL_DRIVER_OBJECTS_GEOMETRY_H_
#define _SNAKEOIL_GPU_API_GL_GL_ANY_OBJECTS_GL_DRIVER_OBJECTS_GEOMETRY_H_

#include "../../../api_object.h"

#include <snakeoil/gli/gl/gl.h>
#include <snakeoil/std/container/vector.hpp>

namespace so_gpu
{
    namespace so_gl
    {
        struct config : public api_object
        {
            config( GLuint id_ ) : id(id_) {}
            config( config && rhv ) 
            {
                id = rhv.id ;
                rhv.id = GLuint(-1) ;
            }

            GLuint id = GLuint(-1) ;

            /// we store all enabled vertex attribute locations.
            /// if the program config changes, these locations 
            /// need to be disabled before the new one are enabled.
            so_std::vector< GLuint > locations ;
        };
    }
}

#endif

