//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_SHADE_SHADER_BINDING_GEOMETRY_BINDING_H_
#define _SNAKEOIL_SHADE_SHADER_BINDING_GEOMETRY_BINDING_H_

#include "../../variable.h"

namespace so_shade
{
    namespace so_shader
    {
        struct position_binding
        {
            so_shade::variable_t var ;
        };
        so_typedef( position_binding ) ;

        struct normal_binding
        {
            so_shade::variable_t var ;
        };
        so_typedef( normal_binding ) ;

        struct color_binding
        {
            so_shade::variable_t var ;
        };
        so_typedef( color_binding ) ;

        struct texcoord_binding
        {
            so_shade::variable_t var ;
        };
        so_typedef( texcoord_binding ) ;
    }
}

#endif
