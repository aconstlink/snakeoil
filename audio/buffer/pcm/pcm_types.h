//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_AUDIO_BUFFER_TYPES_H_
#define _SNAKEOIL_AUDIO_BUFFER_TYPES_H_

namespace so_audio
{
    enum class buffer_type
    {
        invalid,
        int8,
        int16,
        int32,
        uint8,
        uint16,
        uint32,
        ieee_float
    };

    namespace so_internal
    {
        static size_t const _buffer_type_sizes[] = { 0, 1, 2, 4, 1, 2, 4, 4 } ;
    }

    static size_t size_of( buffer_type const bt )
    {
        return so_internal::_buffer_type_sizes[ size_t( bt ) ] ;
    }

    template< typename T >
    struct buffer_type_helper
    {
        static buffer_type deduce_buffer_type( void_t )
        {
            return buffer_type::invalid ;
        }
    };


    template<>
    struct buffer_type_helper<int8_t>
    {
        static buffer_type deduce_buffer_type( void_t )
        {
            return buffer_type::int8 ;
        }
    };

    template<>
    struct buffer_type_helper<int16_t>
    {
        static buffer_type deduce_buffer_type( void_t )
        {
            return buffer_type::int16 ;
        }
    };

    template<>
    struct buffer_type_helper<int32_t>
    {
        static buffer_type deduce_buffer_type( void_t )
        {
            return buffer_type::int32 ;
        }
    };

    template<>
    struct buffer_type_helper<uint8_t>
    {
        static buffer_type deduce_buffer_type( void_t )
        {
            return buffer_type::uint8 ;
        }
    };

    template<>
    struct buffer_type_helper<uint16_t>
    {
        static buffer_type deduce_buffer_type( void_t )
        {
            return buffer_type::uint16 ;
        }
    };

    template<>
    struct buffer_type_helper<uint32_t>
    {
        static buffer_type deduce_buffer_type( void_t )
        {
            return buffer_type::uint32 ;
        }
    };

    template<>
    struct buffer_type_helper<float_t>
    {
        static buffer_type deduce_buffer_type( void_t )
        {
            return buffer_type::ieee_float ;
        }
    };
}

#endif