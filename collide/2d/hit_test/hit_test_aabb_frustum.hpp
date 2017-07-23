//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_COLLIDE_3D_HIT_TEST_BOX_FRUSTUM_HPP_
#define _SNAKEOIL_COLLIDE_3D_HIT_TEST_BOX_FRUSTUM_HPP_

#include "hit_test_aabb_plane.hpp"

#include "../../utility/u3d/view_frustum.hpp"

namespace so_collide 
{
	namespace so_3d
	{
        using namespace so_intersection ;

		/// hit tests box and view frustum
		/// will test all planes in the view frustum
		/// @return
		/// ht_outside: the box is not in the frustum
		/// ht_inside: the box is in the frustum
		/// ht_intersect: the box and the frustum intersect
		template< typename type_t >
		static hit_test_type hit_test_box_frustum( aabb< so_math::vector3< type_t > > const & box, so_math::so_u3d::view_frustum<type_t> const & vf )
		{
			typename so_math::so_u3d::view_frustum<type_t>::plane_t plane ;
			bool_t bintersect = false ;
			hit_test_type ret = ht_outside ;

			for( size_t i=0; i<6; ++i )
			{
				vf.get_plane(i, plane) ;
				ret = hit_test_box_plane<type_t>( box, plane ) ;
				if( ret == ht_intersect ) bintersect = true ;
				if( ret == ht_outside ) break ;
			}

			if((ret != ht_outside) && bintersect) { ret = ht_intersect ;}
			return ret ;
		}
	}
}

#endif

