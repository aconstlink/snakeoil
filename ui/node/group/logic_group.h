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
        class SNAKEOIL_UI_API logic_group : public group
        {
            typedef group base_t ;

            so_this_typedefs( logic_group ) ;

        public:

            logic_group( void_t ) ;
            logic_group( iparent_ptr_t ) ;
            logic_group( this_rref_t ) ;
            virtual ~logic_group( void_t ) ;

        public:

            static this_ptr_t create( so_memory::purpose_cref_t ) ;
            static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
            static void_t destroy( this_ptr_t ) ;

        public:

            so_ui::result add_child( node_ptr_t nptr ) ;
            size_t remove_child( node_ptr_t nptr ) ;

        public:

            virtual so_ui::result apply( so_ui::so_visitor::ivisitor_ptr_t ptr ) ;

        public:

            virtual void_t destroy( void_t ) ;
        };
        so_typedef( logic_group ) ;
    }
}