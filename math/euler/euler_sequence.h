//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_MATH_EULER_EULER_SEQUENCE_H_
#define _SNAKEOIL_MATH_EULER_EULER_SEQUENCE_H_

namespace so_math
{
    /// missing classic euler angle sequences
    enum class euler_sequence
    {
        invalid,
        xyz,
        yzx,
        zxy,
        xzy,
        zyx,
        yxz
    };
}

#endif