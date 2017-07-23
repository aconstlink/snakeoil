//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_CORE_TYPES_TYPE_TRAITS_HPP_
#define _SNAKEOIL_CORE_TYPES_TYPE_TRAITS_HPP_

namespace so_core
{
	template< typename T >
	struct type_traits
	{
		typedef T type_t ;
		typedef T t ;
		
		typedef type_t * ptr_t ;
		typedef type_t const * cptr_t ;

		typedef type_t & ref_t ;
		typedef type_t const & cref_t ;

	} ;
}

#endif



