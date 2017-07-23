//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GFX_RESULT_H_
#define _SNAKEOIL_GFX_RESULT_H_

#include "typedefs.h"
#include <string>

namespace so_gfx
{
    enum result
    {
        ok,
        failed,
        invalid_argument
    };

    namespace so_internal
    {
        static const std::string __graphics_result_strings[] = {
            "so_gfx::result"
        } ;
    }

    /// not implemented yet.
    static std::string const & to_string( result /*res*/ ) 
    {
        return so_internal::__graphics_result_strings[0] ;
    }

    static bool_t success( so_gfx::result res )
    {
        return res == ok ;
    }

    static bool_t no_success( result res )
    {
        return !success( res ) ;
    }
}

#endif

