//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_COLLIDE_HIT_TEST_TYPE_HPP_
#define _SNAKEOIL_COLLIDE_HIT_TEST_TYPE_HPP_

namespace so_collide
{
    
    enum hit_test_type
    {
        ht_inside,
        ht_outside,
        ht_intersect,
		ht_disjoint,
		ht_overlap,
        num_hit_test_types
    };
    
}
#endif

