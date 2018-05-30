//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include <snakeoil/std/string/string.hpp>

namespace so_gfx
{
    enum class predef_post_type
    {
        invalid,
        toscreen,
        vignette,
        downsample,
        blur_box,
        blur_gauss
    };

    namespace so_internal
    {
        static so_std::string_t const __predef_post_type_names[] = { 
            "invalid", "toscreen", "vignette", "downsample", "blur_box", "blur_gauss"
        } ;
    }
    static so_std::string_t to_string( predef_post_type const t )
    {
        return so_internal::__predef_post_type_names[ size_t( t ) ] ;
    }
}