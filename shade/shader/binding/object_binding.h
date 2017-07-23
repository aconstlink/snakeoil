//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_SHADE_SHADER_BINDING_OBJECT_BINDING_H_
#define _SNAKEOIL_SHADE_SHADER_BINDING_OBJECT_BINDING_H_

#include "../../variable.h"

namespace so_shade
{
    namespace so_shader
    {
        enum class object_binding_point
        {
            undefined,
            object_matrix,
            world_matrix
        };

        struct object_binding
        {
            so_shade::variable_t var ;
            object_binding_point binding_point ;
        };
        so_typedef( object_binding ) ;
    }
}

#endif
