//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "../decorator.h"

#include <snakeoil/math/utility/2d/transformation.hpp>

namespace so_ui
{
    namespace so_node
    {
        class SNAKEOIL_UI_API transform_2d : public decorator
        {
            typedef decorator base_t ;
            so_this_typedefs( transform_2d ) ;

            so_typedefs( so_math::trafo2df_t, trafo ) ;

        private:

            trafo_t _trafo ;

        public:

            transform_2d( void_t ) ;
            transform_2d( this_rref_t ) ;
            transform_2d( this_cref_t ) ;
            transform_2d( this_t::trafo_cref_t ) ;
            transform_2d( iparent_ptr_t ) ;
            transform_2d( node_ptr_t, iparent_ptr_t = nullptr ) ;
            virtual ~transform_2d( void_t ) ;

        public:

            static this_ptr_t create( so_memory::purpose_cref_t ) ;
            static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
            static void_t destroy( this_ptr_t ) ;

        public:

            this_ref_t set_decorated( node_ptr_t ) ;

        public:

            void_t set_transformation( trafo_cref_t ) ;
            trafo_cref_t get_transformation( void_t ) const ;
            trafo_t compute_trafo( void_t ) const ;

        public:

            virtual so_ui::result apply( so_ui::so_visitor::ivisitor_ptr_t ptr ) ;

        public:

            virtual void_t destroy( void_t ) ;
        };
        so_typedef( transform_2d ) ;
    }
}

