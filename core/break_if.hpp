//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_CORE_BREAK_IF_HPP_
#define _SNAKEOIL_CORE_BREAK_IF_HPP_

#include "types.hpp"

#include "assert.h"
#include "boolean.h"

#include <cstdlib>

namespace so_core
{
    using namespace so_types ;

    static void_t break_if( bool_t condition )
    {
#if defined(SNAKEOIL_DEBUG)
        assert( so_core::is_not(condition) ) ;
#else
        if( condition )
            exit( EXIT_FAILURE ) ;
#endif
    }
}

#endif


