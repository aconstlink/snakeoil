//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_DRIVER_GL_CONVERT_TIMER_TYPE_H_
#define _SNAKEOIL_GPU_DRIVER_GL_CONVERT_TIMER_TYPE_H_

#include "../../../../enums/timer_types.h"
#include <snakeoil/gli/gl/gl.h>

namespace so_gpu
{
    namespace so_gl
    {
        namespace so_internal
        {
            static const GLint __timer_types_array_gl[] = {
                0,
                GL_TIME_ELAPSED,
                GL_TIMESTAMP
            } ;

            static const unsigned int __timer_types_array_size_gl =
                sizeof( __timer_types_array_gl ) / sizeof( __timer_types_array_gl[ 0 ] ) ;
        }

        static GLint convert( timer_query_type const tt )
        {
            return so_internal::__timer_types_array_gl[ ( size_t ) tt ] ;
        }
    }
}

#endif

