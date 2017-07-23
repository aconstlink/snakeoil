//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_COLLIDE_3D_BOUNDS_SPHERE_HPP_
#define _SNAKEOIL_COLLIDE_3D_BOUNDS_SPHERE_HPP_

#include "../../typedefs.h"
#include <snakeoil/math/vector/vector3.hpp>

namespace so_collide 
{
    namespace so_3d
    {
        template< typename type_t >
        class sphere 
        {
            typedef so_math::vector3< type_t > vec_t ;
            typedef vec_t const & vec_cref_t ;
            typedef vec_t & vec_ref_t ;
            
        private:

            type_t _radius ;
            vec_t _center ;

        public:

            sphere( void_t )
            {
                _radius = type_t(0) ;
            }

            sphere( vec_cref_t center, type_t radius )
            {
                _radius = radius ;
                _center = center ;
            }

            bool_t include( vec_cref_t point ) const 
            {
                return (point - get_center()).length2() < get_radius2() ;
            }

        public:

            type_t get_radius( void_t ) const { return _radius ; }
            type_t get_radius2( void_t ) const { return _radius * _radius ; }

            vec_cref_t get_center( void_t ) const { return _center ; }


        };
    }
}

#endif

