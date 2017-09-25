//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "null_window.h"

#include <snakeoil/core/macros/move.h>

#include <thread>
#include <chrono>

using namespace so_app ;
using namespace so_app::so_null ;

//***********************************************************************
null_window::null_window( void_t )
{
    _handle = so_app::memory::alloc( null_window_handle_t(), 
        "[null_window::null_window] : null_window_handle" ) ;

    _api = so_gpu::so_null::null_api::create( 
        "[null_window::null_window] : null_api" ) ;
}

//***********************************************************************
null_window::null_window( this_rref_t rhv )
{
    so_move_member_ptr( _handle, rhv ) ;
    so_move_member_ptr( _api, rhv ) ;
}

//***********************************************************************
null_window::~null_window( void_t )
{
    if( so_core::is_not_nullptr( _handle ) )
    {
        so_app::memory::dealloc( _handle ) ;
    }

    if( so_core::is_not_nullptr( _api ) )
    {
        so_gpu::so_null::null_api::destroy( _api ) ;
    }
}

//***********************************************************************
null_window::this_ptr_t null_window::create( so_memory::purpose_cref_t p )
{
    return so_app::memory::alloc( this_t(), p ) ;
}

//***********************************************************************
void_t null_window::destroy( this_ptr_t ptr )
{
    so_app::memory::dealloc( ptr ) ;
}

//***********************************************************************
so_app::result null_window::subscribe( iwindow_message_listener_ptr_t )
{
    return so_app::not_implemented ;
}

//***********************************************************************
so_app::result null_window::unsubscribe( iwindow_message_listener_ptr_t )
{
    return so_app::not_implemented ;
}

//***********************************************************************
so_app::result null_window::destroy( void_t )
{
    this_t::destroy( this ) ;
    return so_app::ok ;
}

//***********************************************************************
iwindow_handle_ptr_t null_window::get_handle( void_t )
{
    return _handle ;
}

//***********************************************************************
so_std::string_cref_t null_window::get_name( void_t ) const
{
    return _name ;
}

//***********************************************************************
void_t null_window::send_close( void_t )
{
}

//***********************************************************************
void_t null_window::render_thread_begin( void_t )
{

}

//***********************************************************************
void_t null_window::render_thread_end( void_t )
{

}

//***********************************************************************
void_t null_window::end_frame( void_t )
{
    //so_log::global::status( "[null_window::end_frame] : end_frame" ) ;
    std::this_thread::sleep_for( std::chrono::milliseconds(100) ) ;
}

//***********************************************************************
so_gpu::iapi_ptr_t null_window::get_api( void_t )
{
    return _api ;
}

//***********************************************************************
so_gpu::api_type null_window::get_api_type( void_t )
{
    return so_gpu::api_type::null ;
}