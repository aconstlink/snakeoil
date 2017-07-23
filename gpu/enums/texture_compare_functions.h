//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_TEXTURE_COMPARE_FUNCTIONS_H_
#define _SNAKEOIL_GPU_TEXTURE_COMPARE_FUNCTIONS_H_

namespace so_gpu
{
    enum class texture_compare_function
    {
        lequal,
        gequal,
        less,
        greater,
        equal,
        notequal,
        always,
        never
    } ;
}

#endif

