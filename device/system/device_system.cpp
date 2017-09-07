//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "device_system.h"

#include "../api/imidi_api.h"
#include "../api/igamepad_api.h"

#include "../modules/module_registry.h"

#include <snakeoil/log/log.h>

using namespace so_device ;

//****************************************************************************************
device_system::device_system( void_t ) 
{
    _mod_reg = so_device::module_registry_t::create( 
        "[device_system::device_system] : module registry" ) ;

    this_t::create_default_system_modules() ;

    for( auto * mapi : _midis )
    {
        mapi->create_devices( _mod_reg ) ;
    }
}

//****************************************************************************************
device_system::device_system( this_rref_t rhv )
{
    _midis = std::move( rhv._midis ) ;
    _gamepads = std::move( rhv._gamepads ) ;
}

//****************************************************************************************
device_system::~device_system( void_t )
{
    for( auto * item : _midis )
    {
        item->destroy() ;
    }

    for( auto * item : _gamepads )
    {
        item->destroy() ;
    }

    so_device::module_registry_t::destroy( _mod_reg ) ;
}

//****************************************************************************************
device_system::this_ptr_t device_system::create( so_memory::purpose_cref_t p ) 
{
    return this_t::create( this_t(), p ) ;
}

//****************************************************************************************
device_system::this_ptr_t device_system::create( this_rref_t rhv, so_memory::purpose_cref_t p ) 
{
    return so_device::memory::alloc( std::move(rhv), p ) ;
}

//****************************************************************************************
void_t device_system::destroy( this_ptr_t ptr )
{
    so_device::memory::dealloc( ptr ) ;
}

//****************************************************************************************
so_device::result device_system::register_api( so_device::imidi_api_ptr_t mptr )
{
    if( so_core::is_nullptr(mptr) )
    {
        so_log::log::error("[so_device::device_system::register_module] : "
                            "nullptr module not allowed") ;
        return so_device::invalid_argument ;
    }

    so_thread::lock_guard_t lk(_midi_mtx) ;
    
    auto const iter = std::find( _midis.begin(), _midis.end(), mptr ) ;
    if( iter != _midis.end() )
    {
        so_log::log::warning( "[so_device::device_system::register_module] : "
                                "MIDI module already registered"  ) ;
        return so_device::ok ;
    }

    _midis.push_back( mptr ) ;

    return so_device::ok ;
}

//****************************************************************************************
so_device::result device_system::register_api( so_device::igamepad_api_ptr_t mptr) 
{
    if( so_core::is_nullptr( mptr ) )
    {
        so_log::log::error( "[so_device::device_system::register_module] : "
                                "nullptr gamepad module not allowed" ) ;
        return so_device::invalid_argument ;
    }

    so_thread::lock_guard_t lk( _gamepad_mtx ) ;

    auto const iter = std::find( _gamepads.begin(), _gamepads.end(), mptr ) ;
    if( iter != _gamepads.end() )
    {
        so_log::log::warning( "[so_device::device_system::register_module] : "
                                "gamepad module already registered") ;
        return so_device::ok ;
    }

    _gamepads.push_back( mptr ) ;

    return so_device::ok ;
}

//****************************************************************************************
so_device::result device_system::update( void_t ) 
{
    for( auto * item : _midis )
    {
        item->update() ;
    }

    for( auto * item : _gamepads )
    {
        item->update() ;
    }

    return so_device::ok ;
}

//****************************************************************************************
void_t device_system::destroy( void_t ) 
{
    this_t::destroy( this ) ;
}

//****************************************************************************************
void_t device_system::install_midi_notify( so_device::imidi_notify_ptr_t nptr )
{
    for( auto * api : _midis )
    {
        api->install_midi_notify( nptr ) ;
    }
}

//****************************************************************************************
bool_t device_system::get_midi_device_names( so_std::vector< so_std::string_t > & names )
{
    so_std::vector< so_std::string_t > names_ ;

    for( auto * mapi : _midis )
    {
        mapi->get_device_names( names_ ) ;
    }

    names = std::move( names_ ) ;

    return true ;
}

//****************************************************************************************
so_device::midi_device_ptr_t device_system::find_midi_device( so_std::string_cref_t name )
{
    // for each midi api, check for created midi device

    for( auto * mapi : _midis )
    {
        auto * mdev = mapi->find_midi_device( name ) ;
        if( so_core::is_not_nullptr( mdev ) ) 
            return mdev ;
    }

    return nullptr ;
}

//****************************************************************************************
so_device::midi_device_ptr_t device_system::find_any_midi_device( void_t )
{
    if( _midis.size() > 0 )
        return _midis[ 0 ]->find_any_midi_device() ;

    return nullptr ;
}

//****************************************************************************************
so_device::gamepad_device_ptr_t device_system::find_gamepad_device( void_t )
{
    // return the first gamepad found

    return nullptr ;
}

//****************************************************************************************
so_device::keyboard_device_ptr_t device_system::find_keyboard_device( void_t )
{
    return nullptr ;
}

//****************************************************************************************
so_device::mouse_device_ptr_t device_system::find_mouse_device( void_t )
{
    return nullptr ;
}

//****************************************************************************************