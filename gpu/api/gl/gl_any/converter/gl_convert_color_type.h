//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_API_GL_GL_ANY_GL_CONVERT_COLOR_TYPE_H_
#define _SNAKEOIL_GPU_API_GL_GL_ANY_GL_CONVERT_COLOR_TYPE_H_

#include "../../../../enums/color_types.h"
#include <snakeoil/gli/gl/gl.h>

namespace so_gpu
{
    namespace so_gl
    {
        namespace so_internal
        {
            static const GLenum color_types_gl[] = 
            {
                0, // CT_UNDEFINED	
                GL_RGB, // CT_RGB,
                GL_RGBA, // CT_RGBA,
                GL_RED, // CT_INTENSITY,
                GL_RED, // CT_LUMINANCE
                GL_BGR,
                GL_BGRA
            } ;

            static const GLenum color_types_integer_gl[] = 
            {
                0, // CT_UNDEFINED	
                GL_RGB_INTEGER, // CT_RGB,
                GL_RGBA_INTEGER, // CT_RGBA,
                GL_RED_INTEGER, // CT_INTENSITY,
                GL_RED_INTEGER, // CT_LUMINANCE
                GL_BGR_INTEGER,
                GL_BGRA_INTEGER
            } ;

            static const unsigned int num_color_types_gl =
                    sizeof ( color_types_gl) / sizeof ( color_types_gl[0]) ;
        }

        static GLenum convert( color_type const t, so_gpu::type const gfx_type )
        {
            if( gfx_type == so_gpu::type::tint || gfx_type == so_gpu::type::tuint )
                return so_internal::color_types_integer_gl[size_t(t) >= so_internal::num_color_types_gl ? 0 : size_t(t)] ;
            return so_internal::color_types_gl[size_t(t) >= so_internal::num_color_types_gl ? 0 : size_t(t)] ;
        }
    }
}

#endif

