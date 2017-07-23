//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_API_GL_GL_ANY_OBJECTS_GL_DRIVER_OBJECTS_VERTEX_BUFFER_H_
#define _SNAKEOIL_GPU_API_GL_GL_ANY_OBJECTS_GL_DRIVER_OBJECTS_VERTEX_BUFFER_H_

#include "../../../api_object.h"
#include <snakeoil/gli/gl/gl.h>

namespace so_gpu
{
    namespace so_gl
    {
        struct vertex_buffer : public api_object
        {
            vertex_buffer( GLuint id_ ) : id(id_) {}
            vertex_buffer( vertex_buffer && rhv ) 
            {
                id = rhv.id ;
                rhv.id = GLuint(-1) ;

                sib = rhv.sib ;
                rhv.sib = 0 ;
            }
                

            GLuint id = GLuint(-1) ;
            void_ptr_t mapped_ptr = nullptr ;

            /// allocated sib
            size_t sib = 0 ;
        };
    }
}

#endif

