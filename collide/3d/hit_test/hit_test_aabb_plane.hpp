//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_COLLIDE_3D_HIT_TEST_BOX_PLANE_HPP_
#define _SNAKEOIL_COLLIDE_3D_HIT_TEST_BOX_PLANE_HPP_

#include "../../hit_test_types.h"

#include "../bounds/aabb.hpp"

#include <snakeoil/math/planar/plane/hesse_plane3.hpp>

namespace so_collide 
{
    namespace so_3d
    {
        using so_3d::aabb ;

        /// hit tests box and plane in hesse form
        /// @return
        /// ht_outside: the box is in the negative plane space. the box is behind of the plane
        /// ht_inside: the box is in the positive plane space. the box is in front of the plane
        /// ht_intersect: the box and the plane intersect
        template< typename type_t >
        static hit_test_type hit_test_box_plane( aabb< type_t > const & box, hesse_plane<type_t, 3> const & hesse )
        {
            typedef so_math::vector3< type_t > vec3_t ;

            const vec3_t center = (box.get_max() + box.get_min()) * type_t(0.5) ;
            const vec3_t half = (box.get_max() - box.get_min()) * type_t(0.5) ;
            const vec3_t abs_normal = hesse.get_abs_normal() ;
            type_t e = half.dot(abs_normal) ;
            type_t s = hesse.distance(center) ;
            if( s - e > type_t(0) ) return ht_outside ;
            if( s + e < type_t(0) ) return ht_inside ;
            return ht_intersect ;            
        }

    }
}

#endif

