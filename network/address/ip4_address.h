//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_NETWORK_IP4_ADRESS_H_
#define _SNAKEOIL_NETWORK_IP4_ADRESS_H_

#include "../typedefs.h"

#include <snakeoil/std/string/string.hpp>
#include <snakeoil/math/vector/vector4.hpp>

namespace so_net
{
    class ip4_address
    {
    private:

        so_math::vec4ui_t _addr ;
        
    public:

        ip4_address( void_t )
        {
            _addr = so_math::vec4ui_t( 127, 0, 0, 1 ) ;
        }

        ip4_address( uint_t a, uint_t b, uint_t c, uint_t d )
        {
            _addr = so_math::vec4ui_t( a, b, c, d ) ;
        }

    public:

        so_std::string_t to_string( void_t ) const
        {
            return std::to_string( _addr.x() ) + "." + std::to_string( _addr.y() ) +
                "." + std::to_string( _addr.z() ) + "." + std::to_string( _addr.w() ) ;
        }
    };
    so_typedef( ip4_address ) ;
}

#endif