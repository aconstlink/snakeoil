//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_PHYSICS_2D_MOTION_ANGULAR_HPP_
#define _SNAKEOIL_PHYSICS_2D_MOTION_ANGULAR_HPP_

#include "../../typedefs.h"

#include <snakeoil/math/utility/fn.hpp>
#include <snakeoil/math/utility/angle.hpp>

namespace so_physics
{
	namespace so_2d
	{
		template< typename type_t >
		class angular_motion
		{
			typedef angular_motion< type_t > this_t ;
			typedef this_t const & this_cref_t ;
			typedef this_t & this_ref_t ;

			typedef type_t & type_ref_t ;

			typedef so_math::angle<type_t> angle_t ;
			typedef angle_t & angle_ref_t ;
			typedef angle_t const & angle_cref_t ;

		private: 

			/// angular displacement OMEGA
			angle_t _displacement ;
			
			/// angular velocity omega
			type_t _velocity ;

			type_t _inertia ;
			type_t _inertia_recip ;

		public:

			angular_motion( void_t ) : 
			  _displacement(angle_t(0)), _velocity(type_t(0)), _inertia(type_t(1)), _inertia_recip(type_t(1)) {}

			angular_motion( type_t inertia ) : 
			_displacement(angle_t(0)), _velocity(type_t(0)), _inertia(inertia), _inertia_recip(type_t(1)/inertia) {}

			angular_motion( type_t displace, type_t velocity, type_t inertia ) : 
			_displacement(angle_t(displace)), _velocity(velocity), _inertia(inertia), _inertia_recip(type_t(1)/inertia){}

			angular_motion( type_t displace, type_t velocity, type_t inertia, type_t inertia_recip ) : 
			_displacement(angle_t(displace)), _velocity(velocity), _inertia(inertia), _inertia_recip(inertia_recip){}

		public:

			angle_t get_displacement( void_t ) const { return _displacement ; }
			type_t get_velocity( void_t ) const { return _velocity ; }

			type_t get_inertia( void_t ) const { return _inertia ; }
			type_t get_inertia_recip( void_t ) const { return _inertia_recip ; }
			
		public:

			this_ref_t set_displacement( angle_cref_t a ) { _displacement = a ; return *this ; }
			this_ref_t set_velocity( type_t v ) { _velocity = v ; return *this ; }
			
			this_ref_t set_inertia( type_t i ) { _inertia = i ; _inertia_recip = type_t(1)/_inertia ; return *this ;}

		public:

			angle_ref_t displacement( void_t ) { return _displacement ; }
			type_ref_t velocity( void_t ) { return _velocity ; }
			
		public:

			/// test this object against another object in order to test 
			/// angular displacement difference against an epsilon value.
			/// @param rhv [in] test against that object if difference is smaller than e
			/// @param e [in] epsilon if difference is smaller than that value, 
			/// the function returns true.
			bool_t is_resting( this_cref_t rhv, const type_t e ) const
			{ 
				return so_math::fn<type_t>::abs((this_t::get_displacement() - rhv.get_displacement()).as_radians()) < e ; 
			}		

			bool_t is_resting_velocity( this_cref_t rhv, const type_t e ) const
			{ 
				return (this_t::get_velocity() - rhv.get_velocity()) < e ; 
			}	

		};
	}
}

#endif

