//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_STD_CONTAINER_MAP_H_
#define _SNAKEOIL_STD_CONTAINER_MAP_H_

//#include <snakeoil/memory/allocator.hpp>

#include <map>

namespace so_std
{
    //template< typename T >
    //using map = std::map< T, so_memory::allocator<T> > ;

    // for now, we use the default allocator
    template< typename T, typename G >
    using map = std::map< T, G > ;
}

#endif
