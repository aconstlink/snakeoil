//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "../decorator.h"

#include <snakeoil/gpu/enums/attribute_state.h>
#include <snakeoil/gpu/state_set.h>

namespace so_ui
{
    namespace so_node
    {

        class SNAKEOIL_UI_API render_state : public decorator
        {
            so_typedefs( decorator, base ) ;
            so_this_typedefs( render_state ) ;

        private:

            /// which states to be changed
            so_gpu::attribute_state_field_t _af ;
            /// the states to be changed
            so_gpu::state_sets_t _ss ;
            
        public:

            render_state( void_t ) ;
            render_state( this_rref_t ) ;
            render_state( this_cref_t ) ;
            render_state( iparent_ptr_t ) ;
            render_state( so_gpu::attribute_state_field_cref_t, so_gpu::state_sets_cref_t ) ;
            render_state( so_gpu::attribute_state_field_cref_t, so_gpu::state_sets_cref_t, iparent_ptr_t ) ;

            virtual ~render_state( void_t ) ;

        public:

            static this_ptr_t create( so_memory::purpose_cref_t ) ;
            static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
            static void_t destroy( this_ptr_t ) ;

        public:

            virtual so_ui::result apply( so_ui::so_visitor::ivisitor_ptr_t ptr ) ;
            virtual so_ui::result replace( node_ptr_t which, node_ptr_t with ) ;
            virtual so_ui::result detach( node_ptr_t which_ptr ) ;
        
        public:


            so_gpu::attribute_state_field_cref_t get_attributes( void_t ) const ;
            so_gpu::state_sets_cref_t get_states( void_t ) const ;

            void_t set_decorated( so_ui::so_node::node_ptr_t ) ;

        public:

            this_ref_t operator = ( this_cref_t ) = delete ;
            this_ref_t operator = ( this_rref_t ) ;

        public:
    
            virtual void_t destroy( void_t ) ;
        };
        so_typedef( render_state ) ;
    }
}