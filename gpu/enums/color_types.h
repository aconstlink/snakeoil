//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_COLOR_TYPES_H_
#define _SNAKEOIL_GPU_COLOR_TYPES_H_

#include "types.h"

#include <string>

namespace so_gpu
{
    enum class color_type
    {
        undefined,
        rgb,
        rgba,
        intensity,
        luminance,
        bgr,
        bgra
    } ;

    namespace so_internal
    {
        static std::string const color_type_string_array[] = 
        {
            "undefined","rgb","rgba","intensity","luminance","bgr","bgra"
        } ;

        static type_struct const ts_for_color_type_array[] = 
        {
            type_struct::undefined, 
            type_struct::vec3, 
            type_struct::vec4,
            type_struct::vec1,
            type_struct::vec1,
            type_struct::vec3,
            type_struct::vec4
        } ;
    }
        
    static std::string const & to_string( color_type ct )
    {
        return so_internal::color_type_string_array[size_t(ct)] ;
    }

    static type_struct color_type_to_type_struct( color_type ct )
    {
        return so_internal::ts_for_color_type_array[size_t(ct)] ;
    }
}

#endif

