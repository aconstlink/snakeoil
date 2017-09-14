//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "window_application.h"

#include "../window/iwindow.h"
#include "../window/window_message_receiver.h"

#include <snakeoil/device/global.h>
#include <snakeoil/device/system/idevice_system.h>

#include <snakeoil/io/global.h>
#include <snakeoil/thread/global.h>
#include <snakeoil/thread/scheduler.h>
#include <snakeoil/log/global.h>
#include <snakeoil/core/macros/move.h>

using namespace so_app ;

//***********************************************************************
window_application::window_application( void_t ) 
{
    _app_state_ptr = so_app::memory::alloc( application_state_t(),
        "[window_application::window_application] : application_state" ) ;
}

//***********************************************************************
window_application::window_application( this_rref_t rhv )
{
    _windows = std::move( rhv._windows ) ;
    
    _update_thread = std::move( rhv._update_thread ) ;    

    so_move_member_ptr( _app_state_ptr, rhv ) ;
}

//***********************************************************************
window_application::~window_application( void_t )
{
    shutdown_and_kill_all_windows() ;
    so_app::memory::dealloc( _app_state_ptr ) ;
}

//***********************************************************************
so_app::result window_application::register_window( so_app::iwindow_ptr_t wptr )
{
    if( wptr == nullptr ) 
        return so_app::invalid_argument ;

    auto iter = std::find_if( _windows.begin(), _windows.end(), [&]( window_data const & data )
        { return data.window_ptr == wptr ; } ) ;

    if( iter != _windows.end() ) 
        return so_app::ok ;

    _windows.push_back( window_data { wptr } ) ;

    return so_app::ok ;
}

//***********************************************************************
so_app::result window_application::exec( void_t ) 
{
    return this_t::exec( [=]( application_state_ptr_t as )
    { 
        while( as->is_running() )
        {
            so_device::global::device_system()->update() ;
            std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) ) ;
        }        
        as->update_finised() ;  
    } ) ;
}

//***********************************************************************
so_app::result window_application::exec( std::function< void_t( application_state_ptr_t ) > update_funk )
{
    if( _windows.size() == 0 )
        return so_app::ok ;

    _app_state_ptr->update_begin() ;

    _update_thread = so_thread::thread_t( [=]( void_t )
    {
        update_funk( _app_state_ptr ) ;
    } ) ;

    auto const res = this->exec_derived() ;
    {
        this_t::shutdown_and_kill_all_windows() ;
    }

    return res ;
}

//***********************************************************************
so_app::result window_application::shutdown_and_kill_all_windows( void_t ) 
{
    so_app::result res = shutdown_update_thread() ;
    so_log::global::error( so_app::no_success(res), 
        "[window_application::~window_application] : failed to shutdown update thread." ) ;

    for( auto & data : _windows )
    {
        data.window_ptr->destroy() ;
    }
    _windows.clear() ;

    if( _update_thread.joinable() )
        _update_thread.join()  ;

    return so_app::ok ;
}

//***********************************************************************
so_app::result window_application::shutdown_update_thread( void_t ) 
{
    if( so_core::is_nullptr( _app_state_ptr ) )
        return so_app::ok ;

    if( so_core::is_not( _app_state_ptr->is_running() ) ) 
        return so_app::ok ;

    _app_state_ptr->stop() ;
    _app_state_ptr->wait_for_finish() ;

    return so_app::ok ;
}

