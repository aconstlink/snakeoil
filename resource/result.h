//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_RESOURCE_RESULT_H_
#define _SNAKEOIL_RESOURCE_RESULT_H_

#include "typedefs.h"

namespace so_resource
{
    enum result
    {
        ok,
        failed,
        invalid_argument,
        acquired,
        reserved,
        invalid,
        found,
        invalid_refcount,
        already_managed,
        inserted,
        exchanged
    };

    namespace so_internal
    {
        static const so_std::string_t __graphics_result_strings[] = {
            "so_resource::result"
        } ;
    }

    /// not implemented yet.
    static so_std::string_cref_t to_string( result /*res*/ ) 
    {
        return so_internal::__graphics_result_strings[0] ;
    }

    static bool_t success( so_resource::result res )
    {
        return res == ok ;
    }

    static bool_t no_success( result res )
    {
        return !success( res ) ;
    }
}

#endif

