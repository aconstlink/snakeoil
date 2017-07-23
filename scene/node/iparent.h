//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_SCENE_NODE_IOPARENT_NODE_H_
#define _SNAKEOIL_SCENE_NODE_IOPARENT_NODE_H_

#include "../typedefs.h"
#include "../api.h"
#include "../protos.h"
#include "../result.h"

namespace so_scene
{
    namespace so_node
    {
        /// allows to specify node that can act as 
        /// parent nodes. Nodes without any child nodes MUST NOT
        /// be parent nodes!
        class SNAKEOIL_SCENE_API iparent
        {
        public:

            virtual so_scene::result replace( node_ptr_t which_ptr, node_ptr_t with_ptr ) = 0 ;
            virtual so_scene::result detach( node_ptr_t which_ptr ) = 0 ;

        };
    }
}

#endif
