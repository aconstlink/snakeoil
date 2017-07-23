//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_NOISE_GRADIENT_NOISE_H_
#define _SNAKEOIL_NOISE_GRADIENT_NOISE_H_

#include "inoise.h"

#include <snakeoil/math/vector/vector3.hpp>
#include <snakeoil/math/utility/fn.hpp>
#include <snakeoil/math/interpolation/interpolate.hpp>

#include <vector>

namespace so_noise
{
    /// stores a table of num_entries pseudo random values internally.
    /// generates prns in [-1,1] space
    class gradient_noise : public inoise
    {
        so_this_typedefs( gradient_noise ) ;

    private:

        so_noise::ipermutation_ptr_t _perm_ptr = nullptr ;
            
        std::vector< so_math::vec3f_t > _noises ;

    private:
        
        /// non-copyable
        gradient_noise( this_cref_t ) {}

    public:

        SNAKEOIL_NOISE_API gradient_noise( uint_t seed, uint_t bit, uint_t num_mixes=2 )  ;
        SNAKEOIL_NOISE_API gradient_noise( this_rref_t rhv ) ;
        virtual SNAKEOIL_NOISE_API ~gradient_noise( void_t ) ;

    public:

        static SNAKEOIL_NOISE_API this_ptr_t create( this_rref_t, std::string const & purpose ) ;
        static SNAKEOIL_NOISE_API void_t destroy( this_ptr_t ) ;

    private:

        float_t lattice( uint_t i0, float_t f1 ) const ;
        float_t lattice( uint_t i0, uint_t i1, float_t f1, float_t f2 ) const ;
        float_t lattice( uint_t i0, uint_t i1, uint_t i2, float_t f1, float_t f2, float_t f3 ) const ;
        
    public:

        virtual float_t noise( float_t x ) const  ;
        virtual float_t noise( float_t x, float_t y ) const  ;
        virtual float_t noise( float_t x, float_t y, float_t z ) const ;
        virtual void_t destroy( void_t ) ;

    public:

        uint_t get_num_entries( void_t ) const ;
    };
    
    
}

#endif

