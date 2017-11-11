//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "../visitor.h"

#include <snakeoil/std/container/stack.hpp>
#include <snakeoil/math/vector/vector4.hpp>

namespace so_ui
{
    namespace so_visitor
    {
        class SNAKEOIL_UI_API layout_updater : public visitor
        {
            so_this_typedefs( layout_updater ) ;

        private:

            typedef so_std::stack< so_math::vec4f_t, 10 > __area_stack_t ;
            so_typedefs( __area_stack_t, area_stack ) ;

            /// stores positive normalized coords
            area_stack_t _astack ;

        public:

            layout_updater( void_t ) ;
            layout_updater( this_cref_t ) ;
            layout_updater( this_rref_t ) ;
            virtual ~layout_updater( void_t ) ;

        public:

            static this_ptr_t create( so_memory::purpose_cref_t ) ;
            static void_t destroy( this_ptr_t ) ;


        public:

            virtual so_ui::result visit( so_ui::so_node::row_layout_ptr_t ) ;
            virtual so_ui::result post_visit( so_ui::so_node::row_layout_ptr_t ) ;
            virtual so_ui::result visit( so_ui::so_node::column_layout_ptr_t ) ;
            virtual so_ui::result post_visit( so_ui::so_node::column_layout_ptr_t ) ;
            virtual so_ui::result visit( so_ui::so_node::area_2d_ptr_t ) ;
            virtual so_ui::result post_visit( so_ui::so_node::area_2d_ptr_t ) ;

            virtual void_t destroy( void_t ) ;
        };
        so_typedef( layout_updater ) ;
    }
}