//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_CORE_BOOLEAN_H_
#define _SNAKEOIL_CORE_BOOLEAN_H_

#include "types.hpp"

namespace so_core
{
    using namespace so_core::so_types ;

    static bool_t is_not( bool_t const b )
    {
        return !b ;
    }   

    static bool_t is( bool_t const b )
    {
        return b;
    }
}

#endif


