//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "../render/renderable.h"

#include <snakeoil/math/vector/vector4.hpp>

namespace so_ui
{
    namespace so_node
    {
        class SNAKEOIL_UI_API fill_area_2d : public renderable
        {
            typedef renderable base_t ;
            so_this_typedefs( fill_area_2d ) ;

        private:

            so_math::vec4f_t _color ;

        public:

            fill_area_2d( void_t ) ;
            fill_area_2d( so_math::vec4f_cref_t ) ;
            fill_area_2d( iparent_ptr_t ) ;
            fill_area_2d( this_rref_t ) ;
            virtual ~fill_area_2d( void_t ) ;

        public:

            static this_ptr_t create( so_memory::purpose_cref_t ) ;
            static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
            static void_t destroy( this_ptr_t ) ;

        public:

            so_math::vec4f_cref_t get_fill_area_2d( void_t ) const ;
            void_t set_fill_area_2d( so_math::vec4f_cref_t ) ;

            virtual void_t on_draw( so_ui::so_visitor::rect_render_2d_ptr_t ) ;

        public:
            
            virtual void_t destroy( void_t ) ;

        };
        so_typedef( fill_area_2d ) ;
    }
}
