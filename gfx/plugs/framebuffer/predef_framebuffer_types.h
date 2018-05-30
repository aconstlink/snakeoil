//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include <snakeoil/std/string/string.hpp>

namespace so_gfx
{
    enum class predef_framebuffer_type
    {
        invalid,
        depth32,
        color888_alpha8,
        color888_alpha8_depth32
    };

    namespace so_internal
    {
        static so_std::string_t const __predef_framebuffer_type_names[] = {
            "invalid", "depth32", "color888_alpha8",  "color888_alpha8_depth32"
        } ;
    }
    static so_std::string_t to_string( predef_framebuffer_type const t )
    {
        return so_internal::__predef_framebuffer_type_names[ size_t( t ) ] ;
    }
}