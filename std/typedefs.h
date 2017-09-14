//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_STD_TYPEDEFS_H_
#define _SNAKEOIL_STD_TYPEDEFS_H_

#include <snakeoil/io/typedefs.h>

#include <snakeoil/memory/global.h>
#include <snakeoil/memory/allocator.hpp>

#include <snakeoil/std/container/list.hpp>

#include <snakeoil/core/assert.h>
#include <snakeoil/core/types.hpp>
#include <snakeoil/core/macros/typedef.h>
#include <snakeoil/core/macros/move.h>
#include <snakeoil/core/boolean.h>
#include <snakeoil/core/checks.h>

#include <algorithm>

namespace so_std
{
    using namespace so_core::so_types ;

    so_typedefs( std::string, string ) ;
    so_typedefs( std::string, key ) ;
    so_typedefs( so_std::list<key_t>, key_list ) ;

    using memory = so_memory::global ;
}

#endif

