//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "../api.h"
#include "../typedefs.h"


#include <snakeoil/log/global.h>

namespace so_device
{
    class SNAKEOIL_DEVICE_API igamepad_notify
    {
    public:

        virtual void_t destroy( void_t ) = 0 ;
        virtual void_t on_button_press( so_std::string_cref_t name, size_t const did, size_t const bid ) = 0 ;
        virtual void_t on_dpad_press( so_std::string_cref_t name, size_t const did, int_t const x, int_t const y ) = 0 ;
    };
    so_typedef( igamepad_notify ) ;

    class SNAKEOIL_DEVICE_API gamepad_print_notify : public igamepad_notify
    {
        so_this_typedefs( gamepad_print_notify ) ;

    public:

        static this_ptr_t create( so_memory::purpose_cref_t p )
        {
            return so_device::memory::alloc( this_t(), p ) ;
        }

        static void_t destroy( this_ptr_t ptr )
        {
            so_device::memory::dealloc( ptr ) ;
        }

    public:

        virtual void_t destroy( void_t )
        {
            this_t::destroy( this ) ;
        }
        
        virtual void_t on_button_press( so_std::string_cref_t dname, size_t const did, size_t const bid )
        {
            so_log::global::status( "[" + dname + "] : " +
                std::to_string( did ) + "; " +
                std::to_string( bid ) ) ;
        }

        virtual void_t on_dpad_press( so_std::string_cref_t dname, size_t const did, int_t const x, int_t const y )
        {
            so_log::global::status( "[" + dname + "] : " +
                std::to_string( did ) + "; " +
                std::to_string( x ) + "; " +
                std::to_string( y ) ) ;
            
        }
    };
    so_typedef( gamepad_print_notify ) ;
}