//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_MATH_FAST_HPP_
#define _SNAKEOIL_MATH_FAST_HPP_

namespace so_math
{
	namespace so_internal
	{
		template< typename T, size_t by >
		struct _fast
		{
			static T abs( T x ) { return x < T(0) ? -x : x ; }
		};

		template<>
		struct _fast<float,4>
		{
			static float abs( float x ) { return x < 0.0f ? -x : x ; }
		};
	}

	/// deprecated
	template< typename T >
	struct fast
	{
		static T abs( T x ) { return so_internal::_fast<T, sizeof(T)>::abs(x) ; }
	};
}

#endif

