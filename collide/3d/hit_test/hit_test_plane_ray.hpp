//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_COLLIDE_3D_HIT_TEST_PLANE_RAY_HPP__
#define _SNAKEOIL_COLLIDE_3D_HIT_TEST_PLANE_RAY_HPP__

#include "../../hit_test_types.h"

#include <snakeoil/math/primitive/3d/ray.hpp>
#include <snakeoil/math/vector/vector3.hpp>
#include <snakeoil/math/vector/vector4.hpp>

namespace so_collide 
{
	namespace so_3d
	{
		template< typename type_t >
		static hit_test_type hit_test_plane_ray( 
			hesse_plane<type_t, 3> const & hesse, 
			so_math::vector3< type_t > const & origin,
			so_math::vector3< type_t > const & dir, type_t & lambda )
		{
			typedef so_math::vector3< type_t > vec3_t ;
			typedef so_math::vector4< type_t > vec4_t ;

			// direction test
			float ndr = vec3_t(hesse.get_plane()).dot( dir ) ;
			if( ndr >= -0.00001 ) return so_collide::ht_disjoint ;

			// point distance
			float ndp = hesse.get_plane().dot( vec4_t(origin,1) ) ;

			lambda = - ndp / ndr ;

			return so_collide::ht_intersect ;
		}

		/// all data need to be in the same space and relative to the same origin.
		/// plane_dist: the plane encoded in (normal vector, distance to origin)
		/// ray_origin: the origin of the ray 
		template< typename type_t >
		static hit_test_type hit_test_plane_ray( hesse_plane<type_t, 3> const & hesse, so_math::so_3d::ray<type_t > const & ray, type_t & lambda )
		{
			return hit_test_plane_ray(hesse, ray.get_origin(), ray.get_direction(), lambda ) ;
		}

		template< typename type_t >
		static hit_test_type hit_test_plane_ray( hesse_plane<type_t, 3> const & hesse, 
			so_math::so_3d::ray<type_t> const & ray, type_t threshold, type_t & lambda )
		{
			typedef so_math::vector3< type_t > vec3_t ;
			typedef so_math::vector4< type_t > vec4_t ;

			// direction test
			float ndr = vec3_t(hesse).dot( ray.get_direction()) ;
			if( ndr >= threshold ) return so_collide::ht_disjoint ;

			// point distance
			float ndp = hesse.get_plane().dot( vec4_t(ray.get_origin(),1) ) ;

			lambda = - ndp / (ndr+0.000001f) ;

			return so_collide::ht_intersect ;
		}
	}
}

#endif

