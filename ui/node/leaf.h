//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "node.h"

namespace so_ui
{
    namespace so_node
    {
        class SNAKEOIL_UI_API leaf : public node
        {
            typedef node base_t ;
            so_this_typedefs( leaf ) ;

        public:

            leaf( void_t ) ;
            leaf( iparent_ptr_t ) ;
            leaf( this_rref_t ) ;
            virtual ~leaf( void_t ) ;

        public:

            virtual so_ui::result apply( so_ui::so_visitor::ivisitor_ptr_t ptr ) ;
        };
        so_typedef( leaf ) ;
    }
}

