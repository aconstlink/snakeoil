//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_CONVERTER_CONVERT_TYPE_TO_IMAGE_FORMAT_H_
#define _SNAKEOIL_GPU_CONVERTER_CONVERT_TYPE_TO_IMAGE_FORMAT_H_

#include "../enums/image_formats.h"
#include "../enums/types.h"

namespace so_gpu
{
    static image_format convert_type_to_image_format( so_gpu::type_struct ts )
    {
        switch( ts )
        {
        case so_gpu::type_struct::vec1:
            return image_format::intensity ;
        case so_gpu::type_struct::vec3:
            return image_format::rgb;
        case so_gpu::type_struct::vec4:
            return image_format::rgba;
        default: return image_format::undefined ;
        }
        return image_format::undefined ;
    }
}

#endif

