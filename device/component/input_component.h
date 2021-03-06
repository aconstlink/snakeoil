//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "iinput_component.h"

namespace so_device
{
    namespace so_input
    {
        template< typename T >
        struct input_component : public iinput_component
        {
            so_this_typedefs( input_component<T> ) ;
            so_typedefs( T, component ) ;

            static component_ptr_t create( so_memory::purpose_cref_t p )
            {
                return so_memory::global::alloc( component_t(), p ) ;
            }

            static component_ptr_t create( component_rref_t rhv, so_memory::purpose_cref_t p )
            {
                return so_memory::global::alloc( std::move( rhv ), p ) ;
            }

            static void_t destroy( component_ptr_t ptr )
            {
                so_memory::global::dealloc( ptr ) ;
            }

            virtual void_t destroy( void_t )
            {
                this_t::destroy( static_cast<component_ptr_t>(this) ) ;
            }
        };
    }
}
