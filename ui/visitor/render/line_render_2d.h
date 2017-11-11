//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "../visitor.h"

#include <snakeoil/gfx/protos.h>
#include <snakeoil/gpu/state_set.h>
#include <snakeoil/gpu/enums/attribute_state.h>
#include <snakeoil/std/container/stack.hpp>
#include <snakeoil/math/vector/vector4.hpp>

namespace so_ui
{
    namespace so_visitor
    {
        class SNAKEOIL_UI_API line_render_2d : public visitor
        {
            so_this_typedefs( line_render_2d ) ;

        private: // area stack stuff

            typedef so_std::stack< so_math::vec4f_t, 10 > __area_stack_t ;
            so_typedefs( __area_stack_t, area_stack ) ;

            /// stores positive normalized coords
            area_stack_t _astack ;

        private: // layer stack stuff

            typedef so_std::stack< size_t, 10 > __layer_stack_t ;
            so_typedefs( __layer_stack_t, layer_stack ) ;
            layer_stack_t _layer_stack ;

        private: // render state stuff

            struct render_state_data
            {
                so_gpu::attribute_state_field_t attr ;
                so_gpu::state_sets_t ss ;
            } ;
            so_typedef( render_state_data ) ;
            typedef so_std::stack< render_state_data, 10 > __render_state_stack_t ;
            so_typedefs( __render_state_stack_t, render_state_stack ) ;

            render_state_stack_t _render_state_stack ;

        private: // other variables

            so_gfx::line_render_2d_ptr_t _rnd_ptr = nullptr ;

        public:

            line_render_2d( void_t ) ;
            line_render_2d( so_gfx::line_render_2d_ptr_t ) ;
            line_render_2d( this_cref_t ) = delete ;
            line_render_2d( this_rref_t ) ;
            virtual ~line_render_2d( void_t ) ;

        public:

            static this_ptr_t create( so_memory::purpose_cref_t ) ;
            static void_t destroy( this_ptr_t ) ;

            void_t set_render( so_gfx::line_render_2d_ptr_t ) ;
            so_gfx::line_render_2d_ptr_t get_render( void_t ) ;

            so_math::vec4f_cref_t get_top_area( void_t ) const ;
            size_t get_top_layer( void_t ) const ;

        public:

            virtual so_ui::result visit( so_ui::so_node::render_state_ptr_t ) ;
            virtual so_ui::result post_visit( so_ui::so_node::render_state_ptr_t ) ;
            virtual so_ui::result visit( so_ui::so_node::renderable_ptr_t ) ;

            virtual so_ui::result visit( so_ui::so_node::layer_2d_ptr_t ) ;
            virtual so_ui::result post_visit( so_ui::so_node::layer_2d_ptr_t ) ;
            virtual so_ui::result visit( so_ui::so_node::area_2d_ptr_t ) ;
            virtual so_ui::result post_visit( so_ui::so_node::area_2d_ptr_t ) ;

            virtual void_t destroy( void_t ) ;
        };
        so_typedef( line_render_2d ) ;
    }
}