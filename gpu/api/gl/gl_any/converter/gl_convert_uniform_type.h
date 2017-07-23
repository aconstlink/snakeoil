//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_API_GL_GL_ANY_GL_CONVERT_UNIFORM_TYPE_H_
#define _SNAKEOIL_GPU_API_GL_GL_ANY_GL_CONVERT_UNIFORM_TYPE_H_

#include "../../../../enums/shader_variable_types.h"
#include <snakeoil/gli/gl/gl.h>

namespace so_gpu
{
    namespace so_gl
    {
        namespace so_internal
        {
            static const GLenum convert_uniform_types_gl[] = 
            {
                0,
                GL_FLOAT,
                GL_FLOAT_VEC2,
                GL_FLOAT_VEC3,
                GL_FLOAT_VEC4,
                GL_INT,
                GL_INT_VEC2,
                GL_INT_VEC3,
                GL_INT_VEC4,
                GL_UNSIGNED_INT,
                GL_UNSIGNED_INT_VEC2,
                GL_UNSIGNED_INT_VEC3,
                GL_UNSIGNED_INT_VEC4,
                GL_BOOL,
                GL_BOOL_VEC2,
                GL_BOOL_VEC3,
                GL_BOOL_VEC4,
                GL_FLOAT_MAT2,
                GL_FLOAT_MAT3,
                GL_FLOAT_MAT4,
                GL_SAMPLER_1D,
                GL_SAMPLER_2D,
                GL_SAMPLER_3D,
                GL_INT_SAMPLER_1D,
                GL_INT_SAMPLER_2D,
                GL_INT_SAMPLER_3D,
                GL_UNSIGNED_INT_SAMPLER_1D,
                GL_UNSIGNED_INT_SAMPLER_2D,
                GL_UNSIGNED_INT_SAMPLER_3D,
                GL_SAMPLER_CUBE,
                GL_SAMPLER_1D_SHADOW,
                GL_SAMPLER_2D_SHADOW,
                GL_SAMPLER_BUFFER,
                GL_INT_SAMPLER_BUFFER,
                GL_UNSIGNED_INT_SAMPLER_BUFFER,
                GL_SAMPLER_2D_ARRAY,
                GL_INT_SAMPLER_2D_ARRAY,
                GL_UNSIGNED_INT_SAMPLER_2D_ARRAY
            } ;

            static const size_t convert_uniform_types_gl_size =
                    sizeof ( convert_uniform_types_gl) / sizeof ( convert_uniform_types_gl[0]) ;
        }

        static GLenum convert( so_gpu::shader_variable_type const t )
        {
            return size_t(t) >= so_internal::convert_uniform_types_gl_size ?
                    0 : so_internal::convert_uniform_types_gl[size_t(t)] ;
        }

        static shader_variable_type convert_uniform_type( GLenum const gl_uniform_type )
        {
            size_t i = 0 ;
            for( ; i < so_internal::convert_uniform_types_gl_size ; ++i )
            {
                if( gl_uniform_type == so_internal::convert_uniform_types_gl[i] ) break ;
            }
            return i < so_internal::convert_uniform_types_gl_size ?
                    shader_variable_type(i) : shader_variable_type(0) ;
        }
    }
}

#endif

