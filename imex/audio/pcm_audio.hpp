//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_IMEX_AUDIO_PCM_AUDIO_HPP_
#define _SNAKEOIL_IMEX_AUDIO_PCM_AUDIO_HPP_

#include "iaudio.h"

#include <snakeoil/std/container/vector.hpp>

namespace so_imex
{
    template< typename T >
    class pcm_audio : public iaudio
    {
        so_this_typedefs( pcm_audio<T> ) ;
        so_typedefs( T, type ) ;

    public:

        struct sample
        {
            T value ;
        };
        so_typedef( sample ) ;
        so_typedefs( so_std::vector< sample_t >, samples ) ;

        struct init_data
        {
            size_t num_samples_per_second ;
            size_t num_channels ;
        };
        so_typedef( init_data ) ;

    private:

        size_t _num_samples_per_sec = 0 ;
        size_t _num_channels = 0 ;
        samples_t _samples ;

    public:

        pcm_audio( void_t )
        {}

        pcm_audio( init_data_cref_t id )
        {
            _num_channels = id.num_channels ;
            _num_samples_per_sec = id.num_samples_per_second ;
        }

        pcm_audio( this_cref_t rhv )
        {
            _num_channels = rhv._num_channels ;
            _num_samples_per_sec = rhv._num_samples_per_sec ;
            _samples = rhv._samples ;
        }

        pcm_audio( this_rref_t rhv )
        {
            _num_channels = rhv._num_channels ;
            _num_samples_per_sec = rhv._num_samples_per_sec ;
            _samples = std::move( rhv._samples ) ;
        }

        virtual ~pcm_audio( void_t )
        {}

    public: 

        static this_ptr_t create( this_rref_t rhv, so_memory::purpose_cref_t p )
        {
            return so_imex::memory::alloc( std::move( rhv ), p ) ;
        }

        static void_t destroy( this_ptr_t ptr )
        {
            so_imex::memory::dealloc( ptr ) ;
        }

        void_t resize( size_t const num_samples )
        {
            _samples.resize( num_samples ) ;
        }

        bool_t set_sample( size_t const i, sample_cref_t s )
        {
            if( i >= _samples.size() )
                return false ;

            _samples[ i ] = s ;

            return true ;
        }

    public: // interface

        virtual double_t get_time_in_sec( void_t ) const
        {
            return double_t( _samples.size() ) / double_t(_num_samples_per_sec*_num_channels) ;
        }

        virtual byte_cptr_t get_memory_at( size_t const smp ) const
        {
            return byte_cptr_t( _samples.data() + smp * _num_channels ) ;
        }

        virtual size_t get_num_samples( void_t ) const
        {
            return _samples.size() ;
        }

        virtual size_t get_num_samples_per_second( void_t ) const
        {
            return _num_samples_per_sec ;
        }

        virtual size_t get_num_channels( void_t ) const
        {
            return _num_channels ;
        }

        virtual pcm_buffer_type get_type( void_t ) const
        {
            return buffer_type_helper<T>::deduce_buffer_type() ;
        }

        virtual void_t destroy( void_t )
        {
            this_t::destroy( this ) ;
        }
    };
}

#endif