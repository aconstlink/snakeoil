//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_MEMORY_HINT_H_
#define _SNAKEOIL_GPU_MEMORY_HINT_H_

#include <string>

namespace so_gpu
{
    enum class memory_hint
    {
        undefined,
        stream_draw,
        stream_read,
        stream_copy,
        static_draw,
        static_read,
        static_copy,
        dynamic_draw,
        dynamic_read,
        dynamic_copy
    } ;

    namespace so_internal
    {
        static std::string memory_hint_array[] = 
        {
            "undefined","stream_draw","stream_read",
            "stream_copy","static_draw","static_read","static_copy",
            "dynamic_draw","dynamic_read","dynamic_copy"
        } ;
    }

    static std::string const & to_string( memory_hint mh )
    {
        return so_internal::memory_hint_array[size_t(mh)] ;
    }
    
}
#endif

