//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_FLOW_RESULT_H_
#define _SNAKEOIL_FLOW_RESULT_H_

#include <snakeoil/std/string/string.hpp>
#include <snakeoil/core/boolean.h>

namespace so_flow
{
    enum result
    {
        ok,
        failed,
        invalid_argument,
        invalid_pointer,
        invalid_argname,
        already_connected,
        already_disconnected,
        already_in_use,
        not_found,
        busy_updating,
        busy_operation,
        name_taken,
        slot_is_touched,
        num_results
    };

    namespace so_internal
    {
        static const so_std::string_t result_array[] = {
            "ok", "failed", "invalid_argument", "invalid_pointer",
            "already_connected", "already_disconnected", "not_found",
            "busy_updating","busy_operation", "name_taken","slot_is_touched"
        } ;
    }

    static so_std::string_cref_t to_string( result res )
    {
        return so_internal::result_array[
            res >= num_results ? size_t(num_results) : size_t(res) ] ;
    }

    static bool success( result r )
    { 
        return 
            r == ok || 
            r == already_connected ||
            r == already_disconnected ; 
    }

    static bool no_success( result r ) { return so_core::is_not(success(r)) ; }

}

#endif

