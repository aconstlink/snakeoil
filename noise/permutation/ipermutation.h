//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_NOISE_IPERMUTATION_H_
#define _SNAKEOIL_NOISE_IPERMUTATION_H_

#include "../typedefs.h"
#include "../protos.h"
#include "../api.h"

namespace so_noise
{
    class SNAKEOIL_NOISE_API ipermutation
    {
    public:

        virtual uint_t permute_at( uint_t i0 ) const = 0 ;
        virtual uint_t permute_at( uint_t i0, uint_t i1 ) const = 0 ;
        virtual uint_t permute_at( uint_t i0, uint_t i1, uint_t i2 ) const = 0 ;
        virtual uint_t get_num_entries( void_t ) const = 0 ;
        virtual uint_t get_bit( void_t ) const = 0 ;
        virtual void_t destroy( void_t ) = 0 ;
    };
}

#endif

