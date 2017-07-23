//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_BUFFER_PIXEL_BUFFER_H_
#define _SNAKEOIL_GPU_BUFFER_PIXEL_BUFFER_H_

#include "buffer.h"

#include <snakeoil/memory/buffer/buffer.hpp>

namespace so_gpu
{
    template< typename pixel_t >
    class pixel_buffer : public so_gpu::buffer
    {
        typedef pixel_buffer< pixel_t > this_t ;

        typedef so_memory::buffer< pixel_t > pixels_t ;

    private:

        pixels_t _pixels ;

    public:



    };
}

#endif

