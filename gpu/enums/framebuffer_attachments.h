//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_FRAMEBUFFER_ATTACHMENTS_H_
#define _SNAKEOIL_GPU_FRAMEBUFFER_ATTACHMENTS_H_

#include <string>

namespace so_gpu
{
    enum class framebuffer_attachment 
    {
        undefined,
        color,
        depth,
        stencil
    } ;

    namespace so_internal
    {
        static std::string const framebuffer_attachment_type_string_array[] = 
        {
            "color", "depth", "stencil"
        } ;
    }

    static std::string const & to_string( framebuffer_attachment fa )
    {
        return so_internal::framebuffer_attachment_type_string_array[size_t(fa)] ;
    }
    
}

#endif

