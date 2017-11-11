//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "node.h"

#include <snakeoil/thread/mutex.h>
#include <snakeoil/std/container/vector.hpp>

namespace so_ui
{
    namespace so_node
    {
        class SNAKEOIL_UI_API group : public node, public iparent
        {
            typedef node base_t ;
            so_this_typedefs( group ) ;

        protected:

            struct iuser_data
            {
                virtual void_t destroy( void_t ) = 0 ;
            };
            so_typedef( iuser_data ) ;

        private:

            struct per_child_data
            {
                so_ui::so_node::node_ptr_t node_ptr = nullptr ;
                this_t::iuser_data_ptr_t usr_ptr = nullptr ;
            };
            so_typedef( per_child_data ) ;

            typedef so_std::vector< per_child_data_t > children_t ;
            typedef std::function< bool_t( size_t i ) > traverse_predicate_t ;

        private:

            children_t _children ;

        protected:

            /// allows a derived class to add a child node.
            so_ui::result add_child( node_ptr_t ) ;
            so_ui::result add_child( node_ptr_t, this_t::iuser_data_ptr_t ) ;

            /// allows a derived class to remove a child node.
            /// the derived class might want to know the index at
            /// which the child was removed, so the index at
            /// which the child was removed will be returned.
            size_t remove_child( node_ptr_t ) ;

            /// allows to traverse all child nodes. the predicate
            /// function allows to fine grain traversal for certain 
            /// derived class behavior.
            void_t traverse_children( so_ui::so_visitor::ivisitor_ptr_t ptr,
                traverse_predicate_t func ) ;

            void_t traverse_children( so_ui::so_visitor::ivisitor_ptr_t ptr,
                traverse_predicate_t func, so_ui::result const ) ;

            this_t::iuser_data_ptr_t get_user_data( size_t const child_i ) ;
            this_t::iuser_data_cptr_t get_user_data( size_t const child_i ) const ;
            this_t::per_child_data_t get_child_data( size_t const child_i ) const ;


            /// replaces the node which_ptr with with_ptr.
            /// if which_ptr does not exist, nothing will happen.
            //virtual so_ui::result replace( node_ptr_t which_ptr, node_ptr_t with_ptr ) ;
            //virtual so_ui::result detach( node_ptr_t which_ptr ) ;

        private:

            /// non-copyable
            group( this_cref_t ) = delete ;

        protected:

            group( void_t ) ;
            group( iparent_ptr_t ) ;
            group( this_rref_t ) ;

        public:

            virtual ~group( void_t ) ;

        public:

            virtual so_ui::result apply( so_ui::so_visitor::ivisitor_ptr_t ) ;
            so_ui::result apply( so_ui::so_visitor::ivisitor_ptr_t ptr,
                traverse_predicate_t pred ) ;

            

        public:

            size_t get_num_children( void_t ) const ;
            node_ptr_t get_child( size_t const child_i ) ;

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
