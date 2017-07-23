//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_NETWORK_UTILITY_PACK_DATA_H_
#define _SNAKEOIL_NETWORK_UTILITY_PACK_DATA_H_

#include "../typedefs.h"

#include <snakeoil/memory/utility/uint8_split_merge.h>

namespace so_net
{
    namespace so_utility
    {
        /// register -> network byte order(nbo)
        struct to_nbo
        {
            static void_t of( uint16_t number, uint8_t * to )
            {
                so_memory::so_utility::uint8_msb_split::of( number, to ) ;
            }

            static void_t of( uint32_t number, uint8_t * to )
            {
                so_memory::so_utility::uint8_msb_split::of( number, to ) ;
            }

            static void_t of( uint64_t number, uint8_t * to )
            {
                so_memory::so_utility::uint8_msb_split::of( number, to ) ;
            }
        } ;

        /// network byte order(nbo) -> register
        struct from_nbo
        {
            static uint16_t data_16( const uint8_t * from )
            {
                return so_memory::so_utility::uint8_msb_merge::uint16( from ) ;
            }

            static uint32_t data_32( const uint8_t * from )
            {
                return so_memory::so_utility::uint8_msb_merge::uint32( from ) ;
            }

            static uint64_t data_64( const uint8_t * from )
            {
                return so_memory::so_utility::uint8_msb_merge::uint64( from ) ;
            }
        } ;
    }
}

#endif


