//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "module_registry.h"

#include "imodule.h"
#include "imidi_module.h"
#include "igamepad_module.h"

#include "system/system_module.h"

#include <snakeoil/log/log.h>

using namespace so_device ;

//*****************************************************************************
module_registry::module_registry( void_t )
{
    auto * mod_ptr = so_device::system_module_t::create( 
        "[module_registry::module_registry] : system module" ) ;

    this_t::register_module( mod_ptr ) ;
}

//*****************************************************************************
module_registry::module_registry( this_rref_t rhv )
{
    _midis = std::move( rhv._midis ) ;
    _gamepads = std::move( rhv._gamepads ) ;
    _keyboards = std::move( rhv._keyboards ) ;
    _mouses = std::move( rhv._mouses ) ;
    _mods = std::move( rhv._mods ) ;
}

//*****************************************************************************
module_registry::~module_registry( void_t )
{
    for( auto * m : _mods )
    {
        m->destroy() ;
    }
}

//*****************************************************************************
module_registry::this_ptr_t module_registry::create( so_memory::purpose_cref_t p )
{
    return so_device::memory::alloc( this_t(), p ) ;
}

//*****************************************************************************
void_t module_registry::destroy( this_ptr_t ptr )
{
    so_device::memory::dealloc( ptr ) ;
}

//*****************************************************************************
so_device::result module_registry::register_module( so_device::imodule_ptr_t mptr )
{
    {
        so_thread::lock_guard_t lk( _mtx_mods ) ;
        
        auto const iter = std::find( _mods.begin(), _mods.end(), mptr ) ;
        if( iter != _mods.end() )
        {
            so_log::log::error( "[module_registry::register_module] : module already added" ) ;
            return so_device::failed ;
        }

        _mods.push_back( mptr ) ;
    }

    {
        so_thread::lock_guard_t lk( _mtx_midi ) ;
        auto * dyn = dynamic_cast< so_device::imidi_module_ptr_t >( mptr ) ;
        if( so_core::is_not_nullptr( dyn ) )
        {
            _midis.push_back( dyn ) ;
        }
    }

    {
        so_thread::lock_guard_t lk( _mtx_gamepad ) ;
        auto * dyn = dynamic_cast< so_device::igamepad_module_ptr_t >( mptr ) ;
        if( so_core::is_not_nullptr( dyn ) )
        {
            _gamepads.push_back( dyn ) ;
        }
    }

    return so_device::ok ;
}

//*****************************************************************************
so_device::midi_device_ptr_t module_registry::create_midi_device( so_std::string_cref_t name )
{
    so_device::midi_device_ptr_t ret = nullptr ;

    {
        so_thread::lock_guard_t lk( _mtx_midi ) ;
        for( auto * mptr : _midis )
        {
            ret = mptr->create_midi_device( name ) ;
            if( so_core::is_not_nullptr( ret ) )
            {
                break ;
            }
        }
    }

    return ret ;
}
