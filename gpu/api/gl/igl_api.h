//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_API_GL_IGL_API_H_
#define _SNAKEOIL_GPU_API_GL_IGL_API_H_

#include "../iapi.h"

namespace so_gpu
{
    namespace so_gl
    {
        class igl_api : public iapi
        {
        public:

            virtual so_gpu::result initialize( void_t ) = 0 ;
        };
    }
}

#endif