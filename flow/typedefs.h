//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_FLOW_TYPEDEFS_H_
#define _SNAKEOIL_FLOW_TYPEDEFS_H_

#include <snakeoil/std/string/string.hpp>

#include <snakeoil/memory/memory.h>
#include <snakeoil/memory/allocator.hpp>

#include <snakeoil/log/typedefs.h>

#include <snakeoil/core/macros/typedef.h>
#include <snakeoil/core/macros/move.h>
#include <snakeoil/core/types.hpp>
#include <snakeoil/core/checks.h>
#include <snakeoil/core/boolean.h>

namespace so_flow
{
    using namespace so_core::so_types ;

    using memory = so_memory::memory ;

    template< typename T >
    using allocator = so_memory::allocator< T, so_flow::memory > ;

    so_typedefs( so_std::string_t, key ) ;
}

#endif

