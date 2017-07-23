//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_SCENE_NODE_GROUP_H_
#define _SNAKEOIL_SCENE_NODE_GROUP_H_

#include "node.h"

#include <snakeoil/thread/mutex.h>

#include <vector>

namespace so_scene
{
    namespace so_node
    {
        class SNAKEOIL_SCENE_API group : public node, public iparent
        {
            typedef node base_t ;
            so_this_typedefs( group ) ;

            typedef std::vector< node_ptr_t > children_t ;
            typedef std::function< bool_t( size_t i ) > traverse_predicate_t ;

        private:

            so_thread::mutex_t _mtx ;
            children_t _children ;

        protected:

            /// allows a derived class to add a child node.
            so_scene::result add_child( node_ptr_t ) ;

            /// allows a derived class to remove a child node.
            /// the derived class might want to know the index at
            /// which the child was removed, so the index at
            /// which the child was removed will be returned.
            size_t remove_child( node_ptr_t ) ;

            /// allows to traverse all child nodes. the predicate
            /// function allows to fine grain traversal for certain 
            /// derived class behavior.
            void_t traverse_children( so_scene::so_visitor::ivisitor_ptr_t ptr,
                traverse_predicate_t func ) ;

        private: 

            /// non-copyable
            group( this_cref_t ) {}

        protected:

            group( void_t ) ;
            group( iparent_ptr_t ) ;
            group( this_rref_t ) ;

        public:

            virtual ~group( void_t ) ;

        public:

            virtual so_scene::result apply( so_scene::so_visitor::ivisitor_ptr_t ) ;
            so_scene::result apply( so_scene::so_visitor::ivisitor_ptr_t ptr, 
                traverse_predicate_t pred ) ;

            /// replaces the node which_ptr with with_ptr.
            /// if which_ptr does not exist, nothing will happen.
            virtual so_scene::result replace( node_ptr_t which_ptr, node_ptr_t with_ptr ) ;
            virtual so_scene::result detach( node_ptr_t which_ptr ) ;

        public:

            size_t get_num_children( void_t ) const ;
            node_ptr_t get_child( size_t child_i ) ;

            /// find the index of a given child ptr.
            /// -1 is returned if the passed pointer is not
            /// found in the children container.
            size_t find_index( node_ptr_t ) const ;

        public:

            virtual void_t destroy( void_t ) = 0 ;

        };
        so_typedef( group ) ;
    }    
}

#endif
