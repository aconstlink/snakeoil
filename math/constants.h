//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_MATH_CONSTANTS_H_
#define _SNAKEOIL_MATH_CONSTANTS_H_

#include "typedefs.h"

namespace so_math
{
    struct constants
    {
        template< typename T >
        static T pi( void_t )
        {
            return T(3.141592653589793238462643383279502884197169399375105) ;
        }
    };
    
}

#endif
