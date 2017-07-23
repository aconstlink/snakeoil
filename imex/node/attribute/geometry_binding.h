//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_IMEX_NODE_ATTRIBUTE_GEOMETRY_BINDING_H_
#define _SNAKEOIL_IMEX_NODE_ATTRIBUTE_GEOMETRY_BINDING_H_

#include "../../typedefs.h"

namespace so_imex
{
    namespace so_node
    {
        enum class geometry_binding_point
        {
            undefined,
            position,
            normal,
            color0,
            color1,
            color2,
            texcoord0,
            texcoord1,
            texcoord2
        };

        struct geometry_binding
        {
            geometry_binding_point binding_point;
            so_std::string_t name;
        };
        so_typedef( geometry_binding ) ;
    }
}

#endif
