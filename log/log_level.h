//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_LOG_LOG_LEVEL_H_
#define _SNAKEOIL_LOG_LOG_LEVEL_H_

#include <string>

namespace so_log
{
    enum class log_level
    {
        raw,
        status,
        warning,
        error,
        critical,
        timing
    };

    namespace so_internal
    {
        static std::string const __log_level_strings[] = {
            "raw", "status", "warning", "error", "critical", "timing"
        } ;
        static std::string const __log_level_short_strings[] = {
            "", "[s]", "[w]", "[e]", "[c]", "[t]"
        } ;
    }

    static std::string const & to_string( log_level level )
    {
        return so_internal::__log_level_strings[size_t(level)] ;
    }

    static std::string const & to_string_short( log_level level )
    {
        return so_internal::__log_level_short_strings[size_t(level)] ;
    }

}

#endif

