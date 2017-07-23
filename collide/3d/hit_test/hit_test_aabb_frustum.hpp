//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_COLLIDE_3D_HIT_TEST_BOX_FRUSTUM_HPP_
#define _SNAKEOIL_COLLIDE_3D_HIT_TEST_BOX_FRUSTUM_HPP_

#include "hit_test_aabb_plane.hpp"

#include "../bounds/frustum.hpp"

namespace so_collide 
{
    using namespace so_math ;
    
	namespace so_3d
	{

		/// hit tests box and view frustum
		/// will test all planes in the view frustum
		/// @return
		/// ht_outside: the box is not in the frustum
		/// ht_inside: the box is in the frustum
		/// ht_intersect: the box and the frustum intersect
		template< typename type_t >
		static hit_test_type hit_test_box_frustum( so_3d::aabb< type_t > const & box, so_3d::frustum<type_t> const & frust )
		{
			bool_t bintersect = false ;
			hit_test_type ret = ht_outside ;

			for( size_t i=0; i<6; ++i )
			{
				ret = hit_test_box_plane<type_t>( box, frust.get_plane(i) ) ;
				if( ret == ht_intersect ) bintersect = true ;
				if( ret == ht_outside ) break ;
			}

			if((ret != ht_outside) && bintersect) { ret = ht_intersect ;}
			return ret ;
		}
	}
}

#endif

