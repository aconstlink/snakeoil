//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_CLEAR_MODES_H_
#define _SNAKEOIL_GPU_CLEAR_MODES_H_

#include <string>

namespace so_gpu
{
    enum class clear_mode
    {
        color_buffer = 1 ,
        depth_buffer = 2 ,
        stencil_buffer = 4
    } ;

    namespace so_internal
    {
        static const std::string __clear_mode_string_array[] = {
            "clear_mode"
        } ;
    }

    static std::string const & to_string( clear_mode /*cm*/ )
    {
        return so_internal::__clear_mode_string_array[ 0 ] ;
    }
    
}

#endif

