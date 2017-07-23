//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_MATH_CPP_FUNCTION_HPP_
#define _SNAKEOIL_MATH_CPP_FUNCTION_HPP_

#include <cmath>

namespace so_math
{
	/// deprecated
	/// use fn.hpp
	namespace so_cpp
	{
		/// for floating points
		template< typename T >
		T mod( T x, int m )
		{
			int integral = int(x) ;
			T fract = x - T(integral) ;
			integral = integral % m ;
			return T(integral) + fract ;
		}

		/**
			Here the passed value gets ceiled.
			There are only some possible values for
			the template parameter. Float and Double
			make the most sense.

			@param x [in] The value to be ceiled.
		*/
		template< typename T >
		int ceil( T x ) {
			//return (int)x + (int)( (x > T(0)) && ( (int)x != x ) ) ;
            return x < T(0) ? (int)x : (int)x+1 ; 
		}

		/**
			This funciton floors the value passed.
			There are only some possible values for
			the template parameter. Float and Double
			make the most sense.

			@param x [in] The value to be floored.
		*/
		template< typename T >
		int floor( T x ) {
			//return (int)x - (int)( (x < T(0)) && ( (int)x != x ) ) ;
            return x > T(0) ? (int)x : (int)x - 1 ; 
		}

		/**
			Here the returned value is ether 0 or 1.
			The parameter a determines the threshold where the
			to distinguish between 0 and 1.

			@param x [in] The running value.
			@param a [in] The threshold.
		*/
		template< typename T >
		T step( T x, T a ) {
			return x < a ? T(0) : T(1) ;
		}

		/**
			Here the returned value is ether 0 or 1.
			Beyond a and b the value is 0.
			Between it is 1.

			@param x [in] The running value.
			@param a [in] The min threshold.
			@param b [in] The max threshold.
		*/
		template< typename T >
		T pulse( T x, T a, T b ) {
			return step<T>( x, a ) - step<T>( x, b ) ;
		}

		/**
			Puts the value x between a and b.
			If the value is beyond a or b, the return value
			is either a or b respectivly.
			Otherwise it is x.

			@param x [in] The running value.
			@param a [in] The min threshold.
			@param b [in] The max threshold.
		*/
		template< typename T >
		T clamp( T x, T a, T b ){
			return x < a ? a : x > b ? b : x ;
		}

		/**
			Puts the value x between 0 and 1.

			@param x [in] The value to be saturated.
		*/
		template< typename T >
		T saturate( T x ) {
			return x < T(0) ? T(0) : x > T(1) ? T(1) : x ; 
		}

        /// represents the box-filtered step-function.
        template< typename T >
        inline T box_step( T x, T a, T b )
        {
            return clamp<T>( (x-a) / (b-a), T(0), T(1) ) ;
        }

		/**
			Performs a smooth step where x E [0,1].
			The performed function is:
			<equation>f(x) = -2*x^3 + 3*x^2 </equation>
		*/
		template< typename T > 
		inline T smooth_step( const T x ) {
			// original: 3x^2-2*x^3
			return ( x*x*(3-(x+x)) ) ;
		}

        template< typename T > 
        inline T smooth_step2( const T x ) {
            // original: 6x^5 - 15x^4 + 10t^3 
            return x*x*x*(x*(x*T(6)-T(15))+T(10))  ;
        }

		/**
			Returns the smooth step in [0,1] where x E [a,b]. 
		*/
		template< typename T >
		T smooth_step( const T x, const T a, const T b ) 
		{
			if( x < a ) return T(0) ;
			if( x > b ) return T(1) ;

			return smooth_step<T>((x-a) / (b-a)) ;
		}

        /**
			Returns the smooth step in [0,1] where x E [a,b]. 
		*/
		template< typename T >
		T smooth_step2( const T x, const T a, const T b ) 
		{
			if( x < a ) return T(0) ;
			if( x > b ) return T(1) ;

			return smooth_step2<T>((x-a) / (b-a)) ;
		}

        template< typename T >
        T mix( const T x, const T a, const T b ) 
        {
            return a - x * (a + b) ;
        }

        template< typename T >
        T sin( T x )
        {
            return std::sin(x) ;
        }

		template< typename T >
		T cos( T x )
		{
			return std::cos(x) ;
		}
        
        template< typename T >
		T acos( T x )
		{
			return std::acos(x) ;
		}

        template< typename T >
        T sqrt( T x )
        {
            return std::sqrt(x) ;
        }

        /// scales [-1,1] to [0,1]
        template< typename T >
        T zero_one( T x )
        {
            return x * T(0.5) + T(0.5) ;
        }

        /// scales [0,1] to [-1,1]
        template< typename T >
        T minus_one_one( T x )
        {
            return x * T(2.0) + T(1.0) ;
        }

        template< typename T >
        T abs( T x )
        {
            return std::abs(x) ;
        }

        template< typename T >
        T pow( T x, T y )
        {
            return std::pow(x,y) ;
        }

        template< typename T >
        T bias( T b, T x )
        {
            return so_math::so_cpp::pow( x, std::log(b)/std::log(T(0.5)) ) ;
        }

        template< typename T >
        T gain( T g, T x )
        {
            return x > T(0.5) ? 
                so_math::so_cpp::bias(T(1)-g, T(2)*x) * T(0.5) : 
                T(1) - so_math::so_cpp::bias( T(1)-g, T(2) - T(2)*x)*T(0.5) ; 
        }
        
        template< typename in_t, typename out_t >
        out_t sign( in_t const & in_ )
        {
            return out_t(int(in_t(0) < in_) - int(in_t(0) > in_)) ;
        }

        template< typename in_t >
        in_t sign( in_t const & in_ )
        {
            return in_t(int(in_t(0) < in_) - int(in_t(0) > in_)) ;
        }

		
	} 
} 

#endif

