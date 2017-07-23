//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "akai.h"

using namespace so_device ;
using namespace so_device::so_midi ;

//****************************************************************************************
so_device::midi_device_ptr_t akai::create_by(
    so_device::midi_device::device_info_cref_t di ) 
{
    if( di.device_name == "MIDI Mix" )
    {
        return this_t::create_midimix() ;
    }
    return false ;
}
