//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_API_GL_GL_ANY_OBJECTS_GL_DRIVER_OBJECTS_FEEDBACK_BUFFER_H_
#define _SNAKEOIL_GPU_API_GL_GL_ANY_OBJECTS_GL_DRIVER_OBJECTS_FEEDBACK_BUFFER_H_

#include "../../../api_object.h"

#include <snakeoil/gli/gl/gl.h>

namespace so_gpu
{
    namespace so_gl
    {
        struct feedback_buffer : public api_object
        {
            feedback_buffer( GLuint id_, GLuint qid_ ) : id( id_ ), qid(qid_) {}
            feedback_buffer( feedback_buffer && rhv ) 
            {
                id = rhv.id ;
                rhv.id = GLuint(-1) ;
                qid = rhv.qid ;
                rhv.qid = GLuint(-1) ;
            }
                
            /// the feedback buffer's id
            GLuint id = GLuint(-1) ;

            /// for querying the capture status
            GLuint qid = GLuint(-1) ;
        };
    }
}

#endif

