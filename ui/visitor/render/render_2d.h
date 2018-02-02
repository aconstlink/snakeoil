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
        class SNAKEOIL_UI_API render_2d : public visitor
        {
            typedef visitor base_t ;
            so_this_typedefs( render_2d ) ;
        
            
        private:

        public:

            render_2d( so_gfx::render_2d_ptr_t ) ;
            render_2d( this_cref_t ) = delete ;
            render_2d( this_rref_t ) ;
            virtual ~render_2d( void_t ) ;

        public:

            static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
            static void_t destroy( this_ptr_t ) ;

        public:

            
            
        public:

            virtual void_t destroy( void_t ) ;

        };
        so_typedef( render_2d ) ;
    }
}
