//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_NOISE_METHOD_INOISE_H_
#define _SNAKEOIL_NOISE_METHOD_INOISE_H_

#include "../typedefs.h"
#include "../protos.h"
#include "../api.h"

namespace so_noise
{
    class SNAKEOIL_NOISE_API inoise
    {
    public:

        virtual float_t noise( float_t x ) const = 0 ;
        virtual float_t noise( float_t x, float_t y ) const = 0 ;
        virtual float_t noise( float_t x, float_t y, float_t z ) const = 0 ;
        virtual void_t destroy( void_t ) = 0 ;
    };
}

#endif

