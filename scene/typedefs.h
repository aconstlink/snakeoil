//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_SCENE_TYPEDEFS_H_
#define _SNAKEOIL_SCENE_TYPEDEFS_H_

#include <snakeoil/std/string/string.hpp>

#include <snakeoil/memory/memory.h>
#include <snakeoil/memory/allocator.hpp>

#include <snakeoil/log/typedefs.h>

#include <snakeoil/core/macros/typedef.h>
#include <snakeoil/core/macros/move.h>
#include <snakeoil/core/types.hpp>

namespace so_scene
{
    using namespace so_core::so_types ;

    using memory = so_memory::memory ;

    template< typename T >
    using allocator = so_memory::allocator< T, so_scene::memory > ;
}

#endif

