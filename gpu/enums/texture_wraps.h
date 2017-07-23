//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_TEXTURE_WRAPS_H_
#define _SNAKEOIL_GPU_TEXTURE_WRAPS_H_

namespace so_gpu
{
    enum class texture_wrap_mode
    {
        wrap_s,
        wrap_t, 
        wrap_r
    } ;

    enum class texture_wrap_type
    {
        clamp,
        repeat,
        clamp_border,
        clamp_edge,
        mirror
    } ;    
}

#endif

