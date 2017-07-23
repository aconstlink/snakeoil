//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_API_GL_AL_ANY_GL_CONVERTER_ATTRIBUTE_BIT_H_
#define _SNAKEOIL_GPU_API_GL_AL_ANY_GL_CONVERTER_ATTRIBUTE_BIT_H_

#include "../../../../enums/attribute_state.h"
#include <snakeoil/gli/gl/gl.h>

namespace so_gpu
{
    namespace so_gl
    {
        namespace so_internal
        {
            static const GLbitfield convert_attribute_bit_gl[] = {
                0,                      // psa_none,
                GL_COLOR_BUFFER_BIT,    // psa_color_buffer,
                GL_DEPTH_BUFFER_BIT,    // psa_depth_buffer,
                GL_STENCIL_BUFFER_BIT,  // psa_stencil_buffer,
                GL_ENABLE_BIT,          // psa_enable,
                GL_POLYGON_BIT,         // psa_polygon,
                GL_VIEWPORT_BIT,        // psa_viewport,
                GL_ALL_ATTRIB_BITS      // psa_all
            } ;

            static const size_t convert_attribute_bit_gl_size = 
                sizeof(convert_attribute_bit_gl) / sizeof(convert_attribute_bit_gl[0]) ;
        }

        static GLbitfield const & convert( attribute_state const as )
        {
            return so_internal::convert_attribute_bit_gl[size_t(as)] ;
        }
    }
}

#endif

