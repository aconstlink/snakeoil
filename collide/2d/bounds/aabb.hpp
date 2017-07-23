//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_COLLIDE_2D_BOUNDS_AABB_HPP_
#define _SNAKEOIL_COLLIDE_2D_BOUNDS_AABB_HPP_

#include "../../typedefs.h"

#include <snakeoil/math/vector/vector2.hpp>
#include <snakeoil/math/vector/vector4.hpp>
#include <snakeoil/math/common/function.h>

namespace so_collide 
{
    namespace so_2d
    {
        template< typename type_t >
        class aabb
        {
            typedef aabb<type_t> this_t ;
            typedef this_t & this_ref_t ;
            typedef this_t const & this_cref_t ;

            typedef so_math::vector2< type_t > vec2_t ;
            typedef vec2_t const & vec2_cref_t ;
            typedef vec2_t & vec2_ref_t ;
			typedef so_math::vec2b_t vec2b_t ;

			typedef so_math::vector3< type_t > vec3_t ;
			typedef vec3_t & vec3_ref_t ;

			typedef so_math::vector4< type_t > vec4_t ;
			typedef so_math::vec4b_t vec4b_t ;

        private:

            vec2_t _max ;
            vec2_t _min ;

        public:

            aabb( void_t ){}

			/// constructs the aabb without checking for
			/// validity of the given values. The user must 
			/// assert the correctness of max and being on the 
			/// correct sides herself.
            aabb( vec2_cref_t max, vec2_cref_t min )
            {
                _max = max ;
                _min = min ;
            }

			/// constructs the aabb a circle like
			/// given parameters.
			aabb( vec2_cref_t center, type_t radius )
			{
				const vec2_t vmin = center - vec2_t(radius) ;
				const vec2_t vmax = center + vec2_t(radius) ;
			
				_max = vmax ;
				_min = vmin ;
			}

            aabb( this_cref_t rhv )
            {
                (*this) = rhv ;
            }

        public: // test

			/// is point inside the aabb.
            bool_t is_inside( vec2_cref_t p ) const
            {
				const vec2b_t test_max = p.less_than(_max) ;
				const vec2b_t test_min = p.greater_than(_min) ;

                return test_max.all() && test_min.all() ;
            }

			/// computes the closest point to p on aabb.
			/// q will contain the closest point.
			/// @note only works if p is outside of aabb.
			/// otherwise, p itself is the closest point.
			void_t closest_point_to( vec2_cref_t p, vec2_ref_t q ) const 
			{
				q = _min.max_ed( p ) ;
				q = _max.min_ed( q ) ;				
			}

			/// returns the closest point to p.
			/// @see closest_point_to
			vec2_t closest_point_to( vec2_cref_t p ) const 
			{
				vec2_t q ;
				this_t::closest_point_to( p, q ) ;
				return q ;
			}

			/// returns the squared distance to the given point p
			/// from the aabb's bounds. 
			/// @note if point is inside, 0 will be returned.
			type_t squared_distance_to( vec2_cref_t p ) const 
			{
				const vec2b_t bmin = p.less_than( _min ) ;
				const vec2b_t bmax = p.greater_than( _max ) ;

				const vec2_t min_dist = _min - p ;
				const vec2_t max_dist = p - _max ;

				const vec4_t dist = vec4_t( min_dist, max_dist ).
					select( vec4_t( type_t(0) ), vec4b_t( bmin, bmax ) ) ;
				
				return dist.dot(dist) ;
			}

			/// Calculates the nearest aabb side normal with distance.
			/// @param p [in] the point to which the normal is required.
			/// @param n [out] the normal ( x, y, hesse distance )
			/// @note the distance is measured to be of length to the aabb's center.
			void_t calculate_normal_for( vec2_cref_t p, vec3_ref_t n ) const
			{
				const vec3_t p_local( p - this_t::get_center(), type_t(1) ) ;
				const vec2_t ext = this_t::get_extend() ;

				const type_t sign_x = so_math::sign<type_t>( p_local.x() ) ;
				const type_t sign_y = so_math::sign<type_t>( p_local.y() ) ;

				const vec3_t n_x( sign_x, type_t(0), -ext.x() ) ;
				const vec3_t n_y( type_t(0), sign_y, -ext.y() ) ;

				const type_t d_x = p_local.dot( n_x ) ;
				const type_t d_y = p_local.dot( n_y ) ;
				
				type_t const bias = d_x * d_y ;

				// fully inside/outside
				if( bias > type_t(0) )
				{
                    vec2_t const dir_ext = ext * vec2_t( sign_x, sign_y ) ;
                    vec2_t const local_n = (p_local.xy()-dir_ext).normalized() ;
                    n = vec3_t( local_n, -local_n.dot(dir_ext) ) ;
				}
				// outside partial
				else
				{
					n = ( d_x > type_t(0) ) ? n_x : n_y ;
				}
			}

			/// returns the aabb side normal for the point p.
			/// the normal will also contain the distance to the 
			/// box's center. 
			/// @see calculate_normal_for
			vec3_t calculate_normal_for( vec2_cref_t p ) const 
			{
				vec3f_t n ;
				calculate_normal_for( p, n ) ;
				return n ;
			}

		public: // transformation

            this_ref_t translate( vec2_cref_t translate )
			{
                this->set_min(_min+translate) ;
                this->set_max(_max+translate) ;
                return *this ;
            }

            this_t translated( vec2_cref_t translate ) const 
            {
                return this_t( *this ).translate() ;
            }

            void translate( vec2_cref_t translate, this_ref_t box_out ) const
			{
                box_out.set_max(this_t::get_max()+translate) ;
                box_out.set_min(this_t::get_min()+translate) ;
            }

            this_ref_t translate_to_position( vec2_t const & position ) 
            {
                vec2_t center = center() ;
                this_t::set_min(this_t::get_min()-center+position) ;
                this_t::set_max(this_t::get_max()-center+position) ;
                return *this ;
            }

        public:

            this_ref_t operator()( vec2_cref_t max, vec2_cref_t min )
            {
                _max = max ;
                _min = min ;
                return *this ;
            }

            this_ref_t operator = ( this_cref_t rhv )
            {
                return (*this)( rhv.get_max(), rhv.get_min() ) ;
            }

        public:

            vec2_cref_t get_max( void_t ) const {return _max;}
            vec2_cref_t get_min( void_t ) const {return _min;}
            vec2_ref_t get_max( void_t ){ return _max; }
            vec2_ref_t get_min( void_t ){ return _min; }

            void_t set_max( vec2_cref_t max ){_max=max;}
            void_t set_min( vec2_cref_t min ){_min=min;}

            vec2_t get_center( void_t ) const 
            {
                return (this_t::get_max() - this_t::get_min()) * type_t(0.5) + this_t::get_min() ;
            }

			vec2_t get_extend( void_t ) const 
			{
				return ((this_t::get_max() - this_t::get_min()) * type_t(0.5)) ;
			}

			type_t get_radius( void_t ) const
			{
				return this_t::get_extend().length() ;
			}

			type_t get_radius2( void_t ) const
			{
				return this_t::get_extend().length2() ;
			}

        };
    }
}

#endif

