//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_AUDIO_PCM_PCM_BUFFER_H_
#define _SNAKEOIL_AUDIO_PCM_PCM_BUFFER_H_

#include "ipcm_buffer.h"

#include <snakeoil/std/container/vector.hpp>

namespace so_audio
{
    template< typename T >
    class pcm_buffer : public ipcm_buffer
    {
        so_this_typedefs( pcm_buffer<T> ) ;
        so_typedefs( T, type ) ;

    

    public:

        struct pcm_sample
        {
            type_t value ;
        };
        so_typedef( pcm_sample ) ;
        so_typedefs( so_std::vector< pcm_sample >, pcm_samples ) ;

    public:

        struct init_data
        {
            size_t num_channels ;
            size_t num_samples_per_sec ;
            size_t num_second_cache = 10 ;
        };
        so_typedef( init_data ) ;

        so_typedefs( std::function< void_t ( size_t const, pcm_sample_cref_t ) >, iterate_funk ) ;
        so_typedefs( std::function< void_t ( pcm_sample_cref_t ) >, append_funk ) ;
        so_typedefs( std::function< void_t ( size_t const, pcm_sample_cref_t ) >, for_each_new_funk ) ;

    private:

        size_t _num_channels = 0 ;
        size_t _num_sampes_per_sec = 0 ;
        
        pcm_samples_t _samples ;
        size_t _start = 0 ;
        size_t _end = 0 ;

    public:
        
        pcm_buffer( init_data_cref_t id )
        {
            _num_sampes_per_sec = id.num_samples_per_sec ;
            _num_channels = id.num_channels ;
            
            _samples.resize( _num_channels * _num_sampes_per_sec * id.num_second_cache ) ;
        }

        pcm_buffer( this_cref_t ) = delete ;

        pcm_buffer( this_rref_t rhv )
        {
            _num_sampes_per_sec = rhv._num_sampes_per_sec ;
            _num_channels = rhv._num_channels ;

            _start = rhv._start ;
            _end = rhv._end ;
            _samples = std::move( rhv._samples ) ;
        }

        ~pcm_buffer( void_t ){}

    public:


        static this_ptr_t create( this_rref_t rhv, so_memory::purpose_cref_t p )
        {
            return so_audio::memory::alloc( std::move( rhv ), p ) ;
        }

        static void_t destroy( this_ptr_t ptr )
        {
            so_audio::memory::dealloc( ptr ) ;
        }

    public: // interface

        virtual double_t get_time_in_sec( void_t ) const
        {
            return double_t( _samples.size() ) / double_t(_num_sampes_per_sec*_num_channels) ;
        }

        virtual buffer_type get_buffer_type( void_t ) const
        {
            return so_audio::buffer_type_helper<T>::deduce_buffer_type() ;
        }

        virtual range_t copy_from( ipcm_buffer_ptr_t pcm_in, size_t const start, size_t const num_samples )
        {
            return this_t::copy_from( pcm_in->get_buffer_type(),
                static_cast<byte_cptr_t>( pcm_in->get_buffer_ptr_at( start ) ), num_samples ) ;
        }

        virtual range_t copy_from( so_audio::buffer_type bt, byte_cptr_t dptr, size_t const num_samples )
        {
            switch( bt )
            {
            case buffer_type::int8:
                return this_t::copy_from<int8_t>( dptr, num_samples ) ;
            case buffer_type::int16:
                return this_t::copy_from<int16_t>( dptr, num_samples ) ;
            case buffer_type::int32:
                return this_t::copy_from<int32_t>( dptr, num_samples ) ;
            case buffer_type::uint16:
                return this_t::copy_from<uint16_t>( dptr, num_samples ) ;
            case buffer_type::uint32:
                return this_t::copy_from<uint32_t>( dptr, num_samples ) ;
            case buffer_type::ieee_float:
                return this_t::copy_from<float_t>( dptr, num_samples ) ;
            }
            return range_t( 0, 0 ) ;
        }

        virtual range_t copy_from( byte_cptr_t dptr, size_t const num_samples )
        {
            return this_t::copy_from( this_t::get_buffer_type(), dptr, num_samples ) ;
        }

        virtual void_cptr_t get_buffer_ptr_at( size_t const at ) const
        {
            return _samples.data() + at * _num_channels ;
        }

        virtual void_cptr_t get_buffer_ptr( void_t ) const
        {
            return _samples.data() ;
        }

        virtual size_t get_num_samples( void_t ) const 
        {
            return _samples.size() ;
        }

        virtual size_t get_num_samples_per_second( void_t ) const
        {
            return _num_sampes_per_sec ;
        }

        virtual size_t get_num_channels( void_t ) const
        {
            return _num_channels ;
        }

        virtual void_t destroy( void_t ) 
        {
            this_t::destroy( this ) ;
        }

    public: // copy templates

        template< typename src_t, typename dst_t >
        static dst_t transform( src_t const s )
        {
            return dst_t( s ) ;
        }

        template<>
        static int8_t transform<float_t, int8_t>( float_t const s )
        {
            return int8_t( double(s) * 125.0 ) ;
        }

        template<>
        static int16_t transform<float_t, int16_t>( float_t const s )
        {
            return int16_t( double( s ) * 32768.0 ) ;
        }

        template<>
        static int32_t transform<float_t, int32_t>( float_t const s )
        {
            return int32_t( double( s ) * 2147483648.0 ) ;
        }
        

        template< typename T2 >
        range_t copy_from( byte_cptr_t dptr, size_t const num_frames )
        {
            so_typedefs( T2, src_type ) ;

            size_t const bits_per_sample = sizeof( type_t ) << 3 ;
            size_t const bytes_per_sample = sizeof( type_t ) ;

            src_type_cptr_t data = src_type_cptr_t( dptr ) ;

            _start = _end ;

            for( size_t i = 0; i < num_frames; ++i )
            {
                size_t const index = _num_channels * i ;
                for( size_t c = 0; c < _num_channels; ++c )
                {
                    type_t const val =
                        this_t::transform< T2, T>( data[ index + c ] );
                    
                    size_t const wi = _end ;
                    _samples[wi] = pcm_sample_t {val} ;

                    _end = ( _end + 1 ) % _samples.size() ;
                }
            }

            return range_t( _start, _end ) ;
        }

        void_t iterate( size_t const start, size_t const end, iterate_funk_t funk ) const
        {
            for( size_t i = start; i < end; ++i )
            {
                this_t::pcm_sample_t const smp { _samples[i] };
                funk( i-start, smp ) ;
            }
        }

        /// append the whole incoming pcm buffer to the end of this
        append_funk_t get_append_funk( void_t )
        {
            return [&]( pcm_sample_cref_t smp )
            {
                size_t const index = _end % _samples.size() ;
                _samples[ index ] = smp ;

                _end = ++_end % _samples.size() ;
                _start = _start == _end ? ++_start% _samples.size() : _start ;
            } ;
        }

        void_t for_each_new( for_each_new_funk_t f ) const
        {
            size_t const start = _end < _start ? _end : _start ;
            size_t const end = _end < _start ? _start : _end ;

            for( size_t i = start; i < end; ++i )
            {
                size_t const channel = i % _num_channels ;
                f( channel, _samples[ i ] ) ;
            }
        }
        
        size_t get_end_index( void_t ) const
        {
            return _end ;
        }

        size_t get_cur_index( void_t ) const
        {
            return _end ;
        }
    };
    so_typedefs( pcm_buffer< uint8_t >, pcm_buffer_uint8 ) ;
    so_typedefs( pcm_buffer< uint16_t >, pcm_buffer_uint16 ) ;
    so_typedefs( pcm_buffer< uint32_t >, pcm_buffer_uint32 ) ;

    so_typedefs( pcm_buffer< int8_t >, pcm_buffer_int8 ) ;
    so_typedefs( pcm_buffer< int16_t >, pcm_buffer_int16 ) ;
    so_typedefs( pcm_buffer< int32_t >, pcm_buffer_int32 ) ;

    so_typedefs( pcm_buffer< float_t >, pcm_buffer_float ) ;
}

#endif