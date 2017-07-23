//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_MATH_TYPEDEFS_H_
#define _SNAKEOIL_MATH_TYPEDEFS_H_

#include <snakeoil/core/types.hpp>
#include <snakeoil/core/macros/move.h>
#include <snakeoil/core/macros/typedef.h>
#include <snakeoil/core/break_if.hpp>

namespace so_math
{
    using namespace so_core::so_types ;

    namespace so_vector
    {
        struct x_axis{ int d; };
        struct y_axis{ int d; };
        struct z_axis{ int d; };
    }

    namespace so_matrix
    {
        struct with_identity{int d;} ;
        struct xyzw{};
        struct yxzw{};
        struct xzyw{};
        
    }

    namespace so_quaternion
    {
        struct axis_normalized{int d;};
    }
}

#endif

