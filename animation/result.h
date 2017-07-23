//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_ANIMATION_RESULT_H_
#define _SNAKEOIL_ANIMATION_RESULT_H_

#include <snakeoil/std/string/string.hpp>
#include <snakeoil/core/boolean.h>

namespace so_ani
{
    enum result
    {
        ok,
        failed,
        invalid_argument,
        num_results
    };

    namespace so_internal
    {
        static const so_std::string_t result_array[] = {
            "so_ani::result"
        } ;
    }

    static so_std::string_cref_t to_string( result res )
    {
        return so_internal::result_array[ 0 ] ;
    }

    static bool success( result r )
    { 
        return r == ok ; 
    }

    static bool no_success( result r ) { return so_core::is_not(success(r)) ; }

}

#endif

