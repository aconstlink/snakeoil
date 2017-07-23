//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_NOISE_METHOD_VALUE_NOISE_H_
#define _SNAKEOIL_NOISE_METHOD_VALUE_NOISE_H_

#include "inoise.h"

#include <vector>

namespace so_noise
{
    /// stores a table of num_entries pseudo random values internally.
    /// generates prns in [-1,1] space
    class value_noise : public inoise
    {
        so_this_typedefs( value_noise ) ;

    private:

        ipermutation_ptr_t _perm_ptr = nullptr ;
        std::vector< float_t > _noises ;

    public:

        static SNAKEOIL_NOISE_API this_ptr_t create( this_rref_t, std::string const & purpose ) ;
        static SNAKEOIL_NOISE_API void_t destroy( this_ptr_t ) ;

    public:

        SNAKEOIL_NOISE_API value_noise( uint_t seed, uint_t bit, uint_t num_mixes=2 ) ;
        SNAKEOIL_NOISE_API value_noise( this_rref_t ) ;
        virtual SNAKEOIL_NOISE_API ~value_noise( void_t ) ;

    public:

        float_t lattice( uint_t i0 ) const  ;
        float_t lattice( uint_t i0, uint_t i1 ) const ;
        float_t lattice( uint_t i0, uint_t i1, uint_t i2 ) const ;

    public:

        /// @return value in [-1,1]
        virtual float_t noise( float_t f0 ) const ;

        /// returns a noise value for a 2d input.
        /// @return value in [-1,1]
        virtual float_t noise( float_t f0, float_t f1 ) const ;

        virtual float_t noise( float_t f0, float_t f1, float_t f2 ) const ;

        virtual void_t destroy( void_t ) ;

    public:

        uint_t get_num_entries( void_t ) const ;
    };
}

#endif

