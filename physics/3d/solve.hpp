//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_PHYSICS_3D_INTEGRATE_EULER_HPP_
#define _SNAKEOIL_PHYSICS_3D_INTEGRATE_EULER_HPP_

#include "body.hpp"

#include <snakeoil/math/vector/vector3.hpp>

namespace so_physics
{
    namespace so_3d
    {
        template< typename type_t >
        class solve
        {
            typedef body< type_t > body_t ;
            typedef body_t const & body_cref_t ;
            typedef body_t & body_ref_t ;

            typedef so_math::vector3< type_t > vec3_t ;
            typedef vec3_t const & vec3_cref_t ;

        public:

            static body_t euler( body_cref_t b, vec3_cref_t force, type_t dt )
            {
                const vec3_t acc = force / b.get_mass() ;
                const vec3_t vel = b.get_velocity() + acc * dt ;
                const vec3_t pos = b.get_position() + vel * dt ;
                return body_t( pos, vel, acc, b.get_mass() ) ;
            }

            static void_t euler_inplace( body_ref_t b, vec3_cref_t force, type_t dt )
            {
                b.set_acceleration( force / b.get_mass() ) ;
                b.set_velocity( b.get_velocity() + b.get_acceleration() * dt ) ;
                b.set_position( b.get_position() + b.get_velocity() * dt ) ;
            }
        };
        
    }
}

#endif

