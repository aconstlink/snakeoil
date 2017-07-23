//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_SCENE_NODE_LOGIC_LEAF_H_
#define _SNAKEOIL_SCENE_NODE_LOGIC_LEAF_H_

#include "../leaf.h"

namespace so_scene
{
    namespace so_node
    {
        class SNAKEOIL_SCENE_API logic_leaf : public leaf
        {
            typedef leaf base_t ;
            so_this_typedefs( logic_leaf ) ;

        public:

            logic_leaf( void_t ) ;
            logic_leaf( iparent_ptr_t ) ;
            logic_leaf( this_rref_t ) ;
            virtual ~logic_leaf( void_t ) ;

        public:

            static this_ptr_t create( so_memory::purpose_cref_t ) ;
            static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
            static void_t destroy( this_ptr_t ) ;

        public:

            virtual so_scene::result apply( so_scene::so_visitor::ivisitor_ptr_t ) ;
            virtual void_t destroy( void_t ) ;

        };
        so_typedef( logic_leaf ) ;
    }
}

#endif
