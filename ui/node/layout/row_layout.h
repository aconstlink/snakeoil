//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "../group.h"

#include <snakeoil/std/container/vector.hpp>

namespace so_ui
{
    namespace so_node
    {
        class SNAKEOIL_UI_API row_layout : public group
        {
            typedef group base_t ;

            so_this_typedefs( row_layout ) ;

        private:

            struct user_data : public group::iuser_data
            {
                float_t weight = 0.0f ;
                so_ui::so_node::node_ptr_t nptr = nullptr ;

                virtual void_t destroy( void_t ) ;
            };
            so_typedef( user_data ) ;
            
        public:

            row_layout( void_t ) ;
            row_layout( iparent_ptr_t ) ;
            row_layout( this_rref_t ) ;
            virtual ~row_layout( void_t ) ;

        public:

            static this_ptr_t create( so_memory::purpose_cref_t ) ;
            static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
            static void_t destroy( this_ptr_t ) ;

        public:

            so_ui::result add_child( node_ptr_t nptr, float_t const weight = 1.0f ) ;
            size_t remove_child( node_ptr_t nptr ) ;

            float_t get_max_weight( void_t ) const ;
            float_t get_weight( size_t const ) const ;

        public:

            virtual so_ui::result apply( so_ui::so_visitor::ivisitor_ptr_t ptr ) ;

        public:

            virtual void_t destroy( void_t ) ;
        };
        so_typedef( row_layout ) ;
    }
}