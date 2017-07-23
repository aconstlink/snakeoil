//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "korg.h"

#include "../../../component/buttons/binary_button.h"
#include "../../../component/sliders/slider.h"
#include "../../../component/leds/binary_led.h"
#include "../../../component/knobs/rotary_knob.h"

using namespace so_device ;
using namespace so_device::so_midi ;

//****************************************************************************************
so_device::midi_device_ptr_t korg::create_nanopad2( void_t )
{

    return nullptr ;
}
