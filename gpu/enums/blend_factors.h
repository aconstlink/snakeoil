//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_BLEND_FACTORS_H_
#define _SNAKEOIL_GPU_BLEND_FACTORS_H_

namespace so_gpu
{
    enum class blend_factor
    {
        zero,
        one, 
        src_color,
        one_minus_src_color,
        dest_color,
        one_minus_dst_color,
        src_alpha,
        one_minus_src_alpha,
        dst_alpha,
        one_minus_dst_alpha,
        const_color,
        one_minus_const_color,
        const_alpha,
        one_minus_cost_alpha,
        src_alpha_saturate
    } ;    
}

#endif

