//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_FEEDBACK_BUFFER_TYPES_H_
#define _SNAKEOIL_GPU_FEEDBACK_BUFFER_TYPES_H_

#include <string>

namespace so_gpu
{
    enum class feedback_buffer_type
    {
        undefined,
        interleaved,
        separate
    } ;

    namespace so_internal
    {
        static std::string const __feedback_buffer_type_string_array[] = 
        {
            "undefined", 
            "interleaved", 
            "separate"
        } ;
    }

    static std::string const & to_string( feedback_buffer_type fbt )
    {
        return so_internal::__feedback_buffer_type_string_array[size_t(fbt)] ;
    }    
}

#endif

