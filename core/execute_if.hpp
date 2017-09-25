//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "boolean.h"
#include "execute_result.h"

namespace so_core
{
    static execute_result execute_if( bool_t cond, std::function< so_core::execute_result ( void_t ) > funk )
    {
        if( so_core::is_not( cond ) ) 
            return execute_result::cond_false ;

        return funk() ;
    }
}


