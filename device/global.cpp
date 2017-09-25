//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "global.h"

#include "system/device_system.h"

#include <snakeoil/log/global.h>

using namespace so_device ;

so_thread::mutex_t global::_mtx ;
global::this_ptr_t global::_ptr = nullptr ;

//****************************************************************************************
global::global( void_t )
{
    _dsys_ptr = so_device::memory::alloc(
        so_device::device_system_t(), "[so_device::global::global] : device_system" ) ;
}

//****************************************************************************************
global::global( this_rref_t rhv )
{
    so_move_member_ptr( _dsys_ptr, rhv ) ;
}

//****************************************************************************************
global::~global( void_t )
{
    so_device::device_system_t::destroy( _dsys_ptr ) ;
}

//****************************************************************************************
global::this_ptr_t global::init( void_t )
{
    if( so_core::is_not_nullptr( global_t::_ptr ) )
        return this_t::_ptr ;

    {
        so_thread::lock_guard_t lk( this_t::_mtx ) ;

        if( so_core::is_not_nullptr( global_t::_ptr ) )
            return this_t::_ptr ;

        global::_ptr = so_device::memory::alloc( this_t(),
            "[so_device::global::init]" ) ;

        so_log::global::status( "[online] : snakeoil device" ) ;
    }

    return this_t::_ptr ;
}

//****************************************************************************************
void_t global::deinit( void_t )
{
    if( so_core::is_nullptr( global_t::_ptr ) )
        return ;

    so_device::memory::dealloc( this_t::_ptr ) ;
    this_t::_ptr = nullptr ;
}

//****************************************************************************************
global::this_ptr_t global::get( void_t )
{
    return this_t::init() ;
}

//****************************************************************************************
idevice_system_ptr_t global::device_system( void_t )
{
    return this_t::get()->__get_device_system() ;
}

//****************************************************************************************
idevice_system_ptr_t global::__get_device_system( void_t )
{
    return _dsys_ptr ;
}