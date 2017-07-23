//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "device_system.h"

#include "../modules/imidi_module.h"
#include "../modules/igamepad_module.h"

#include <snakeoil/log/log.h>

using namespace so_device ;

//****************************************************************************************
device_system::device_system( void_t ) 
{
    this_t::create_default_system_modules() ;
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
    for( auto & item : _midis )
    {
        item.second->destroy() ;
    }

    for( auto & item : _gamepads )
    {
        item.second->destroy() ;
    }
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
so_device::result device_system::register_module( so_std::string_cref_t name, 
    so_device::imidi_module_ptr_t mptr )
{
    if( so_core::is_nullptr(mptr) )
    {
        so_log::log::error("[so_device::device_system::register_module] : \
                            nullptr module not allowed") ;
        return so_device::invalid_argument ;
    }


    so_thread::lock_guard_t lk(_midi_mtx) ;
    
    auto iter = _midis.find( name ) ;
    if( iter != _midis.end() )
    {
        so_log::log::warning( "[so_device::device_system::register_module] : \
                               MIDI module already registered with name : " + name ) ;
        return so_device::ok ;
    }

    _midis[name] = mptr ;

    return so_device::ok ;
}

//****************************************************************************************
so_device::result device_system::register_module( so_std::string_cref_t name, 
    so_device::igamepad_module_ptr_t mptr) 
{
    if( so_core::is_nullptr( mptr ) )
    {
        so_log::log::error( "[so_device::device_system::register_module] : \
                                nullptr gamepad module not allowed" ) ;
        return so_device::invalid_argument ;
    }

    so_thread::lock_guard_t lk( _gamepad_mtx ) ;

    auto iter = _gamepads.find( name ) ;
    if( iter != _gamepads.end() )
    {
        so_log::log::warning( "[so_device::device_system::register_module] : \
                                gamepad module already registered with name : " + name ) ;
        return so_device::ok ;
    }

    _gamepads[name] = mptr ;

    return so_device::ok ;
}

//****************************************************************************************
so_device::result device_system::update( void_t ) 
{
    for( auto & item : _midis )
    {
        item.second->update() ;
    }

    for( auto & item : _gamepads )
    {
        item.second->update() ;
    }

    return so_device::ok ;
}

//****************************************************************************************
so_device::imidi_module_ptr_t device_system::get_midi_module( so_std::string_cref_t module_name ) 
{
    so_thread::lock_guard_t lk( _midi_mtx ) ;
    auto iter = _midis.find( module_name ) ;
    if( iter == _midis.end() )
    {
        so_log::log::error( "[so_device::device_system::get_midi_module] : \
                             no module with name : " + module_name ) ;
        return nullptr ;
    }
    return iter->second ;
}

//****************************************************************************************
so_device::igamepad_module_ptr_t device_system::get_gamepad_module( so_std::string_cref_t module_name ) 
{
    so_thread::lock_guard_t lk( _gamepad_mtx ) ;
    auto iter = _gamepads.find( module_name ) ;
    if( iter == _gamepads.end() )
    {
        so_log::log::error( "[so_device::device_system::get_midi_module] : \
                             no module with name : " + module_name ) ;
        return nullptr ;
    }
    return iter->second ;
}

//****************************************************************************************
void_t device_system::destroy( void_t ) 
{
    this_t::destroy( this ) ;
}
