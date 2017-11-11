//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "../leaf.h"

#include <snakeoil/gfx/protos.h>
#include <snakeoil/gpu/state_set.h>
#include <snakeoil/gpu/enums/attribute_state.h>
#include <snakeoil/std/container/vector.hpp>

namespace so_ui
{
    namespace so_node
    {
        class SNAKEOIL_UI_API renderable : public leaf
        {
            typedef leaf base_t ;
            so_this_typedefs( renderable ) ;
            
        public:

            renderable( void_t ) ;
            renderable( this_cref_t ) = delete ;
            renderable( this_rref_t ) ;
            renderable( iparent_ptr_t ) ;

            virtual ~renderable( void_t ) ;

        public:

            static this_ptr_t create( so_memory::purpose_cref_t ) ;
            static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
            static void_t destroy( this_ptr_t ) ;

        public:

            virtual so_ui::result apply( so_ui::so_visitor::ivisitor_ptr_t ) final ;
            virtual void_t destroy( void_t ) ;

        public: // user implementable

            virtual void_t on_draw( so_ui::so_visitor::rect_render_2d_ptr_t ) ;
            virtual void_t on_draw( so_ui::so_visitor::line_render_2d_ptr_t ) ;
            virtual void_t on_draw( so_ui::so_visitor::text_render_2d_ptr_t ) ;
            virtual void_t on_draw( so_ui::so_visitor::image_render_2d_ptr_t ) ;
            
        };
        so_typedef( renderable ) ;
    }
}
