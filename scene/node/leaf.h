//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_SCENE_NODE_LEAF_NODE_H_
#define _SNAKEOIL_SCENE_NODE_LEAF_NODE_H_

#include "node.h"

namespace so_scene
{
    namespace so_node
    {
        class SNAKEOIL_SCENE_API leaf : public node
        {
            typedef node base_t ;
            so_this_typedefs( leaf ) ;

        public:

            leaf( void_t ) ;
            leaf( iparent_ptr_t ) ;
            leaf( this_rref_t ) ;
            virtual ~leaf( void_t ) ;

        public:

            virtual so_scene::result apply( so_scene::so_visitor::ivisitor_ptr_t ptr ) ;
        };
        so_typedef( leaf ) ;
    }
}

#endif
