//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GFX_TYPEDEFS_H_
#define _SNAKEOIL_GFX_TYPEDEFS_H_

#include <snakeoil/math/vector/vector2.hpp>
#include <snakeoil/math/vector/vector3.hpp>
#include <snakeoil/math/vector/vector4.hpp>

#include <snakeoil/math/matrix/matrix2.hpp>
#include <snakeoil/math/matrix/matrix3.hpp>
#include <snakeoil/math/matrix/matrix4.hpp>

#include <snakeoil/memory/global.h>
#include <snakeoil/memory/allocator.hpp>

#include <snakeoil/core/assert.h>
#include <snakeoil/core/types.hpp>
#include <snakeoil/core/macros/typedef.h>

#include <string>
#include <vector>
#include <algorithm>

namespace so_gfx
{
    using namespace so_core::so_types ;
    using memory = so_memory::global ;

    template< typename T >
    using allocator = so_memory::allocator< T, so_gfx::memory > ;
}

#endif

