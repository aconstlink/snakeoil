//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "../visitor.h"

#include "../../structs/inode_listener.h"

#include <snakeoil/device/protos.h>
#include <snakeoil/math/vector/vector2.hpp>

namespace so_ui
{
    namespace so_visitor
    {
        class SNAKEOIL_UI_API mouse_event_tester : public visitor
        {
            so_this_typedefs( mouse_event_tester ) ;

        private:

            class node_listener : public so_ui::inode_listener
            {
            public:
                so_this_typedefs( node_listener ) ;
                mouse_event_tester * _owner ;
                node_listener( mouse_event_tester * o ) : _owner(o ){}
                node_listener( this_cref_t ) = delete ;
                node_listener( this_rref_t rhv ) { so_move_member_ptr( _owner, rhv ) ; }
                virtual ~node_listener( void_t ) {}
                void_t set_owner( mouse_event_tester * o ) { _owner = o ; }
                virtual void_t on_move( so_ui::so_node::node_ptr_t nptr ) ;
            };
            so_typedef( node_listener ) ;
            friend class node_listener ;

            node_listener_ptr_t _listener = nullptr ;

        private:

            so_math::vec2f_t _local_coords ;
            so_ui::so_node::input_event_ptr_t _hit_old = nullptr ;
            so_ui::so_node::input_event_ptr_t _hit_new = nullptr ;

            so_device::three_button_mouse_ptr_t _mptr = nullptr ;

        public:

            mouse_event_tester( void_t ) ;
            mouse_event_tester( this_cref_t ) = delete ;
            mouse_event_tester( this_rref_t ) ;
            virtual ~mouse_event_tester( void_t ) ;

        public:

            static this_ptr_t create( so_memory::purpose_cref_t ) ;
            static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
            static void_t destroy( this_ptr_t ) ;

        public:

            virtual so_ui::result visit( so_ui::so_node::area_2d_ptr_t ) ;
            virtual so_ui::result post_visit( so_ui::so_node::area_2d_ptr_t ) ;
            virtual so_ui::result visit( so_ui::so_node::input_event_ptr_t ) ;
            virtual so_ui::result post_visit( so_ui::so_node::input_event_ptr_t ) ;

            void_t trigger_hit_node( void_t ) ;

            void_t set_mouse( so_device::three_button_mouse_ptr_t ) ;

        public:

            virtual void_t destroy( void_t ) ;
        };
        so_typedef( mouse_event_tester ) ;
    }
}