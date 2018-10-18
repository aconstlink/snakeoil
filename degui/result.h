//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "typedefs.h"
#include <string>

namespace so_imgui
{
    enum result
    {
        ok,
        failed,
    };

    namespace so_internal
    {
        static const std::string __graphics_result_strings[] = {
            "so_imgui::result"
        } ;
    }

    /// not implemented yet.
    static std::string const & to_string( result /*res*/ ) 
    {
        return so_internal::__graphics_result_strings[0] ;
    }

    static bool_t success( so_imgui::result res )
    {
        return res == ok ;
    }

    static bool_t no_success( result res )
    {
        return !success( res ) ;
    }
}


