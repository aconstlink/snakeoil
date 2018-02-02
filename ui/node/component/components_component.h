//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "../../typedefs.h"
#include "../../api.h"
#include "../../protos.h"
#include "../../result.h"

#include <snakeoil/std/container/vector.hpp>
#include <functional>

namespace so_ui
{
    namespace so_node
    {
        class SNAKEOIL_UI_API components_component
        {
            so_this_typedefs( components_component ) ;

        public:

            so_typedefs( std::function < bool_t ( so_ui::icomponent_ptr_t ) >, foreach_funk ) ;

        private:

            so_typedefs( so_std::vector< so_ui::icomponent_ptr_t >, components ) ;
            components_t _components ;

        public:

            components_component( void_t ) ;
            components_component( this_cref_t ) = delete ;
            components_component( this_rref_t ) ;
            ~components_component( void_t ) ;

        public:

            this_ref_t operator = ( this_cref_t ) = delete ;
            this_ref_t operator = ( this_rref_t ) ;

        public:

            bool_t add_component( so_ui::icomponent_ptr_t ) ;

            template< typename comp_t >
            bool_t find( comp_t * & ptr_out )
            {
                for( auto * cptr : _components )
                {
                    if( cptr->is_of_type( typeid( comp_t ) ) )
                    {
                        ptr_out = dynamic_cast<comp_t*>( cptr ) ;
                        return true ;
                    }
                }
                return false ;
            }

            template< typename comp_t >
            bool_t has( void_t ) const
            {
                for( auto * cptr : _components )
                {
                    if( cptr->is_of_type( typeid( comp_t ) ) )
                    {
                        return true ;
                    }
                }
                return false ;
            }

        public:

            void_t foreach_component( foreach_funk_t ) ;
        };
        so_typedef( components_component ) ;
    }
}