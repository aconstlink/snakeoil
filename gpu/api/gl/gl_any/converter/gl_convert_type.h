//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_API_GL_GL_ANY_GL_CONVERT_TYPE_H_
#define _SNAKEOIL_GPU_API_GL_GL_ANY_GL_CONVERT_TYPE_H_

#include "../../../../enums/types.h"
#include <snakeoil/gli/gl/gl.h>

namespace so_gpu
{
    namespace so_gl
    {
        namespace so_internal 
        {
            static const GLenum convert_types_gl[] = {
                0,					// T_UNDEFINED	
                GL_BYTE,			// T_CHAR,
                GL_UNSIGNED_BYTE,	// T_UCHAR,
                GL_SHORT,			// T_SHORT,
                GL_UNSIGNED_SHORT,	// T_USHORT
                GL_INT,				// T_INT,
                GL_UNSIGNED_INT,	// T_UINT,
                GL_FLOAT,			// T_FLOAT,
                GL_DOUBLE,			// T_DOUBLE,
                GL_BOOL				// T_BOOL
            } ;

            static const unsigned int convert_types_gl_size = 
                sizeof( convert_types_gl ) / sizeof( convert_types_gl[0] ) ;
        }

        static GLenum convert( type const t )
        {
            return so_internal::convert_types_gl[size_t(t)] ;
        }
    }
}

#endif

