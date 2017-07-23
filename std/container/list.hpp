//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_STD_CONTAINER_LIST_H_
#define _SNAKEOIL_STD_CONTAINER_LIST_H_

#include <snakeoil/memory/allocator.hpp>

#include <list>

namespace so_std
{
    //template< typename T >
    //using list = std::list< T, so_memory::allocator<T> > ;

    // for now, we use the default allocator
    template< typename T >
    using list = std::list< T > ;
}

#endif
