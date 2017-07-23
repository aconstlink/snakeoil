//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_IMEX_NODE_ATTRIBUTE_TRAFO_BINDING_H_
#define _SNAKEOIL_IMEX_NODE_ATTRIBUTE_TRAFO_BINDING_H_

#include "../../typedefs.h"

namespace so_imex
{
    namespace so_node
    {
        enum class trafo_binding_point
        {
            undefined,
            object_matrix,      // mat4
            world_matrix        // mat4
        };

        struct trafo_binding
        {
            trafo_binding_point binding_point ;
            so_std::string_t name ;
        };
        so_typedef( trafo_binding ) ;
    }
}

#endif
