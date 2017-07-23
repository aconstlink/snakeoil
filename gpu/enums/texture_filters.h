//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_TEXTURE_FILTERS_H_
#define _SNAKEOIL_GPU_TEXTURE_FILTERS_H_

namespace so_gpu
{
    enum class texture_filter_mode
    {
        mag_filter, 
        min_filter
    } ;

    enum class texture_filter_type
    {
        nearest,
        linear
    } ;
}

#endif

