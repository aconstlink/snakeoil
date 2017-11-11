//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "../decorator.h"

#include <snakeoil/math/vector/vector4.hpp>

namespace so_ui
{
    namespace so_node
    {
        class SNAKEOIL_UI_API area_2d : public decorator
        {
            typedef decorator base_t ;
            so_this_typedefs( area_2d ) ;

        public:

            /// xy: offset
            /// zw: width/height
            so_math::vec4f_t _data ;

        public:

            area_2d( void_t ) ;
            area_2d( this_rref_t ) ;
            area_2d( so_ui::so_node::iparent_ptr_t ) ;
            virtual ~area_2d( void_t ) ;

        public:

            static this_ptr_t create( so_memory::purpose_cref_t ) ;
            static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
            static void_t destroy( this_ptr_t ) ;

        public:

            void_t set_decorated( so_ui::so_node::node_ptr_t ) ;
            void_t set_area( so_math::vec4f_cref_t ) ;
            so_math::vec4f_cref_t get_area( void_t ) const ;

        public:

            virtual so_ui::result apply( so_ui::so_visitor::ivisitor_ptr_t ) ;
            virtual void_t destroy( void_t ) ;
        };
        so_typedef( area_2d ) ;
    }
}
