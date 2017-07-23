//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_COLLIDE_2D_HIT_TEST_BOX_BOX_HPP_
#define _SNAKEOIL_COLLIDE_2D_HIT_TEST_BOX_BOX_HPP_

#include "../hit_test_types.h"
#include "../bounds/aabb.hpp"

namespace so_collide 
{
    namespace so_2d
    {
		/// @returns
		/// ht_outside
		/// ht_intersect
		template< typename type_t >
		static hit_test_type hit_test_box_box( so_2d::aabb< type_t > const & a, so_2d::aabb< type_t > const & b )
		{
            if( (a.get_min().greater_than(b.get_max())).any() ) return so_intersection::ht_outside ;
            if( (b.get_min().greater_than(a.get_max())).any() ) return so_intersection::ht_outside ;

			return ht_intersect ;
		}

        template< typename type_t >
        static bool_t intersect( so_2d::aabb< type_t > const & a, so_2d::aabb< type_t > const & b )
        {
            return hit_test_box_box(a,b) == ht_intersect ;
        }
    }
}

#endif

