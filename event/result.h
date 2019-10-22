//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include <string>
#include "typedefs.h"

namespace so_event
{
    enum class result
    {
        ok,
        failed,
        invalid,
        invalid_argument,
        processing,
        not_ready,
        name_taken,
        trigger_already_added
    };

    namespace so_internal
    {
        static const std::string __graphics_result_strings[] = {
            "so_imex::result"
        } ;
    }

    /// not implemented yet.
    static std::string const & to_string( so_event::result /*res*/ ) 
    {
        return so_internal::__graphics_result_strings[0] ;
    }

    static bool_t success( so_event::result res )
    {
        return res == so_event::result::ok ;
    }

    static bool_t no_success( so_event::result res )
    {
        return !success( res ) ;
    }
}
