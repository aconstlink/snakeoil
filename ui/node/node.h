//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "iparent.h"
#include "component/variable_node_component.h"
#include "component/components_component.h"

#include <snakeoil/std/container/vector.hpp>

namespace so_ui
{
    namespace so_node
    {
        class SNAKEOIL_UI_API node : private variable_node_component, private components_component
        {
            so_this_typedefs( node ) ;

        private:

            so_typedefs( so_std::vector< so_ui::inode_listener_ptr_t >, listeners ) ;

        private:

            /// every node has a parent node
            /// if no parent exists, it is the root
            iparent_ptr_t _parent = nullptr ;

            listeners_t _listeners ;

        private:

            // not copyable
            node( this_cref_t ) {}

        public:

            node( void_t ) ;
            node( iparent_ptr_t ) ;
            node( this_rref_t ) ;
            virtual ~node( void_t ) ;

        public:

            /// returns the parent node.
            iparent_ptr_t get_parent( void_t ) ;

            /// allows to set the parent of this node while
            /// returning the old one.
            iparent_ptr_t set_parent( iparent_ptr_t ) ;

            void_t register_listener( so_ui::inode_listener_ptr_t ) ;
            void_t remove_listener( so_ui::inode_listener_ptr_t ) ;

        public: // variable_node_policy

            so_flow::variable_node_ptr_t get_flow_node( void_t ) ;
            bool_t reconnect_flow_node( void_t ) ;

            bool_t bind_variable( so_std::string_in_t, so_flow::ivariable_ptr_t ) ;

        public: // components_component

            bool_t add_component( so_ui::icomponent_ptr_t ) ;

            template< typename comp_t >
            bool_t find_component( comp_t * & ptr_out )
            {
                return components_component::find<comp_t>( ptr_out ) ;
            }

            template< typename comp_t >
            bool_t has_component( void_t ) const
            {
                return components_component::has<comp_t>() ;
            }

        public:

            virtual so_ui::result apply( so_ui::so_visitor::ivisitor_ptr_t ) ;
            virtual void_t destroy( void_t ) = 0 ;

            this_ref_t operator = ( this_cref_t ) = delete ;
            this_ref_t operator = ( this_rref_t ) ;

        public:

            template< typename T >
            static bool_t is_of( so_ui::so_node::node_ptr_t ptr )
            {
                return dynamic_cast< T* >( ptr ) != nullptr ;
            }

            template< typename T >
            static T * safe_cast( so_ui::so_node::node_ptr_t ptr )
            {
                return dynamic_cast< T* >( ptr ) ;
            }

            template< typename T >
            static T * just_cast( so_ui::so_node::node_ptr_t ptr )
            {
                return static_cast< T* >( ptr ) ;
            }
        };
    }
}
