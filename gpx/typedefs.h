//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPX_TYPEDEFS_H_
#define _SNAKEOIL_GPX_TYPEDEFS_H_

#include <snakeoil/math/vector/vector2.hpp>
#include <snakeoil/math/vector/vector3.hpp>
#include <snakeoil/math/vector/vector4.hpp>

#include <snakeoil/math/matrix/matrix2.hpp>
#include <snakeoil/math/matrix/matrix3.hpp>
#include <snakeoil/math/matrix/matrix4.hpp>

#include <snakeoil/std/string/string.hpp>
#include <snakeoil/std/container/vector.hpp>

#include <snakeoil/memory/memory.h>
#include <snakeoil/memory/allocator.hpp>

#include <snakeoil/core/assert.h>
#include <snakeoil/core/types.hpp>
#include <snakeoil/core/macros/typedef.h>

namespace so_gpx
{
    using namespace so_core::so_types ;
    using memory = so_memory::memory ;

    template< typename T >
    using allocator = so_memory::allocator< T, so_gpx::memory > ;
}

#endif

