//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include <snakeoil/std/string/string.hpp>
#include <snakeoil/core/boolean.h>

namespace so_dsp
{
    enum class result
    {
        ok,
        failed,
        invalid_argument,
        invalid_pointer,
        num_results
    } ;

    namespace so_internal
    {
        static const so_std::string_t result_array[] = {
            "ok", "failed", "invalid_argument", "invalid_pointer"
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
            r == result::ok || 
            r == result::already_connected ||
            r == result::already_disconnected ; 
    }

    static bool no_success( result r ) { return so_core::is_not(success(r)) ; }

}