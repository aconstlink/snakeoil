//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_STD_STRING_STRING_H_
#define _SNAKEOIL_STD_STRING_STRING_H_

//#include <snakeoil/memory/allocator.hpp>

#include <snakeoil/core/macros/typedef.h>
#include <string>

namespace so_std
{
    //template< typename T >
    //using string = std::vector< T, so_memory::allocator<T> > ;

    // for now, we use the default allocator
    using string = std::string ;

    so_typedefs( string, string ) ;
}

#endif
