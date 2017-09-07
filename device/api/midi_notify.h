//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_DEVICE_API_MIDI_NOTIFY_H_
#define _SNAKEOIL_DEVICE_API_MIDI_NOTIFY_H_

#include "../api.h"
#include "../typedefs.h"

#include "../devices/midi/midi_message.h"

#include <snakeoil/log/log.h>

namespace so_device
{
    class SNAKEOIL_DEVICE_API imidi_notify
    {
    public:

        virtual void_t destroy( void_t ) = 0 ;
        virtual void_t on_message( so_std::string_cref_t, so_device::midi_message_cref_t ) = 0 ;
    };
    so_typedef( imidi_notify ) ;

    class SNAKEOIL_DEVICE_API midi_print_notify : public imidi_notify
    {
        so_this_typedefs( midi_print_notify ) ;

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
        
        virtual void_t on_message( so_std::string_cref_t dname, so_device::midi_message_cref_t msg )
        {
            so_log::log::status( "[" + dname + "] : " +
                std::to_string( msg.status ) + "; " +
                std::to_string( msg.byte1 ) + "; " +
                std::to_string( msg.byte2 ) + "; " +
                std::to_string( msg.byte3 ) ) ;
        }
    };
    so_typedef( midi_print_notify ) ;
}

#endif