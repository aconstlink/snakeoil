//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_API_GL_GL_ANY_GL_CONTEXT_H_
#define _SNAKEOIL_GPU_API_GL_GL_ANY_GL_CONTEXT_H_

#include "../../../api.h"
#include "../../../typedefs.h"
#include <snakeoil/gli/gl/gl.h>

namespace so_gpu
{
    namespace so_gl
    {
        /// allows to provide an interface to the outside
        /// world for the driver.
        struct SNAKEOIL_GPU_API gl_context
        {
            gl_context( void_t  ) {}
            virtual ~gl_context( void_t ){}
        };
        so_typedef( gl_context ) ;
    }
}

#endif

