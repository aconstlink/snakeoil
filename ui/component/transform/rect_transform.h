//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "../icomponent.h"

#include <snakeoil/math/vector/vector4.hpp>

namespace so_ui
{
    namespace so_component
    {
        class SNAKEOIL_UI_API rect_transform : public component_base<rect_transform>
        {
            so_this_typedefs( rect_transform ) ;

        private:

            so_math::vec4f_t _area ;

        public:

            rect_transform( void_t ) ;
            rect_transform( so_math::vec4f_cref_t ) ;
            rect_transform( this_cref_t ) = delete ;
            rect_transform( this_rref_t ) ;
            virtual ~rect_transform( void_t ) ;

        public:

            static this_ptr_t create( so_memory::purpose_cref_t ) ;
            static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
            static void_t destroy( this_ptr_t ) ;

        public:

            void_t set_rect( so_math::vec4f_cref_t ) ;
            so_math::vec4f_t get_rect( void_t ) const ;

            void_t set_dims( so_math::vec4f_cref_t ) ;
            so_math::vec4f_t get_dims( void_t ) const ;
        public:

            virtual void_t destroy( void_t ) ;
        };
        so_typedef( rect_transform ) ;
    }
}