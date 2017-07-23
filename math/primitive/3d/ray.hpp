//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_MATH_PRIMITIVE_3D_RAY_HPP_
#define _SNAKEOIL_MATH_PRIMITIVE_3D_RAY_HPP_

#include "../../vector/vector3.hpp"

#include <snakeoil/core/types.hpp>

namespace so_math
{
    using namespace so_core::so_types ;

    namespace so_3d
    {
        template< typename type_t >
        class ray
        {
            typedef so_math::vector3< type_t > vec3_t ;
            typedef vec3_t const & vec3_cref_t ;

        private:

            vec3_t _origin ;
            vec3_t _dir ;

        public:

            ray( void_t ){}

            ray( vec3_cref_t orig, vec3_cref_t dir )
            {
                _origin = orig ;
                _dir = dir ;
            }

            vec3_cref_t get_origin( void_t ) const { return _origin ; }
            vec3_cref_t get_direction( void_t ) const { return _dir ; }

            vec3_t point_at( type_t dist ) const { return _origin + _dir * dist ; }
        };
    }

    typedef so_3d::ray< float_t > ray3f_t ;
}

#endif

