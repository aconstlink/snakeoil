//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_PHYSICS_2D_SOLVE_HPP_
#define _SNAKEOIL_PHYSICS_2D_SOLVE_HPP_

#include "../../typedefs.h"

#include "../motion/linear.hpp"
#include "../motion/angular.hpp"

namespace so_physics
{
	namespace so_2d
	{
		namespace so_solve
		{
			template< typename type_t >
			class euler
			{
				typedef euler< type_t > this_t ;
				typedef this_t & this_ref_t ;

				typedef so_math::vector2< type_t > vec2_t ;
				typedef vec2_t const & vec2_cref_t ;

				typedef linear_motion< type_t > linear_t ;
				typedef linear_t const & linear_cref_t ;
				typedef linear_t & linear_ref_t ;

				typedef angular_motion< type_t > angular_t ;
				typedef angular_t const & angular_cref_t ;
				typedef angular_t & angular_ref_t ;

			public:

				/// pass the linear motion to be solved for the passed accumulated
				/// force during dt time.
				/// @return the integrated linear motion.
				static linear_t integrate( linear_cref_t lm_old, vec2_cref_t force, type_t dt )
				{
					const vec2_t a = force * lm_old.get_mass_recip() ;
					const vec2_t v = a * dt + lm_old.get_velocity() ;
					const vec2_t p = v * dt + lm_old.get_position() ;
					return linear_t( p, v, lm_old.get_mass(), lm_old.get_mass_recip() ) ;
				}

				/// pass the angular motion to be solved for the passed
				/// accumulated momentum during dt time.
				static angular_t integrate( angular_cref_t am_old, type_t torque, type_t dt )
				{
					const type_t a = torque * am_old.get_inertia_recip() ;
					const type_t v = a * dt + am_old.get_velocity() ;
					const type_t p = v * dt + am_old.get_displacement().as_radians() ;
					return angular_t( p, v, am_old.get_inertia(), am_old.get_inertia_recip() ) ;
				}
			};
		}
		
	}
}

#endif

