//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include <snakeoil/gfx/typedefs.h>

#include <snakeoil/io/typedefs.h>
#include <snakeoil/std/filesystem/filesystem.hpp>

namespace sox
{
    namespace so_internal
    {
        so_std::string_t const __shader_search_path_base[] = 
        {
            "./sox",
            "@CMAKE_CURRENT_SOURCE_DIR@"
        } ;
    }

    static so_io::path_t find_shader_path( so_std::string_cref_t append ) 
    {
        for( auto const & p : so_internal::__shader_search_path_base )
        {
            so_io::path_t const path = so_io::path_t( p + append ) ;
            if( so_std::filesystem::exists( path ) )
                return path ;
        }

        return so_io::path_t() ;
    }
}
