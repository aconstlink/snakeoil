//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "device_system.h"

#include "../api/imidi_api.h"
#include "../api/igamepad_api.h"
#include "../api/ikeyboard_api.h"
#include "../api/imouse_api.h"

#include "../modules/module_registry.h"

#include "../modules/imidi_module.h"
#include "../modules/igamepad_module.h"
#include "../modules/ikeyboard_module.h"
#include "../modules/imouse_module.h"

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
        _mod_reg->create_devices( mapi ) ;
    }
}

//****************************************************************************************
device_system::device_system( this_rref_t rhv )
{
    _midis = std::move( rhv._midis ) ;
    _gamepads = std::move( rhv._gamepads ) ;
    _keyboards = std::move( rhv._keyboards ) ;
    _mouses = std::move( rhv._mouses ) ;
    _apis = std::move( rhv._apis ) ;

    so_move_member_ptr( _mod_reg, rhv ) ;
}

//****************************************************************************************
device_system::~device_system( void_t )
{
    so_device::module_registry_t::destroy( _mod_reg ) ;

    for( auto * item : _apis )
    {
        item->destroy() ;
    }

    _midis.clear() ;
    _gamepads.clear() ;
    _mouses.clear() ;
    _keyboards.clear() ;
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
so_device::result device_system::register_api( so_device::iapi_ptr_t api_ptr ) 
{
    if( so_core::is_nullptr( api_ptr ) )
    {
        so_log::log::error("[so_device::device_system::register_api] : "
                            "nullptr api not allowed") ;
        return so_device::invalid_argument ;
    }

    // 1. check if api is already registered
    // 2. add to apis if not
    {
        so_thread::lock_guard_t lk( _api_mtx ) ;

        auto const iter = std::find( _apis.begin(), _apis.end(), api_ptr ) ;
        if( iter != _apis.end() )
            return so_device::ok ;

        _apis.push_back( api_ptr ) ;
    }

    // midi
    if( so_core::is_not_nullptr( dynamic_cast<so_device::imidi_api_ptr_t>( api_ptr ) ) )
    {
        auto * ap = dynamic_cast< so_device::imidi_api_ptr_t >( api_ptr ) ;

        so_thread::lock_guard_t lk( _midi_mtx ) ;
        _midis.push_back( ap ) ;
    }

    // gamepad
    if( so_core::is_not_nullptr( dynamic_cast< so_device::igamepad_api_ptr_t >( api_ptr ) ) )
    {
        auto * ap = dynamic_cast< so_device::igamepad_api_ptr_t >( api_ptr ) ;

        so_thread::lock_guard_t lk( _gamepad_mtx ) ;
        _gamepads.push_back( ap ) ;
    }
    
    // keyboard
    if( so_core::is_not_nullptr( dynamic_cast< so_device::ikeyboard_api_ptr_t >( api_ptr ) ) )
    {
        auto * ap = dynamic_cast< so_device::ikeyboard_api_ptr_t >( api_ptr ) ;

        so_thread::lock_guard_t lk( _keyboard_mtx ) ;
        _keyboards.push_back( ap ) ;
    }

    // mouse
    if( so_core::is_not_nullptr( dynamic_cast< so_device::imouse_api_ptr_t >( api_ptr ) ) )
    {
        auto * ap = dynamic_cast< so_device::imouse_api_ptr_t >( api_ptr ) ;

        so_thread::lock_guard_t lk( _mouse_mtx ) ;
        _mouses.push_back( ap ) ;
    }

    return so_device::ok ;
}

//****************************************************************************************
so_device::result device_system::update( void_t ) 
{
    for( auto * item : _midis )
    {
        item->update_midi() ;
    }

    for( auto * item : _gamepads )
    {
        item->update_gamepad() ;
    }

    for( auto * item : _keyboards )
    {
        item->update_keyboard() ;
    }

    for( auto * item : _mouses )
    {
        item->update_mouse() ;
    }

    return so_device::ok ;
}

//****************************************************************************************
void_t device_system::destroy( void_t ) 
{
    this_t::destroy( this ) ;
}

//****************************************************************************************
so_device::result device_system::register_module( so_device::imodule_ptr_t mptr )
{
    if( so_core::is_nullptr( mptr ) )
        return so_device::invalid_argument ;

    {
        auto const res = _mod_reg->register_module( mptr ) ;
        if( res != so_device::ok )
            return res ;
    }

    {
        auto * dyn = dynamic_cast< so_device::imidi_module_ptr_t >( mptr ) ;
        if( so_core::is_not_nullptr( dyn ) )
        {
            for( auto * aptr : _midis )
            {
                aptr->create_devices( dyn ) ;
            }
        }
    }

    {
        auto * dyn = dynamic_cast< so_device::igamepad_module_ptr_t >( mptr ) ;
        if( so_core::is_not_nullptr( dyn ) )
        {
            for( auto * aptr : _gamepads )
            {
                aptr->create_devices( dyn ) ;
            }
        }
    }

    return so_device::ok ;
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
so_device::midi_device_ptr_t device_system::find_midi_device( void_t )
{
    if( _midis.size() > 0 )
        return _midis[ 0 ]->find_any_midi_device() ;

    return nullptr ;
}

//****************************************************************************************
so_device::gamepad_device_ptr_t device_system::find_gamepad_device( void_t )
{
    for( auto * api : _gamepads )
    {
        auto * dev = api->find_any_device() ;
        if( so_core::is_not_nullptr( dev ) )
            return dev ;
    }
    
    return nullptr ;
}

//****************************************************************************************
so_device::ascii_keyboard_ptr_t device_system::find_ascii_keyboard( void_t )
{
    for( auto * api : _keyboards )
    {
        auto * dev = api->find_ascii_keyboard() ;
        if( so_core::is_not_nullptr( dev ) )
            return dev ;
    }

    return nullptr ;
}

//****************************************************************************************
so_device::three_button_mouse_ptr_t device_system::find_three_button_mouse( void_t )
{
    for( auto * api : _mouses )
    {
        auto * dev = api->find_three_button_mouse() ;
        if( so_core::is_not_nullptr( dev ) )
            return dev ;
    }

    return nullptr ;
}

//****************************************************************************************