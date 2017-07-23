//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_MATH_PRIMITIVE_2D_RAY_HPP_
#define _SNAKEOIL_MATH_PRIMITIVE_2D_RAY_HPP_

#include "../../vector/vector2.hpp"

namespace so_math
{
    namespace so_2d
    {
        template< typename type_t >
        class ray
        {
            typedef so_math::vector2< type_t > vec2_t ;
            typedef vec2_t const & vec2_cref_t ;

        private:

            vec2_t _origin ;
            vec2_t _dir ;

        public:

            ray( void_t ){}

            ray( vec2_cref_t orig, vec2_cref_t dir )
            {
                _origin = orig ;
                _dir = dir ;
            }

            vec2_cref_t get_origin( void_t ) const { return _origin ; }
            vec2_cref_t get_direction( void_t ) const { return _dir ; }

            vec2_t point_at( type_t dist ) const { return _origin + _dir * dist ; }

        };
    }

    typedef so_2d::ray< float > ray2f_t ;
}

#endif

