//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_SCENE_NODE_DECORATOR_LOGIC_DECORATOR_H_
#define _SNAKEOIL_SCENE_NODE_DECORATOR_LOGIC_DECORATOR_H_

#include "../decorator.h"

namespace so_scene
{
    namespace so_node
    {
        class SNAKEOIL_SCENE_API logic_decorator : public decorator
        {
            typedef decorator base_t ;
            so_this_typedefs( logic_decorator ) ;

        public:

            logic_decorator( void_t ) ;
            logic_decorator( this_rref_t ) ;
            logic_decorator( so_scene::so_node::iparent_ptr_t ) ;
            virtual ~logic_decorator( void_t ) ;

        public:

            static this_ptr_t create( so_memory::purpose_cref_t ) ;
            static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
            static void_t destroy( this_ptr_t ) ;
    
        public:

            void_t set_decorated( so_scene::so_node::node_ptr_t ) ;

        public:

            virtual so_scene::result apply( so_scene::so_visitor::ivisitor_ptr_t ) ;
            virtual void_t destroy( void_t ) ;
        };
        so_typedef( logic_decorator ) ;
    }
}

#endif
