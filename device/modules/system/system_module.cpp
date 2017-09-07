//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "system_module.h"

#include "../../devices/midi/midi_device.h"

using namespace so_device ;

//*****************************************************************************
system_module::system_module( void_t )
{

}

//*****************************************************************************
system_module::system_module( this_rref_t )
{

}

//*****************************************************************************
system_module::~system_module( void_t )
{

}

//*****************************************************************************
system_module::this_ptr_t system_module::create( so_memory::purpose_cref_t p )
{
    return so_device::memory::alloc( this_t(), p ) ;
}

//*****************************************************************************
void_t system_module::destroy( this_ptr_t ptr )
{
    so_device::memory::dealloc( ptr ) ;
}

//*****************************************************************************
so_device::midi_device_ptr_t system_module::create_midi_device( so_std::string_cref_t name )
{
    if( name == "nanoPAD2" )
    {
        return this_t::create_korg_nanopad2() ;
    }
    else if( name == "nanoKONTROL2" )
    {
        return this_t::create_korg_nanokontrol2() ;
    }
    else if( name == "MIDI Mix" )
    {
        return this_t::create_akai_midimix() ;
    }
    else if( name == "APC Key 25" )
    {
        return this_t::create_akai_apckey25() ;
    }
    else if( name == "CMD MM-1" )
    {
        return this_t::create_behringer_cmdmm1() ;
    }
    
    return this_t::create_empty_midi_device( name ) ;
}

//*****************************************************************************
void_t system_module::destroy( void_t )
{
    this_t::destroy( this ) ;
}

//*****************************************************************************
so_device::midi_device_ptr_t system_module::create_empty_midi_device( so_std::string_cref_t name )
{
    so_device::midi_device::device_info di ;
    di.device_name = name ;

    return so_device::midi_device_t::create( so_device::midi_device( di ),
        "[system_module::create_empty_midi_device] : empty midi device" ) ;
}