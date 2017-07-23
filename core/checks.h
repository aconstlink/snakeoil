//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_CORE_CHECKS_H_
#define _SNAKEOIL_CORE_CHECKS_H_

#include "types.hpp"

namespace so_core
{
    using namespace so_core::so_types ;

    template< typename T >
    static bool_t is_nullptr( T const * const ptr )
    {
        return ptr == nullptr ;
    }

    template< typename T >
    static bool_t is_not_nullptr( T const * const ptr )
    {
        return ptr != nullptr ;
    }
}

#endif


