//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_FONT_TYPEDEFS_H_
#define _SNAKEOIL_FONT_TYPEDEFS_H_

#include <snakeoil/std/container/list.hpp>

#include <snakeoil/memory/memory.h>
#include <snakeoil/memory/allocator.hpp>

#include <snakeoil/math/vector/vector2.hpp>
#include <snakeoil/math/vector/vector4.hpp>

#include <snakeoil/core/assert.h>
#include <snakeoil/core/types.hpp>
#include <snakeoil/core/macros/typedef.h>

#include <algorithm>

namespace so_font
{
    using namespace so_core::so_types ;
    using memory = so_memory::memory ;

    template< typename T >
    using allocator = so_memory::allocator< T, so_font::memory > ;

    so_typedefs( uint8_t, point_size ) ;
    so_typedefs( so_math::vec2ui_t, dpi ) ;
    so_typedefs( so_math::vec2ui_t, image_bounds ) ;
    so_typedefs( so_math::vec4ui_t, bounding_box_ui ) ;

    so_typedefs( uint32_t, utf32 ) ;
}

#endif

