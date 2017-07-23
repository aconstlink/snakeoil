//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_MATH_UTILITY_DEGREE_HPP_
#define _SNAKEOIL_MATH_UTILITY_DEGREE_HPP_

#include "angle.hpp"

namespace so_math
{    
    template< typename T >
    class degree
    {
        so_this_typedefs( degree<T> ) ;
        so_typedefs( T, type ) ;

        so_typedefs( angle<T>, angle ) ;

    public:

        static type_t val_to_radian( type_t const degree )
        {
            return degree * so_math::constants::pi<type_t>() / type_t( 180 ) ;
        }

        static angle_t to_radian( type_t const deg )
        {
            return angle_t( this_t::val_to_radian(deg) ) ;
        }
            
    };    
}

#endif
