//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_MANAGER_RESULT_H_
#define _SNAKEOIL_MANAGER_RESULT_H_

#include "typedefs.h"
#include <string>

namespace so_manager
{
    enum result
    {
        ok,
        failed,
        invalid_argument,
        program_does_not_exist,
        program_already_exists,
        program_already_linking,
        key_already_in_use,
        reserved,
        found,
        not_ready,
        not_implemented,
        under_construction
    };

    namespace so_internal
    {
        static const std::string __graphics_result_strings[] = {
            "so_manager::result"
        } ;
    }

    /// not implemented yet.
    static std::string const & to_string( result /*res*/ ) 
    {
        return so_internal::__graphics_result_strings[0] ;
    }

    static bool_t success( so_manager::result res )
    {
        return res == ok ;
    }

    static bool_t no_success( result res )
    {
        return !success( res ) ;
    }
}

#endif
