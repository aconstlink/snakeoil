//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_COLLIDE_2D_HIT_TEST_BOX_BOX_HPP_
#define _SNAKEOIL_COLLIDE_2D_HIT_TEST_BOX_BOX_HPP_

#include "../../hit_test_types.h"
#include "../bounds/aabb.hpp"

namespace so_collide 
{
    namespace so_2d
    {
        template< typename T >
        struct aabb_aabb_hit_test
        {
            so_typedefs( so_collide::so_2d::aabb<T>, aabb ) ;

            /// @returns
            /// ht_outside
            /// ht_intersect
            static hit_test_type hit_test_box_box( aabb_cref_t a, aabb_cref_t b )
            {
                if( ( a.get_min().greater_than( b.get_max() ) ).any() ) 
                    return so_collide::hit_test_type::outside ;

                if( ( b.get_min().greater_than( a.get_max() ) ).any() ) 
                    return so_collide::hit_test_type::outside ;

                return so_collide::hit_test_type::intersect ;
            }

            static bool_t intersect( aabb_cref_t a, aabb_cref_t b )
            {
                return hit_test_box_box( a, b ) == so_collide::hit_test_type::intersect ;
            }
        };
    }
}

#endif

