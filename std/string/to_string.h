//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_CORE_STRING_TO_STRING_H_
#define _SNAKEOIL_CORE_STRING_TO_STRING_H_

#include "../types.hpp"

#include <string>

namespace so_core
{
    namespace so_internal
    {
        static std::string const _bool_strings[] = { "true", "false" } ;
    }

    struct to_string
    {
        static std::string const & of( bool_t b )
        {
            return so_internal::_bool_strings[b?0:1] ;
        }
    };
}

#endif

