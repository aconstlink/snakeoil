//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_SCENE_NODE_DECORATOR_NODE_H_
#define _SNAKEOIL_SCENE_NODE_DECORATOR_NODE_H_

#include "node.h"

namespace so_scene
{
    namespace so_node
    {
        class SNAKEOIL_SCENE_API decorator : public node, public iparent
        {
            so_typedefs( node, base ) ;
            so_this_typedefs( decorator ) ;

        private:

            node_ptr_t _decorated = nullptr ;

        protected:

            decorator( void_t ) ;
            decorator( this_rref_t ) ;
            decorator( iparent_ptr_t ) ;

        public:

            virtual ~decorator( void_t ) ;

        public:

            virtual so_scene::result apply( so_scene::so_visitor::ivisitor_ptr_t ptr ) ;
            virtual so_scene::result replace( node_ptr_t which, node_ptr_t with ) ;
            virtual so_scene::result detach( node_ptr_t which_ptr ) ;

        public:

            node_ptr_t get_decorated( void_t ) ;
            node_cptr_t get_decorated( void_t ) const ;

            this_ref_t operator = ( this_cref_t ) = delete ;
            this_ref_t operator = ( this_rref_t ) ;


        protected:

            void_t set_decorated( node_ptr_t nptr, bool_t reset_parent ) ;
            void_t set_decorated( node_ptr_t nptr ) ;

            so_scene::result traverse_decorated( so_scene::so_visitor::ivisitor_ptr_t ptr ) ;            
        };
    }
}

#endif
