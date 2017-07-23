//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_DEVICE_DEVICES_MIDI_MIDI_MESSAGE_H_
#define _SNAKEOIL_DEVICE_DEVICES_MIDI_MIDI_MESSAGE_H_

#include "../../typedefs.h"

namespace so_device
{
    struct midi_message
    {
        so_this_typedefs( midi_message ) ;

        byte_t status ;
        byte_t byte1 ;
        byte_t byte2 ;
        byte_t byte3 ;

    public:

        midi_message( void_t ) {}

        midi_message( byte_t status_, byte_t b1_, byte_t b2_, byte_t b3_ )
        {
            status = status_ ;
            byte1 = b1_ ;
            byte2 = b2_ ;
            byte3 = b3_ ;
        }

        midi_message( uint32_t msg )
        {
            status = ( msg >> 0 ) & 255 ;
            byte1 = ( msg >> 8 ) & 255 ;
            byte2 = ( msg >> 16 ) & 255 ;
            byte3 = ( msg >> 24 ) & 255 ;
        }

    public:

        bool_t operator == ( this_cref_t rhv ) const 
        {
            return this_t::to_32bit(*this) == this_t::to_32bit(rhv) ;
        }

        bool_t compare_s_b1( byte_t status_, byte_t byte1_ ) const
        {
            return status == status_ && byte1 == byte1_ ;
        }

        bool_t compare_s_b1( this_cref_t rhv ) const
        {
            return this_t::compare_s_b1( rhv.status, rhv.byte1 ) ;
        }

    public:

        static uint32_t to_32bit( this_cref_t m )
        {
            return m.byte3 << 24 | m.byte2 << 16 | m.byte1 << 8 | m.status ;
        }

        static this_t from_32bit( uint32_t msg )
        {
            return this_t( msg ) ;
        }

        static this_t from_byte( byte_t status, byte_t b1, byte_t b2, byte_t b3 )
        {
            this_t msg ;

            msg.status = status ;
            msg.byte1 = b1 ;
            msg.byte2 = b2 ;
            msg.byte3 = b3 ;

            return msg ;
        }
    };
    so_typedef( midi_message ) ;

    struct midi_sysex_message
    {
        so_this_typedefs( midi_sysex_message ) ;

        /// TBD
    };
    so_typedef( midi_sysex_message ) ;
}

#endif
