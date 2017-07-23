//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_DEVICE_COMPONENT_INPUT_COMPONENT_H_
#define _SNAKEOIL_DEVICE_COMPONENT_INPUT_COMPONENT_H_

#include "iinput_component.h"

namespace so_device
{
    namespace so_component
    {
        template< typename T >
        struct input_component : public iinput_component
        {
            so_this_typedefs( input_component<T> ) ;
            so_typedefs( T, component ) ;

            static component_ptr_t create( so_memory::purpose_cref_t p )
            {
                return so_memory::memory::alloc( component_t(), p ) ;
            }

            static component_ptr_t create( component_rref_t rhv, so_memory::purpose_cref_t p )
            {
                return so_memory::memory::alloc( std::move( rhv ), p ) ;
            }

            static void_t destroy( component_ptr_t ptr )
            {
                so_memory::memory::dealloc( ptr ) ;
            }

            virtual void_t destroy( void_t )
            {
                this_t::destroy( static_cast<component_ptr_t>(this) ) ;
            }
        };
    }
}

#endif
