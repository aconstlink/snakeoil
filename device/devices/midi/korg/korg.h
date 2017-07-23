//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_DEVICE_DEVICES_MIDI_KORG_KORG_H_
#define _SNAKEOIL_DEVICE_DEVICES_MIDI_KORG_KORG_H_

#include "../../../typedefs.h"
#include "../../../protos.h"
#include "../../../api.h"

#include "../midi_device.h"

namespace so_device
{
    namespace so_midi
    {
        class SNAKEOIL_DEVICE_API korg
        {
            so_this_typedefs( korg ) ;

        public:

            static so_device::midi_device_ptr_t create_by(
                so_device::midi_device::device_info_cref_t di ) ;

        public:

            static so_device::midi_device_ptr_t create_nanokontrol2( void_t ) ;
            static so_device::midi_device_ptr_t create_nanopad2( void_t ) ;
        };
    }
}

#endif
