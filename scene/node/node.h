//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_SCENE_NODE_NODE_H_
#define _SNAKEOIL_SCENE_NODE_NODE_H_

#include "iparent.h"
#include "policy/variable_node_policy.h"

#include "../component/icomponent.h"

#include <snakeoil/std/container/vector.hpp>
#include <functional>

namespace so_scene
{
    namespace so_node
    {
        class SNAKEOIL_SCENE_API node : private variable_node_policy
        {
            so_this_typedefs( node ) ;

        private:

            /// every node has a parent node
            /// if no parent exists, it is the root
            iparent_ptr_t _parent = nullptr ;

        private:

            // not copyable
            node( this_cref_t ) = delete ;

        private: // components

            so_typedefs( so_std::vector<so_scene::so_node::icomponent_ptr_t>, components ) ;
            components_t _comps ;

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

        public: // variable_node_policy

            so_flow::variable_node_ptr_t get_flow_node( void_t ) ;
            bool_t reconnect_flow_node( void_t ) ;

            bool_t bind_variable( so_std::string_in_t, so_flow::ivariable_ptr_t ) ;

        public:

            virtual so_scene::result apply( so_scene::so_visitor::ivisitor_ptr_t ) ;
            virtual void_t destroy( void_t ) = 0 ;

            this_ref_t operator = ( this_cref_t ) = delete ;
            this_ref_t operator = ( this_rref_t ) ;

        public:

            template< typename T >
            static bool_t is_of( so_scene::so_node::node_ptr_t ptr ) 
            {
                return dynamic_cast<T*>( ptr ) != nullptr ;
            }

            template< typename T >
            static T * safe_cast( so_scene::so_node::node_ptr_t ptr )
            {
                return dynamic_cast< T* >( ptr ) ;
            }

            template< typename T >
            static T * just_cast( so_scene::so_node::node_ptr_t ptr )
            {
                return static_cast< T* >( ptr ) ;
            }

        public:

            so_scene::result add_component( so_scene::so_node::icomponent_ptr_t ) ;

            typedef std::function< void_t ( so_scene::so_node::icomponent_ptr_t ) >
                component_funk_t ;

            void_t for_each( component_funk_t ) ;
        };
    }
}

#endif
