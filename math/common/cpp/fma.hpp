//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_MATH_CPP_FMA_HPP_
#define _SNAKEOIL_MATH_CPP_FMA_HPP_

namespace so_math
{
	 namespace so_cpp
	 {
		 /**
			Fused Multiply Add

			return a + b * c 
		*/
		template< typename type_t >
		inline type_t fma( type_t a, type_t b, type_t c ){
			return a + b * c ;
		}
	 } ;
} ;

#endif

