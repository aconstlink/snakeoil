//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_NOISE_PERMUTATION_TABLE_H_
#define _SNAKEOIL_NOISE_PERMUTATION_TABLE_H_

#include "ipermutation.h"

#include <vector>
#include <algorithm>

namespace so_noise
{
    /// contains random values. It's primary usage is to 
    /// generate indices.
    /// there will be 2^bit values.
    class permutation_table : public ipermutation
    {
        so_this_typedefs( permutation_table ) ;

    private:

        uint_t _bit ;
        uint_t _seed ;
        uint_t _upper_bound ;

        std::vector< uint_t > _indices ;

    public:

        SNAKEOIL_NOISE_API permutation_table( uint_t seed, uint_t bit, uint_t num_mixes ) ;
        SNAKEOIL_NOISE_API permutation_table( this_cref_t rhv ) ;
        SNAKEOIL_NOISE_API permutation_table( this_rref_t rhv ) ;
        virtual SNAKEOIL_NOISE_API ~permutation_table( void_t ) ;

    public:

        static SNAKEOIL_NOISE_API this_ptr_t create( this_rref_t, std::string const & purpose ) ;
        static SNAKEOIL_NOISE_API void_t destroy( this_ptr_t ) ;

    public:

        virtual uint_t permute_at( uint_t i0 ) const ;
        virtual uint_t permute_at( uint_t i0, uint_t i1 ) const ;
        virtual uint_t permute_at( uint_t i0, uint_t i1, uint_t i2 ) const ;
        virtual uint_t get_num_entries( void_t ) const ;
        virtual uint_t get_bit( void_t ) const ;
        virtual void_t destroy( void_t ) ;

    private:

        /// not checked if in range. 
        /// since this is a private method, check i before using it.
        inline uint_t perm( uint_t i ) const ;

    };
}

#endif

