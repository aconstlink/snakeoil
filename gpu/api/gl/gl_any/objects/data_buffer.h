//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_API_GL_GL_ANY_OBJECTS_GL_DRIVER_OBJECTS_DATA_BUFFER_H_
#define _SNAKEOIL_GPU_API_GL_GL_ANY_OBJECTS_GL_DRIVER_OBJECTS_DATA_BUFFER_H_

#include "../../../api_object.h"
#include <snakeoil/gli/gl/gl.h>

namespace so_gpu
{
    namespace so_gl
    {
        struct data_buffer : public api_object
        {
            data_buffer( GLuint buffer_id, GLuint texture_id ) : 
                bid(buffer_id), tid(texture_id) {}

            data_buffer( data_buffer && rhv ) 
            {
                bid = rhv.bid ;
                rhv.bid = GLuint(-1) ;
                tid = rhv.tid ;
                rhv.tid = GLuint(-1) ;
            }

            GLuint bid = GLuint(-1) ;
            GLuint tid = GLuint(-1) ;
        };
    }
}

#endif

