//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_PHYSICS_2D_MOTION_LINEAR_HPP_
#define _SNAKEOIL_PHYSICS_2D_MOTION_LINEAR_HPP_

#include "../../typedefs.h"
#include "../math.h"

namespace so_physics
{
	namespace so_2d
	{
		template< typename type_t >
		class linear_motion
		{
			typedef linear_motion< type_t > this_t ;
			typedef this_t const & this_cref_t ;
			typedef this_t & this_ref_t ;

			typedef so_math::vector2< type_t > vec2_t ;
			typedef vec2_t const & vec2_cref_t ;
			typedef vec2_t & vec2_ref_t ;

		private:

			vec2_t _position ;
			vec2_t _velocity ;

			type_t _mass ;
			type_t _mass_recip ;

		public:

			linear_motion( void_t ) : _mass(type_t(1)), _mass_recip(type_t(1)) {}
			linear_motion( type_t mass ) : _mass(mass), _mass_recip(type_t(1)/mass) {}

			linear_motion( vec2_cref_t pos, vec2_cref_t vel, type_t mass ) : 
			_position(pos), _velocity(vel), _mass(mass), _mass_recip(type_t(1)/mass){}

			linear_motion( vec2_cref_t pos, vec2_cref_t vel, type_t mass, type_t mass_recip ) : 
			_position(pos), _velocity(vel), _mass(mass), _mass_recip(mass_recip){}

		public:

			vec2_cref_t get_position( void_t ) const { return _position ; }
			vec2_cref_t get_velocity( void_t ) const { return _velocity ; }

			float_t get_mass( void_t ) const { return _mass ; }
			float_t get_mass_recip( void_t ) const { return _mass_recip ; }
			
		public:

			this_ref_t set_position( vec2_cref_t p ) { _position = p ; return *this ; }
			this_ref_t set_velocity( vec2_cref_t v ) { _velocity = v ; return *this ; }

			this_ref_t set_mass( type_t m ) { _mass = m ; _mass_recip = type_t(1)/m ; return *this ; }

		public:

			vec2_ref_t position( void_t ) { return _position ; }
			vec2_ref_t velocity( void_t ) { return _velocity ; }
			
		public:

			/// test this object to another object in order to test 
			/// positional displacement difference against an epsilon value.
			/// @param rhv [in] test that object if position difference is smaller than e
			/// @param e [in] epsilon if difference is smaller than that value, 
			/// the function returns true.
			bool_t is_resting( this_cref_t rhv, const type_t e ) const
			{
				return (this_t::get_position() - rhv.get_position()).length2() < e ;
			}

			bool_t is_resting_velocity( this_cref_t rhv, const type_t e ) const
			{
				return (this_t::get_velocity() - rhv.get_velocity()).length2() < e ;
			}
		};
	}
}

#endif

