//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_FONT_RESULT_H_
#define _SNAKEOIL_FONT_RESULT_H_

#include "typedefs.h"
#include <string>

namespace so_font
{
    enum result
    {
        ok,
        failed,
        invalid,
        invalid_argument,
        not_ready
    };

    namespace so_internal
    {
        static const std::string __graphics_result_strings[] = {
            "so_font::result"
        } ;
    }

    /// not implemented yet.
    static std::string const & to_string( result /*res*/ ) 
    {
        return so_internal::__graphics_result_strings[0] ;
    }

    static bool_t success( so_font::result res )
    {
        return res == ok ;
    }

    static bool_t no_success( result res )
    {
        return !success( res ) ;
    }
}

#endif

