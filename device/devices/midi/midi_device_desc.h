//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_DEVICE_DEVICES_MIDI_MIDI_DEVICE_DESC_H_
#define _SNAKEOIL_DEVICE_DEVICES_MIDI_MIDI_DEVICE_DESC_H_

#include "../../typedefs.h"

namespace so_device
{
    struct midi_device_desc
    {
        so_this_typedefs( midi_device_desc ) ;

        so_std::string_t device_name ;
    };
    so_typedef( midi_device_desc ) ;
}

#endif