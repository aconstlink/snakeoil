//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "typedefs.h"
#include <string>

namespace so_device
{
    enum result
    {
        ok,
        failed,
        invalid_argument,
        no_default,
        not_implemented
    };

    static bool_t success( so_device::result r )
    {
        return r == ok ;
    }

    static bool_t no_success( so_device::result r )
    {
        return !success(r) ;
    }

    namespace so_internal
    {
        static const std::string __result_strings[] = {
            "no-strings"
        } ;
    }

    static std::string const & to_string( so_device::result /*r*/ )
    {
        return so_internal::__result_strings[0] ;
    }
}
