//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "../visitor.h"

#include "../../structs/inode_listener.h"

#include <snakeoil/device/protos.h>
#include <snakeoil/math/vector/vector2.hpp>
#include <snakeoil/math/vector/vector4.hpp>
#include <snakeoil/std/container/stack.hpp>

namespace so_ui
{
    namespace so_visitor
    {
        class SNAKEOIL_UI_API mouse_event_tester : public visitor
        {
            so_this_typedefs( mouse_event_tester ) ;

        private:

            typedef so_std::stack< so_math::vec4f_t, 10 > __area_stack_t ;
            so_typedefs( __area_stack_t, area_stack ) ;
            area_stack_t _area_stack ;

        private:

            so_math::vec2f_t _local_coords ;
            so_math::vec2f_t _local_extend ;

            so_ui::so_component::mouse_event_ptr_t _hit_old = nullptr ;
            so_ui::so_component::mouse_event_ptr_t _hit_new = nullptr ;

            so_device::three_button_mouse_ptr_t _mptr = nullptr ;

            so_math::vec2f_t _dims ;

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

            so_ui::result visit( so_ui::so_node::node_ptr_t ) ;
            so_ui::result post_visit( so_ui::so_node::node_ptr_t ) ;
            

            void_t trigger_hit_node( void_t ) ;

            void_t set_dimension( so_math::vec2f_cref_t ) ;
            void_t set_mouse( so_device::three_button_mouse_ptr_t ) ;

        public:

            virtual void_t destroy( void_t ) ;
        };
        so_typedef( mouse_event_tester ) ;
    }
}