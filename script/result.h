//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_SCRIPT_RESULT_H_
#define _SNAKEOIL_SCRIPT_RESULT_H_

#include <snakeoil/std/string/string.hpp>

namespace so_script
{
    enum result
    {
        ok,
        failed,
        invalid_argument,
        invalid_argname,
        invalid_pointer        
    };

    namespace so_internal
    {
        static const so_std::string_t result_array[] = {
            "ok", "failed", "invalid_argument", "invalid_argname",
            "invalid_pointer"
        } ;
    }

    static so_std::string_cref_t to_string( result res )
    {
        return so_internal::result_array[size_t(res)] ;
    }

    static bool success( result r ){ return r == ok ; }
    static bool no_success( result r ) { return !success(r) ; }

}

#endif

