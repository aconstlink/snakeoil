//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_ENUMS_DATA_BUFFER_FORMATS_H_
#define _SNAKEOIL_GPU_ENUMS_DATA_BUFFER_FORMATS_H_

#include <string>

namespace so_gpu
{
    enum class data_buffer_format
    {
        undefined,
        intensity8,
        intensity16,
        intensity16f,
        intensity32f,
        intensity8i,
        intensity16i,
        intensity32i,
        intensity8ui,
        intensity16ui,
        intensity32ui,
        rgba8,
        rgba16,
        rgba16f,
        rgba32f,
        rgba8i,
        rgba16i,
        rgba32i,
        rgba8ui,
        rgba16ui,
        rgba32ui
    } ;

    namespace so_internal
    {
        static const std::string data_buffer_format_string_array[] = 
        {
            "undefined", 
            "intensity8", "intensity16","intensity16f","intensity32f",
            "intensity8i","intensity16i","intensity32i",
            "intensity8ui",	"intensity16ui","intensity32ui","rgba8",
            "rgba16","rgba16f","rgba32f","rgba8i",
            "rgba16i","rgba32i","rgba8ui","rgba16ui","rgba32ui"
        } ;
    }

    static std::string const & to_string( data_buffer_format tbf )
    {
        return so_internal::data_buffer_format_string_array[size_t(tbf)] ;
    }
    
}

#endif

