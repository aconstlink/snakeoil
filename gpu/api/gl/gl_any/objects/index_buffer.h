//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_API_GL_GL_ANY_OBJECTS_GL_DRIVER_OBJECTS_INDEX_BUFFER_H_
#define _SNAKEOIL_GPU_API_GL_GL_ANY_OBJECTS_GL_DRIVER_OBJECTS_INDEX_BUFFER_H_

#include "../../../api_object.h"
#include <snakeoil/gli/gl/gl.h>

namespace so_gpu
{
    namespace so_gl
    {
        struct index_buffer : public api_object
        {
            index_buffer( GLuint id_ ) : id( id_ ) {}
            index_buffer( index_buffer && rhv )
            {
                id = rhv.id ;
                rhv.id = GLuint( -1 ) ;
            }

            GLuint id = GLuint( -1 ) ;
            void_ptr_t mapped_ptr = nullptr ;
        };
    }
}

#endif

