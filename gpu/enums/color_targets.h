//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_USE_BUFFER_MASKS_H_
#define _SNAKEOIL_GPU_USE_BUFFER_MASKS_H_

#include "../typedefs.h"

namespace so_gpu
{
    enum class color_target
    {
        zero, 
        one,
        two,
        three,
        four,
        five,
        six,
        seven,
        eight,
        nine,
        ten,
        eleven,
        twelf,
        thirteen,
        fourteen,
        fifteen,
        none,		// no mask is used.
        all
    } ;

    static color_target convert_number_to_color_target( size_t i )
    {
        return i < size_t(color_target::none) ? color_target(i) : color_target::none ; 
    }

    /// maps a zero-based number to its ubm_X counter part.
    /// e.g
    /// ubm_zero -> 0
    /// ubm_one -> 1 ...
    static size_t convert_color_target_to_number( color_target ct )
    {
        return size_t(ct) < size_t(color_target::none) ? size_t(ct) : -1 ; 
    }

    namespace so_internal
    {
        static std::string const use_buffer_string_array[] = 
        {
            "zero","one","two","three","four",
            "five","six","seven","eight","nine","ten","eleven",
            "twelf","thirteen","fourteen","fifteen","none", "all", "invalid"
        } ;	
    }

    static std::string const & to_string( color_target ct )
    {
        return so_internal::use_buffer_string_array[ size_t(ct)] ;
    }
} 

#endif

