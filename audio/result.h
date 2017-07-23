//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_AUDIO_RESULT_H_
#define _SNAKEOIL_AUDIO_RESULT_H_

#include "typedefs.h"
#include <string>

namespace so_audio
{
    enum result
    {
        ok,
        failed,
        invalid_argument,
        not_implemented,
        no_default
    };

    static bool_t success( so_audio::result r )
    {
        return r == ok ;
    }

    static bool_t no_success( so_audio::result r )
    {
        return !success(r) ;
    }

    namespace so_internal
    {
        static const std::string __result_strings[] = {
            "no-strings"
        } ;
    }

    static std::string const & to_string( so_audio::result /*r*/ )
    {
        return so_internal::__result_strings[0] ;
    }


}

#endif

