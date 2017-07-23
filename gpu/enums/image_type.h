//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_ENUMS_IMAGE_TYPE_H_
#define _SNAKEOIL_GPU_ENUMS_IMAGE_TYPE_H_

namespace so_gpu
{
    enum class image_type
    {
        undefined,
        color_2d,
        color_3d,
        depth,
        stencil
    };

}

#endif

