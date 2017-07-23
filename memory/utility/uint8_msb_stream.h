//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_MEMORY_UTILITY_UINT8_MSB_STREAM_HPP_
#define _SNAKEOIL_MEMORY_UTILITY_UINT8_MSB_STREAM_HPP_

#include "../typedefs.h"
#include "uint8_split_merge.h"

namespace so_memory
{
    namespace so_utility
    {
        /// streams certain numbers to an array set in the ctor.
        /// @note not thread safe
        class uint8_msb_stream
        {
            typedef uint8_msb_stream this_t ;
            typedef this_t & this_ref_t ;

            uint8_t * _array ;
            size_t _max_len ;

            size_t _cur_pos = size_t(0) ;

        public:

            uint8_msb_stream( uint8_t * ar, size_t max_len ) : _array(ar), _max_len(max_len) {}

        public:

            this_ref_t operator << ( uint8_t n )
            {
                _array[_cur_pos++] = n ;
                return *this ;
            }

            this_ref_t operator << ( uint16_t n )
            {
                so_memory::so_utility::uint8_msb_split::of( n, &_array[_cur_pos] ) ;
                _cur_pos += 2 ;
                return *this ;
            }

            this_ref_t operator << ( uint32_t n )
            {
                so_memory::so_utility::uint8_msb_split::of( n, &_array[_cur_pos] ) ;
                _cur_pos += 4 ;
                return *this ;
            }

            this_ref_t operator << ( uint64_t n )
            {
                so_memory::so_utility::uint8_msb_split::of( n, &_array[_cur_pos] ) ;
                _cur_pos += 8 ;
                return *this ;
            }

        };

        class uint8c_msb_stream
        {
            typedef uint8c_msb_stream this_t ;
            typedef this_t & this_ref_t ;

            uint8_t const * _array ;
            size_t _max_len ;

            size_t _cur_pos = size_t(0) ;

        public:

            uint8c_msb_stream( uint8_t const * ar, size_t max_len ) : _array(ar), _max_len(max_len) {}

        public:

            this_ref_t operator >> ( uint8_t & n )
            {
                n = _array[_cur_pos++] ;
                return *this ;
            }

            this_ref_t operator >> ( uint16_t & n )
            {
                n = so_memory::so_utility::uint8_msb_merge::uint16( &_array[_cur_pos] ) ;
                _cur_pos += 2 ;
                return *this ;
            }

            this_ref_t operator >> ( uint32_t & n )
            {
                n = so_memory::so_utility::uint8_msb_merge::uint32( &_array[_cur_pos] ) ;
                _cur_pos += 4 ;
                return *this ;
            }

            this_ref_t operator >> ( uint64_t & n )
            {
                n = so_memory::so_utility::uint8_msb_merge::uint64( &_array[_cur_pos] ) ;
                _cur_pos += 8 ;
                return *this ;
            }

        };
    }
}

#endif

