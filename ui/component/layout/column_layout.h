//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "../icomponent.h"

namespace so_ui
{
    namespace so_component
    {
        class SNAKEOIL_UI_API column_layout : public so_ui::component_base< column_layout >
        {
            so_this_typedefs( column_layout ) ;

        public:

            column_layout( void_t ) ;
            column_layout( this_cref_t ) = delete ;
            column_layout( this_rref_t ) ;
            virtual ~column_layout( void_t ) ;

        public:

            static this_ptr_t create( so_memory::purpose_cref_t ) ;
            static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
            static void_t destroy( this_ptr_t ) ;

        public:

            virtual void_t destroy( void_t ) ;
        };
        so_typedef( column_layout ) ;
    }
}