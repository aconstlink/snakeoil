//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_MANAGER_TYPEDEFS_H_
#define _SNAKEOIL_MANAGER_TYPEDEFS_H_

#include <snakeoil/std/string/string.hpp>
#include <snakeoil/std/container/list.hpp>

#include <snakeoil/memory/memory.h>
#include <snakeoil/memory/allocator.hpp>

#include <snakeoil/core/assert.h>
#include <snakeoil/core/types.hpp>
#include <snakeoil/core/macros/typedef.h>
#include <snakeoil/core/macros/move.h>

#include <algorithm>

namespace so_manager
{
    using namespace so_core::so_types ;
    using memory = so_memory::memory ;

    template< typename T >
    using allocator = so_memory::allocator< T, so_manager::memory > ;

    so_typedefs( so_std::string_t, string ) ;
    so_typedefs( so_std::string_t, key ) ;
    so_typedefs( so_std::list<key_t>, key_list ) ;
}

#endif
