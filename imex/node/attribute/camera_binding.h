//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_IMEX_NODE_ATTRIBUTE_CAMERA_BINDING_H_
#define _SNAKEOIL_IMEX_NODE_ATTRIBUTE_CAMERA_BINDING_H_

#include "../../typedefs.h"

namespace so_imex
{
    namespace so_node
    {
        enum class camera_binding_point
        {
            undefined,
            camera_matrix,              // mat4
            camera_orientation_matrix,  // mat3
            view_matrix,                // mat4
            projection_matrix,          // mat4
            mvp_matrix                  // mat4
        };

        struct camera_binding
        {
            camera_binding_point binding_point ;
            so_std::string_t name ;
        };
        so_typedef( camera_binding ) ;
    }
}

#endif
