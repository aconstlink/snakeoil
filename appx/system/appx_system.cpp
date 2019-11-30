//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "appx_system.h"
#include "../app/iapp.h"

#include <snakeoil/gpx/system/render_system.h>
#include <snakeoil/imex/system/system.h>
#include <snakeoil/device/system/device_system.h>
#include <snakeoil/io/system/system.h>
#include <snakeoil/log/global.h>

#include <snakeoil/core/macros/move.h>

using namespace so_appx ;

//*************************************************************************************
appx_system::appx_system( void_t ) 
{
    _ctx = so_appx::memory::alloc( so_appx::appx_context(),
        "[appx_system::appx_system] : appx_context" ) ;
   
    _imexs_ptr = so_imex::system_t::create_with_default_modules(
        "[appx_system::appx_system] : imex system" ) ;
    _ctx->set_imexs( _imexs_ptr ) ;
}

//*************************************************************************************
appx_system::appx_system( this_rref_t rhv ) 
{
    _apps = std::move( rhv._apps ) ;

    so_move_member_ptr( _ctx, rhv ) ;
    so_move_member_ptr( _imexs_ptr, rhv ) ;
    so_move_member_ptr( _rsys_ptr, rhv ) ;

    _window_datas = std::move( rhv._window_datas ) ;
}

//*************************************************************************************
appx_system::~appx_system( void_t ) 
{
    for( auto & a : _apps )
    {
        a.app_ptr->destroy() ;
    }
    
    so_appx::memory::dealloc( _ctx ) ;

    if( so_core::is_not_nullptr(_imexs_ptr ) )
        _imexs_ptr->destroy() ;
}

//*************************************************************************************
appx_system::this_ptr_t appx_system::create( void_t ) 
{
    return so_appx::memory::alloc( this_t(), "[appx_system::create]" ) ;
}

//*************************************************************************************
void_t appx_system::destroy( this_ptr_t ptr ) 
{
    so_appx::memory::dealloc( ptr ) ;
}

//*************************************************************************************
so_appx::result appx_system::register_app( iapp_ptr_t ptr ) 
{
    auto iter = std::find_if( _apps.begin(), _apps.end(), [&]( __app_data const & rhv ) {
        return rhv.app_ptr == ptr ;
    } ) ;
    
    if( iter != _apps.end() ) 
        return so_appx::ok ;

    _apps.push_back( __app_data 
    {
        ptr,
        so_appx::appx_state::invalid
    } ) ;

    return so_appx::ok ;
}

//*************************************************************************************
size_t appx_system::register_window( std::string const & name ) 
{
    auto iter = std::find_if( _window_datas.begin(), _window_datas.end(), 
        [&]( window_data const & data )
    {
        return data.name == name ;
    } ) ;

    if( iter != _window_datas.end() )
        return size_t(-1) ;

    _window_datas.push_back( window_data{ _window_id++, name, so_appx::window_state_informer() } ) ;

    return _window_datas.back().id ;
}


//*************************************************************************************
so_appx::result appx_system::set_render_system( so_gpx::render_system_ptr_t ptr )
{
    _rsys_ptr = ptr ;
    _ctx->set_render_system( _rsys_ptr ) ;
    return so_appx::ok ;
}

//*************************************************************************************
so_appx::result appx_system::set_window_event_manager( so_appx::window_event_manager_str_t ptr ) 
{
    _wnd_evt_mgr = ptr ;
    _ctx->set_window_event_manager( ptr ) ;
    return so_appx::ok ;
}

//*************************************************************************************
so_appx::result appx_system::update( size_t wid, so_app::iwindow_message_listener_ptr_t iptr ) 
{
    auto iter = std::find_if( _window_datas.begin(), _window_datas.end(), [&]( window_data const & data )
    {
        return data.id == wid ;
    } ) ;

    if( iter == _window_datas.end() ) 
        return so_appx::invalid_argument ;

    iter->state_informer = so_appx::window_state_informer(iptr) ;

    _has_any_window_state_change = true ;

    return so_appx::ok ;
}

//*************************************************************************************
so_appx::result appx_system::update( update_data_cref_t ud )
{
    // check app's state first and act based on it
    for( auto & d : _apps )
    {
        // 1. check state
        // 2. call appropriate callback
        if( d.state == so_appx::appx_state::invalid )
        {
            d.state = so_appx::appx_state::initializing ;

            // if the app is to be initialized,
            // it requires the current state of the windows.
            // the state informer is required to reset its 
            // change flags to true in order to have all states
            // query-able by the app.
            for( auto & wd : _window_datas )
            {
                auto si = wd.state_informer ;
                si.reset(true) ;
                so_appx::window_event_data wed {
                    wd.id,
                    &si
                } ;
                d.app_ptr->on_event( wed ) ;
            }

            so_appx::init_data idata ;
            if( so_appx::no_success( d.app_ptr->on_initialize( _ctx, idata ) ) )
            {
                d.state = so_appx::appx_state::failed ;
            }            
        }
        else if( d.state == so_appx::appx_state::initializing )
        {
            if( d.app_ptr->has_initialized() != so_appx::not_ready )
            {                
                d.state = so_appx::appx_state::running ;
            }
        }
        
        if( d.state == so_appx::appx_state::running )
        {
            if( _has_any_window_state_change )
            {
                for( auto & wd : _window_datas )
                {
                    so_appx::window_event_data wed = {
                        wd.id,
                        &wd.state_informer
                    } ;
                    d.app_ptr->on_event( wed ) ;
                }
            }

            auto const res = d.app_ptr->on_update( ud ) ;
            if( res == so_appx::terminate )
                return so_appx::terminate ;
        }
    }

    if( _has_any_window_state_change )
    {
        for( auto & wd : _window_datas )
        {
            wd.state_informer.reset() ;
        }
        _has_any_window_state_change = false ;
    }   

    return so_appx::ok ;
}

//*************************************************************************************
so_appx::result appx_system::render( render_data_cref_t rd ) 
{
    // check app's state first and act based on it
    for( auto d : _apps )
    {
        if( d.state == so_appx::appx_state::running )
            d.app_ptr->on_render( rd ) ;
    }

    return so_appx::ok ;
}

//*************************************************************************************
so_appx::result appx_system::shutdown( void_t ) 
{
    size_t num_shutdown = 0 ;

    for( auto & d : _apps )
    {
        if( d.state == so_appx::appx_state::initializing )
        {
            if( d.app_ptr->has_initialized() != so_appx::not_ready )
            {
                d.state = so_appx::appx_state::running ;
            }
        }
        else if( d.state == so_appx::appx_state::running )
        {
            d.app_ptr->on_shutdown() ;
            d.state = so_appx::appx_state::shutting_down ;
        }
        else if( d.state == so_appx::appx_state::shutdown ||
            d.app_ptr->has_shutdown() != so_appx::not_ready )
        {
            d.state = so_appx::appx_state::shutdown ;
            ++num_shutdown ;
        }
    }

    if( num_shutdown < _apps.size() ) 
        return so_appx::not_ready ;

    if( num_shutdown == _apps.size() )
    {
        for( auto d : _apps )
        {
            d.app_ptr->destroy() ;
        }
        _apps.clear() ;
    }

    return so_appx::ok ;
}

