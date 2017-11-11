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
        /// idea:
        /// activate/deactivate traversal
        class switch_group : public group
        {
            typedef group base_t ;
            so_this_typedefs( switch_group ) ;

            typedef std::vector< bool_t > bools_t ;

        private:

            // one-to-one relation to children
            bools_t _actives ;

        public:

            switch_group( void_t ) ;
            switch_group( iparent_ptr_t ) ;
            switch_group( this_rref_t ) ;
            virtual ~switch_group( void_t ) ;

        public:

            static this_ptr_t create( so_memory::purpose_cref_t ) ;
            static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
            static void_t destroy( this_ptr_t ) ;

        public:

            /// if set active, the ith child is added to the base group.
            /// if set deactive, the ith child is removed from the base group.
            so_ui::result switch_child( size_t child_i, bool_t active ) ;

            /// switches off all children, so no children are traversed anymore.
            so_ui::result switch_off_all( void_t ) ;

            /// switches on a particular node.
            so_ui::result switch_on( node_ptr_t nptr ) ;


            so_ui::result add_child( node_ptr_t nptr, bool_t active ) ;
            so_ui::result remove_child( node_ptr_t nptr ) ;


        public:

            virtual so_ui::result apply( so_ui::so_visitor::ivisitor_ptr_t ptr ) ;
            virtual so_ui::result detach( node_ptr_t which_ptr ) ;

        public:

            virtual void_t destroy( void_t ) ;
        };
    }
}

