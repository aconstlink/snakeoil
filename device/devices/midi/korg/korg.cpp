//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "korg.h"

using namespace so_device ;
using namespace so_device::so_midi ;

//****************************************************************************************
so_device::midi_device_ptr_t korg::create_by(
    so_device::midi_device::device_info_cref_t di ) 
{
    if( di.device_name == "nanoKONTROL2" )
    {
        return this_t::create_nanokontrol2() ;
    }
    else if( di.device_name == "nanoPAD2" )
    {
        return this_t::create_nanopad2() ;
    }
    return false ;
}
