//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_COLLIDE_3D_HIT_TEST_AABB_POINT_HPP__
#define _SNAKEOIL_COLLIDE_3D_HIT_TEST_AABB_POINT_HPP__

#include "../../hit_test_types.h"

#include "../bounds/aabb.hpp"

#include <snakeoil/math/planar/plane/hesse_plane3.hpp>

namespace so_collide 
{
    namespace so_3d
    {
        /// test if a point is inside the box.
        /// @return
        /// ht_outside: point is not in the box
        /// ht_inside: point is in the box
        template< typename type_t >
        static hit_test_type hit_test_box_point( aabb< type_t > const & box, so_math::vector3< type_t > const & point )
        {
            typedef so_math::vector3< type_t > vec3_t ;

			vec3_t point_a = (point-box.get_center()).absed() ;
			return point_a < box.get_extend() ? ht_inside : ht_outside ;
        }

    }
}

#endif

