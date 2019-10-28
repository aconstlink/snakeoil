//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "typedefs.h"
#include <string>

namespace sox_presentation
{
    enum class result
    {
        ok,
        failed,
        invalid
    };

    namespace so_internal
    {
        static const std::string __graphics_result_strings[] = {
            "sox_presentation::result"
        } ;
    }

    /// not implemented yet.
    static std::string const & to_string( sox_presentation::result /*res*/ ) 
    {
        return so_internal::__graphics_result_strings[0] ;
    }

    static bool_t success( sox_presentation::result res )
    {
        return res == result::ok ;
    }

    static bool_t no_success( result res )
    {
        return !success( res ) ;
    }
}