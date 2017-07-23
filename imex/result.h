//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_IMEX_RESULT_H_
#define _SNAKEOIL_IMEX_RESULT_H_

#include "typedefs.h"
#include <string>

namespace so_imex
{
    enum result
    {
        ok,
        failed,
        invalid,
        invalid_argument,
        key_in_use,
        already_managed,
        ref_count_not_null,
        processing,
        not_ready,
        pre_exit,
        not_implemented,
        extension_not_supported,
        acquired,
        reserved,
        inserted,
        exchanged,
        found,
        path_does_not_exist,
        invalid_cast
    };

    namespace so_internal
    {
        static const std::string __graphics_result_strings[] = {
            "so_imex::result"
        } ;
    }

    /// not implemented yet.
    static std::string const & to_string( result /*res*/ ) 
    {
        return so_internal::__graphics_result_strings[0] ;
    }

    static bool_t success( so_imex::result res )
    {
        return res == ok ;
    }

    static bool_t no_success( result res )
    {
        return !success( res ) ;
    }
}

#endif

