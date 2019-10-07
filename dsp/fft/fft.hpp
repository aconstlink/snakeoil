
#pragma once

#include "../typedefs.h"

#include <snakeoil/math/utility/constants.hpp>

#include <cmath>
#include <complex>
#include <valarray>

namespace so_dsp
{
    // basic fft algorithm 
    // @see compute
    template< typename T >
    class fft
    {
        so_this_typedefs( fft<T> ) ;

    public:

        so_typedefs( std::complex<T>, complex ) ;
        so_typedefs( std::valarray<float_t>, samples ) ;
        so_typedefs( std::valarray<complex_t>, complexes ) ;

    private: // variables

        size_t _n = 2048 ;
        size_t _fi = 0 ;
        complexes_t _frequencies ;
        complexes_t _accumulate ;

    public:

        fft( void_t )
        {
            _frequencies.resize( _n ) ;
            _accumulate.resize( _n ) ;
        }

        // @param n should be pow2, if not, nearest upper 
        // bound is taken
        fft( size_t const n )
        {
            _n = std::pow( 2, size_t( std::floor( std::log2( n ) ) ) + 1 ) ;
            _frequencies.resize( _n ) ;
            _accumulate.resize( _n ) ;
        }

        fft( this_cref_t rhv )
        {
            _n = rhv._n ;
            _fi = rhv._fi ;
            _frequencies = rhv._frequencies ;
            _accumulate = rhv._accumulate ;
        }

        fft( this_rref_t rhv )
        {
            _n = rhv._n ;
            rhv._n = 0 ;
            _fi = rhv._fi ;
            rhv._fi = 0 ;
            _frequencies = std::move( rhv._frequencies ) ;
            _accumulate = std::move( rhv._accumulate ) ;
        }

        size_t n( void_t ) const { return _n ; }

    public:

        // this function puts any number of samples into a 
        // fft computable buffer and accumulates all 
        // frequencies. If data is available, frequencies will
        // be filled with the accumulated frequencies and true is
        // returned, otherwise false.
        bool_t update( samples_cref_t smps, complexes_out_t frequencies )
        {
            size_t nsmps = smps.size() ;
            size_t num_copy = std::min( _n - _fi, nsmps ) ;

            size_t start_copy = 0 ;


            if( frequencies.size() < _n ) frequencies.resize( _n ) ;

            bool_t have_new = false ;
            while( num_copy != 0 )
            {
                for( size_t i = 0; i < num_copy; ++i )
                {
                    _frequencies[ _fi + i ] = std::complex<double_t>( 
                        smps[ start_copy + i ], T(0.0) ) ;
                }
                _fi += num_copy ;
                start_copy += num_copy ;

                if( _fi == _n )
                {
                    this_t::compute( _frequencies ) ;
                    for( size_t i = 0; i < _n; ++i )
                    {
                        _accumulate[ i ] += _frequencies[ i ] ;
                    }
                    _fi = 0 ;
                    have_new = true ;
                }
                nsmps -= num_copy ;
                num_copy = std::min( _n - _fi, nsmps )  ;
            }

            if( have_new )
            {
                for( size_t i = 0; i < _n; ++i )
                {
                    frequencies[ i ] = _accumulate[ i ] ;
                    _accumulate[ i ] = std::complex<double_t>( 0.0, 0.0 ) ;
                }
            }

            return have_new ;
        }

    public: // static 

        // Cooley�Tukey FFT (in-place, divide-and-conquer)
        // Higher memory requirements and redundancy although more intuitive
        // code taken from rosettastone website
        static void_t compute( complexes_inout_t x )
        {
            size_t const N = x.size();
            if( N <= 1 ) return;

            // divide
            complexes_t even = x[ std::slice( 0, N / 2, 2 ) ];
            complexes_t  odd = x[ std::slice( 1, N / 2, 2 ) ];

            // conquer
            this_t::compute( even );
            this_t::compute( odd );

            // combine
            for( size_t k = 0; k < N / 2; ++k )
            {
                complex_t t = std::polar( T(1.0), T(-2.0) * so_math::constants<T>::pi() * 
                    T( k ) / T( N ) ) * odd[ k ];
                x[ k ] = even[ k ] + t;
                x[ k + N / 2 ] = even[ k ] - t;
            }
        }
    };
}