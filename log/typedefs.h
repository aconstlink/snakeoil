//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_LOG_TYPEDEFS_H_
#define _SNAKEOIL_LOG_TYPEDEFS_H_

#include <snakeoil/core/types.hpp>
#include <snakeoil/core/assert.h>
#include <snakeoil/core/macros/typedef.h>
#include <snakeoil/core/macros/move.h>
#include <snakeoil/core/checks.h>
#include <snakeoil/core/break_if.hpp>

#include <string>

namespace so_log
{
    using namespace so_core ;
    using namespace so_core::so_types ;

    so_typedefs( std::string, string ) ; 
}

#endif

