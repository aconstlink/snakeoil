//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_SHADE_TREE_FUNK_MATH_FUNK_NODE_H_
#define _SNAKEOIL_SHADE_TREE_FUNK_MATH_FUNK_NODE_H_

#include "../../protos.h"
#include "../../typedefs.h"
#include "../../api.h"

namespace so_shade
{
    namespace so_tree
    {
        struct SNAKEOIL_SHADE_API math_funk
        {
            static so_shade::so_tree::funk_node_ptr_t create_cos( void_t ) ;
            static so_shade::so_tree::funk_node_ptr_t create_smoothstep( void_t ) ;
            static so_shade::so_tree::funk_node_ptr_t create_step( void_t ) ;
            static so_shade::so_tree::funk_node_ptr_t create_pulse( void_t ) ;
            static so_shade::so_tree::funk_node_ptr_t create_dot( void_t ) ;
            static so_shade::so_tree::funk_node_ptr_t create_lerp( void_t ) ;
        };
    }
}

#endif
