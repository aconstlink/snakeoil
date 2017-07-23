//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_IO_RESULT_H_
#define _SNAKEOIL_IO_RESULT_H_

#include <string>

namespace so_io
{
    enum result
    {
        ok,
        failed,
        processing,
        state_change_failed,
        file_does_not_exist,
        invalid,
        invalid_argument,
        unknown,
        num_results
    };

    static bool success( result r ){ return r == ok ; }
    static bool no_success( result r ) { return !success(r) ; }

    namespace so_internal
    {
        static std::string const __result_strings[] = {
            "ok", "failed", "processing", "state_change_failed",
            "file_does_not_exist", "invalid","invalid_argument","unknown"
        } ;
    }

    static std::string const & to_string( so_io::result r )
    {
        return size_t(r) < size_t(num_results) ? 
            so_internal::__result_strings[size_t(r)] : 
            so_internal::__result_strings[size_t(num_results)-1] ;
    }

}

#endif

