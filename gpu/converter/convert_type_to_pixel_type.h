//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_CONVERTER_CONVERT_TYPE_TO_PIXEL_TYPE_H_
#define _SNAKEOIL_GPU_CONVERTER_CONVERT_TYPE_TO_PIXEL_TYPE_H_

#include "../enums/types.h"
#include "../enums/pixel_type.h"

namespace so_gpu
{
    static pixel_type convert_type_to_pixel_type( so_gpu::type t )
    {
        switch( t )
        {
        case type::undefined: return pixel_type::undefined ;
        case type::tshort: return pixel_type::tshort ;
        case type::tushort: return pixel_type::tushort ;
        case type::tchar: return pixel_type::tbyte ;
        case type::tuchar: return pixel_type::tubyte ;
        case type::tuint: return pixel_type::tuint ;
        case type::tint: return pixel_type::tint ;
        case type::tfloat: return pixel_type::tfloat ;
        default: return pixel_type::undefined ;
        }

        return pixel_type::undefined ;
    }
}

#endif

