//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_DEVICE_DEVICES_MIDI_MIDI_ENUMS_H_
#define _SNAKEOIL_DEVICE_DEVICES_MIDI_MIDI_ENUMS_H_

namespace so_device
{
    enum class midi_output_result
    {
        transmit_and_queue,
        transmit_and_dismiss,
        queue,
        dismiss
    };
}

#endif
