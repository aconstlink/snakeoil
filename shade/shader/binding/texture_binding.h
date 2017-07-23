//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_SHADE_SHADER_TEXTURE_BINDING_H_
#define _SNAKEOIL_SHADE_SHADER_TEXTURE_BINDING_H_

#include "../../variable.h"

namespace so_shade
{
    namespace so_shader
    {        
        struct texture_binding
        {
            so_shade::texture_variable_t var ;
        };
        so_typedef( texture_binding ) ;
    }
}

#endif
