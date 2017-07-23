//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_SHADE_TREE_FUNK_TEXTURE_FUNK_NODE_H_
#define _SNAKEOIL_SHADE_TREE_FUNK_TEXTURE_FUNK_NODE_H_

#include "../../protos.h"
#include "../../typedefs.h"
#include "../../api.h"

namespace so_shade
{
    namespace so_tree
    {
        struct SNAKEOIL_SHADE_API texture_funk
        {
            static so_shade::so_tree::funk_node_ptr_t create_texture_access( void_t ) ;            
        };
    }
}

#endif
