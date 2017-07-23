//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_DEVIVCE_DEVICES_MIDI_AKAI_MIDIMIX_H_
#define _SNAKEOIL_DEVIVCE_DEVICES_MIDI_AKAI_MIDIMIX_H_

#include "../../../typedefs.h"
#include "../../../protos.h"
#include "../../../api.h"

#include "../midi_device.h"

namespace so_device
{
    namespace so_midi
    {
        class SNAKEOIL_DEVICE_API akai
        {
            so_this_typedefs( akai ) ;

        public:

            static so_device::midi_device_ptr_t create_by( 
                so_device::midi_device::device_info_cref_t di ) ;

        public:

            static so_device::midi_device_ptr_t create_midimix( void_t ) ;
        };
    }
}

#endif
