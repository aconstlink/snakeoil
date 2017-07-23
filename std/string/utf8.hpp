//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_STD_STRING_utf8_H_
#define _SNAKEOIL_STD_STRING_utf8_H_

#include <snakeoil/memory/allocator.hpp>

#include <string>

namespace so_std
{
    //template< typename T >
    //using string = std::vector< T, so_memory::allocator<T> > ;

    // for now, we use the default allocator
    using utf8 = std::string ;


    so_typedefs( utf8, utf8 ) ;
}

#endif
