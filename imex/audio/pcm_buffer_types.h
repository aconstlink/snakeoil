//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_IMEX_AUDIO_PCM_BUFFER_TYPES_H_
#define _SNAKEOIL_IMEX_AUDIO_PCM_BUFFER_TYPES_H_

#include "../typedefs.h"

namespace so_imex
{
    enum class pcm_buffer_type
    {
        invalid,
        int8,
        int16,
        int32,
        ieee_float
    };

    namespace so_internal
    {
        static size_t const _buffer_type_sizes[] = { 0, 1, 2, 4, 4 } ;
    }

    static size_t size_of( pcm_buffer_type const bt )
    {
        return so_internal::_buffer_type_sizes[ size_t( bt ) ] ;
    }

    template< typename T >
    struct buffer_type_helper
    {
        static pcm_buffer_type deduce_buffer_type( void_t )
        {
            return pcm_buffer_type::invalid ;
        }
    };

    template<>
    struct buffer_type_helper<int8_t>
    {
        static pcm_buffer_type deduce_buffer_type( void_t )
        {
            return pcm_buffer_type::int8 ;
        }
    };

    template<>
    struct buffer_type_helper<int16_t>
    {
        static pcm_buffer_type deduce_buffer_type( void_t )
        {
            return pcm_buffer_type::int16 ;
        }
    };

    template<>
    struct buffer_type_helper<int32_t>
    {
        static pcm_buffer_type deduce_buffer_type( void_t )
        {
            return pcm_buffer_type::int32 ;
        }
    };

    template<>
    struct buffer_type_helper<float_t>
    {
        static pcm_buffer_type deduce_buffer_type( void_t )
        {
            return pcm_buffer_type::ieee_float ;
        }
    };

}

#endif