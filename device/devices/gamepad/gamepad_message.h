//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_DEVICE_DEVICES_GAMEPAD_GAMEPAD_MESSAGE_H_
#define _SNAKEOIL_DEVICE_DEVICES_GAMEPAD_GAMEPAD_MESSAGE_H_

#include "../../typedefs.h"
#include "../../protos.h"

namespace so_device
{
    struct gamepad_message
    {
        so_this_typedefs( gamepad_message ) ;

    public:

        /// allows for 255 different 
        /// components on a gamepad
        uint8_t id = uint8_t(-1) ;

        /// additional information per component
        uint8_t info = uint8_t(0) ;
        
        /// the actual value
        uint16_t value = uint16_t(-1) ;

    public:

        gamepad_message( void_t )
        {}

        gamepad_message( uint32_t msg )
        {
            id = uint16_t(msg >> 24) & 255 ;
            info = uint16_t(msg >> 16) & 255 ;
            value = uint16_t(msg & 65535) ;
        }

        gamepad_message( uint8_t id_, uint8_t info_, uint16_t value_ )
        {
            id = id_ ;
            info = info_ ;
            value = value_ ;
        }

        static uint32_t to_uint32( this_cref_t rhv ) 
        {
            return uint32_t( rhv.id << 24 | rhv.info << 16 | rhv.value ) ;
        }

    public:

        bool_t compare_id_info( this_cref_t rhv ) const
        {
            return id == rhv.id && info == rhv.info ;
        }

        bool_t operator == ( this_cref_t rhv ) const 
        {
            return id == rhv.id && info == rhv.info && value == rhv.value ;
        }

    };
    so_typedef( gamepad_message ) ;
}

#endif
