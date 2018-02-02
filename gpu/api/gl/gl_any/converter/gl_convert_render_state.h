//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_GL_API_GL_GL_ANY_CONVERT_RENDER_STATE_H_
#define _SNAKEOIL_GPU_GL_API_GL_GL_ANY_CONVERT_RENDER_STATE_H_

#include "../../../../enums/render_states.h"
#include <snakeoil/gli/gl/gl.h>

namespace so_gpu
{
    namespace so_gl
    {
        namespace so_internal
        {
            static const GLenum convert_render_state_gl[] = 
            {
                0, // RS_UNDEFINED
                0, // RS_CLEARSCREEN
                GL_CULL_FACE, // RS_CULLMODE
                0, // RS_FRONTFACE
                0, // RS_FILLMODE
                GL_BLEND, // RS_BLEND
                GL_DEPTH_TEST, // RS_DEPTH_TEST
                0, // RS_DEPTH_WRITE
                GL_SCISSOR_TEST, // RS_DEPTH_TEST
            } ;

            static const size_t num_convert_render_state_gl =
                sizeof ( convert_render_state_gl) / sizeof ( convert_render_state_gl[0]) ;
        }

        static GLenum convert( so_gpu::render_state const rs )
        {
            return so_internal::convert_render_state_gl[size_t(rs)] ;
        }
    }
}

#endif

