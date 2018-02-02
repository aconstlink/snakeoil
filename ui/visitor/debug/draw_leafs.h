//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
# pragma once

#include "../visitor.h"

#include <snakeoil/gfx/protos.h>
#include <snakeoil/math/vector/vector4.hpp>
#include <snakeoil/std/container/stack.hpp>

namespace so_ui
{
    namespace so_visitor
    {
        class SNAKEOIL_UI_API draw_leafs : public so_ui::so_visitor::visitor
        {
            so_this_typedefs( draw_leafs ) ;

        private:

            typedef so_std::stack< so_math::vec4f_t, 10 > __area_stack_t ;
            so_typedefs( __area_stack_t, area_stack ) ;

            typedef so_std::stack< size_t, 10 > __in_row_stack_t ;
            so_typedefs( __in_row_stack_t, in_row_stack ) ;

            area_stack_t _astack ;
            in_row_stack_t _in_row_stack ;

            so_gfx::rect_render_2d_ptr_t _rect_rnd_ptr = nullptr ;

            size_t _count = 0 ;

            size_t _layer = 0 ;

        public:

            draw_leafs( so_math::vec4f_cref_t rect, so_gfx::rect_render_2d_ptr_t rnd ) ;
            draw_leafs( this_cref_t ) = delete ;
            draw_leafs( this_rref_t ) ;
            virtual ~draw_leafs( void_t ) ;

        public:

            static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
            static void_t destroy( this_ptr_t ) ;

        public:

            virtual so_ui::result visit( so_ui::so_node::node_ptr_t nptr ) ;

            virtual so_ui::result post_visit( so_ui::so_node::node_ptr_t nptr ) ;

            virtual so_ui::result visit( so_ui::so_node::leaf_ptr_t nptr ) ;

            virtual so_ui::result post_visit( so_ui::so_node::leaf_ptr_t ) ;

            virtual void_t destroy( void_t ) ;
        };
        so_typedef( draw_leafs ) ;
    }
}