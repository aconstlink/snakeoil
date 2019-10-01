//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "../typedefs.h"

namespace so_math
{
    template< typename T >
    struct constants
    {
        so_this_typedefs( constants<T> ) ;
        so_typedefs( T, type ) ;

        static type_t pi( void_t )
        {
            return type_t( 3.141592653589793238462643383279502884197169399375105 ) ;
        }

        static type_t pix2( void_t )
        {
            return ( type_t(2.0) * this_t::pi() ) ;
        }
    };

}
