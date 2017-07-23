//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_PHYSICS_3D_BODY_HPP_
#define _SNAKEOIL_PHYSICS_3D_BODY_HPP_

#include <snakeoil/math/vector/vector3.hpp>

#include <snakeoil/core/types.hpp>

namespace so_physics
{
    using namespace so_core::so_types ;

    namespace so_3d
    {
        template< typename type_t >
        class body
        {
            typedef body< type_t > this_t ;
            typedef this_t & this_ref_t ;
            typedef this_t const & this_cref_t ;
            

            typedef so_math::vector3< type_t > vec3_t ;
            typedef vec3_t const & vec3_cref_t ;

        private:

            vec3_t _pos ;
            vec3_t _vel ;
            vec3_t _acc ;

            type_t _mass ;
        
        public:
            
            body( void_t )
            {
                _mass = type_t(1) ;
            }

            body( vec3_cref_t pos, vec3_cref_t vel, vec3_cref_t acc, type_t mass )
            {
                _pos = pos ;
                _vel = vel ;
                _acc = acc ;
                _mass = mass ;
            }

        public:

            this_ref_t set_position( vec3_cref_t pos ){ _pos = pos ; return *this ; }
            this_ref_t set_velocity( vec3_cref_t vel ) { _vel = vel ; return *this ; }
            this_ref_t set_acceleration( vec3_cref_t acc ) { _acc = acc ; return *this ; }
            this_ref_t set_mass( type_t mass ) { _mass = mass ; return *this ; }

        public:

            vec3_cref_t get_position( void_t ) const { return _pos ; }
            vec3_cref_t get_velocity( void_t ) const { return _vel ; }
            vec3_cref_t get_acceleration( void_t ) const { return _acc ; }
            type_t get_mass( void_t ) const { return _mass ; }

        };
    }
}

#endif

