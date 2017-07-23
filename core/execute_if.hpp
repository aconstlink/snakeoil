//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_CORE_EXECUTE_IF_HPP_H_
#define _SNAKEOIL_CORE_EXECUTE_IF_HPP_H_

#include "boolean.h"

namespace so_core
{
    static bool_t execute_if( bool_t cond, std::function< bool_t ( void_t ) > funk )
    {
        if( so_core::is_not( cond ) ) return cond ;
        return funk() ;
    }
}

#endif


