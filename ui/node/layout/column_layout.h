//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "../group.h"

namespace so_ui
{
    namespace so_node
    {
        class SNAKEOIL_UI_API column_layout : public group
        {
            typedef group base_t ;

            so_this_typedefs( column_layout ) ;

        private:

            struct user_data : public group::iuser_data
            {
                float_t weight = 0.0f ;
                so_ui::so_node::node_ptr_t nptr = nullptr ;

                virtual void_t destroy( void_t ) ;
            };
            so_typedef( user_data ) ;

        public:

            column_layout( void_t ) ;
            column_layout( iparent_ptr_t ) ;
            column_layout( this_rref_t ) ;
            virtual ~column_layout( void_t ) ;

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
        so_typedef( column_layout ) ;
    }
}