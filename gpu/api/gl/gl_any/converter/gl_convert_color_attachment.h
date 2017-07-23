//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_API_GL_GL_ANY_GL_CONVERT_COLOR_ATTACHMENT_H_
#define _SNAKEOIL_GPU_API_GL_GL_ANY_GL_CONVERT_COLOR_ATTACHMENT_H_

#include "../../../../enums/color_targets.h"
#include <snakeoil/core/assert.h>
#include <snakeoil/gli/gl/gl.h>

namespace so_gpu
{
    namespace so_gl
    {
        namespace so_internal
        {
            static const GLenum gl_convert_framebuffer_attachment_type_array[] = 
            {
                GL_COLOR_ATTACHMENT0,	
                GL_COLOR_ATTACHMENT1,
                GL_COLOR_ATTACHMENT2,
                GL_COLOR_ATTACHMENT3,
                GL_COLOR_ATTACHMENT4,
                GL_COLOR_ATTACHMENT5,
                GL_COLOR_ATTACHMENT6,
                GL_COLOR_ATTACHMENT7,
                GL_COLOR_ATTACHMENT8,
                GL_COLOR_ATTACHMENT9,
                GL_COLOR_ATTACHMENT10,
                GL_COLOR_ATTACHMENT11,
                GL_COLOR_ATTACHMENT12,
                GL_COLOR_ATTACHMENT13,
                GL_COLOR_ATTACHMENT14,
                GL_COLOR_ATTACHMENT15,
                0 // corresponds to UBM_ALL.
            } ;
            static const size_t gl_convert_framebuffer_attachment_type_array_size = 
                sizeof(gl_convert_framebuffer_attachment_type_array) / sizeof(gl_convert_framebuffer_attachment_type_array[0]) ;

        }
        
        static inline GLenum convert_color_attachment( size_t const i )
        {
            return so_internal::gl_convert_framebuffer_attachment_type_array[
                i >= so_internal::gl_convert_framebuffer_attachment_type_array_size ? 0 : i 
            ] ;
        }

        static inline GLenum convert( so_gpu::color_target const ct )
        {
            size_t const i = so_gpu::convert_color_target_to_number(ct) ;
            so_assert( i != size_t(-1) ) ;
            return convert_color_attachment(i) ;
        }
    }
}

#endif

