//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "../icomponent.h"

#include <snakeoil/gfx/protos.h>
#include <snakeoil/std/string/utf8.hpp>

namespace so_ui
{
    namespace so_component
    {
        class SNAKEOIL_UI_API gfx_draw_2d : public component_base<gfx_draw_2d>
        {
            so_this_typedefs( gfx_draw_2d ) ;

            typedef std::function< void_t ( so_ui::so_visitor::gfx_draw_2d_ptr_t ) > render_funk_t ;

        private:

            render_funk_t _funk ;

        public:

            gfx_draw_2d( void_t ) ;
            gfx_draw_2d( render_funk_t ) ;
            gfx_draw_2d( this_cref_t ) = delete ;
            gfx_draw_2d( this_rref_t ) ;
            virtual ~gfx_draw_2d( void_t ) ;

        public:

            static this_ptr_t create( so_memory::purpose_cref_t ) ;
            static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
            static void_t destroy( this_ptr_t ) ;

        public:

            virtual void_t render( so_ui::so_visitor::gfx_draw_2d_ptr_t )  ;

        public:

            virtual void_t destroy( void_t ) ;
        };
        so_typedef( gfx_draw_2d ) ;
    }
}