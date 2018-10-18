//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GLI_RESULT_H_
#define _SNAKEOIL_GLI_RESULT_H_

#include "typedefs.h"
#include <string>

namespace so_gli
{
    enum result
    {
        ok,
        failed,
        failed_load_function,
        num_results
    };

    typedef result * result_ptr_t ;

    namespace so_internal
    {
        static const std::string __graphics_result_strings[] = {
            "so_gli::result"
        } ;
    }

    /// not implemented yet.
    static std::string const & to_string( result /*res*/ ) 
    {
        return so_internal::__graphics_result_strings[0] ;
    }

    static bool_t success( so_gli::result res )
    {
        return res == ok ;
    }

    static bool_t no_success( result res )
    {
        return !success( res ) ;
    }
}

#endif

