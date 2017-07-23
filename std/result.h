//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_STD_RESULT_H_
#define _SNAKEOIL_STD_RESULT_H_

#include "typedefs.h"
#include <string>

namespace so_std
{
    enum result
    {
        ok,
        failed,
        invalid,
        invalid_argument
    };

    namespace so_internal
    {
        static const std::string __result_strings[] = {
            "so_std::result"
        } ;
    }

    /// not implemented yet.
    static std::string const & to_string( result /*res*/ ) 
    {
        return so_internal::__result_strings[0] ;
    }

    static bool_t success( so_std::result res )
    {
        return res == ok ;
    }

    static bool_t no_success( result res )
    {
        return !success( res ) ;
    }
}

#endif

