//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_SHADE_CODE_CODE_TYPE_H_
#define _SNAKEOIL_SHADE_CODE_CODE_TYPE_H_

#include <snakeoil/std/string/string.hpp>

namespace so_shade
{
    enum class code_type
    {
        unknown,
        glsl_33,
        hlsl,
        osl,    
        renderman,
        num_code_types
    };

    namespace so_internal
    {
        static const so_std::string_t __code_type_strings[] = {
            "code_type"
        } ;
    }

    static so_std::string_cref_t to_string( code_type ct )
    {
        return so_internal::__code_type_strings[0] ;
    }
}

#endif
