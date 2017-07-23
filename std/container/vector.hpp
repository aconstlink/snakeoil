//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_STD_CONTAINER_VECTOR_H_
#define _SNAKEOIL_STD_CONTAINER_VECTOR_H_

#include <snakeoil/memory/allocator.hpp>

#include <vector>

namespace so_std
{
    //template< typename T >
    //using vector = std::vector< T, so_memory::allocator<T> > ;

    // for now, we use the default allocator
    template< typename T >
    using vector = std::vector< T > ;
}

#endif
