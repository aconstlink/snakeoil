//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "appx_application.h"

#include "../system/appx_system.h"
#include "../system/update_data.h"

#include "../system/window_state_informer.h"

#include <snakeoil/application/application/application.h>
#include <snakeoil/application/window/gl_window.h>
#include <snakeoil/application/window/window_message_receiver.h>

#include <snakeoil/device/global.h>
#include <snakeoil/device/system/device_system.h>

#include <snakeoil/audiox/system/audio_system.h>
#include <snakeoil/gpx/system/render_system.h>
#include <snakeoil/thread/global.h>
#include <snakeoil/thread/scheduler.h>
#include <snakeoil/log/global.h>
#include <snakeoil/core/macros/move.h>

using namespace so_appx ;

//***********************************************************************
appx_application::this_ptr_t appx_application::create( std::string const & purpose ) 
{
    return this_t::create( this_t(), purpose ) ;
}

//***********************************************************************
appx_application::this_ptr_t appx_application::create( this_rref_t rhv, std::string const & purpose ) 
{
    return so_appx::memory::alloc( std::move(rhv), purpose ) ;
}

//***********************************************************************
void_t appx_application::destroy( this_ptr_t ptr ) 
{
    so_appx::memory::dealloc( ptr ) ;
}

//***********************************************************************
appx_application::appx_application( void_t ) 
{
    // this creation depends on the platform
    _win_app_ptr = so_app::application_t::create() ;
    _appx_ptr = appx_system::create() ;

    _shared_data_ptr = so_appx::memory::alloc( shared_data(), 
        "[appx_application::appx_application] : shared_data" ) ;

    _rs_ptr = so_gpx::render_system_t::create(
        "[appx_application::create_window] : render_system" ) ;

    _as_ptr = so_audiox::audio_system_t::create( 
        "[appx_application::create_window] : audio_system" ) ;

    _appx_ptr->set_render_system( _rs_ptr ) ;
}

//***********************************************************************
appx_application::appx_application( this_rref_t rhv )
{
    _rwindows = std::move( rhv._rwindows ) ;

    so_move_member_ptr( _shared_data_ptr, rhv ) ;
    so_move_member_ptr( _appx_ptr, rhv ) ;
    so_move_member_ptr( _win_app_ptr, rhv ) ;
    so_move_member_ptr( _rs_ptr, rhv ) ;
    so_move_member_ptr( _as_ptr, rhv ) ;
}

//***********************************************************************
appx_application::~appx_application( void_t )
{
    // 1. trigger the shutdown of the update thread in order
    // to have the thread exit.
    // 2. wait for the destruction of the window application so
    // to be sure to join threads.
    // 3. after the windows have been successfully destroyed,
    // also destroy the internal window specific data.
    this_t::trigger_update_thread_shutdown() ;
    this_t::kill_window_application() ;
    this_t::kill_window_spcific_data() ;

    so_appx::memory::dealloc( _shared_data_ptr ) ;
    appx_system::destroy( _appx_ptr ) ;
    so_gpx::render_system_t::destroy( _rs_ptr ) ;
    so_audiox::audio_system_t::destroy( _as_ptr ) ;
}

//***********************************************************************
so_appx::result appx_application::register_app( so_appx::iapp_ptr_t app_ptr ) 
{
    return _appx_ptr->register_app( app_ptr ) ;
}

//***********************************************************************
so_appx::result appx_application::create_window( 
    so_app::gl_info_cref_t gli, so_app::window_info_cref_t wi ) 
{
    auto wid = _appx_ptr->register_window( wi.window_name ) ;
    if( wid == size_t(-1) )
        return so_appx::invalid_window_name ;

    so_app::gl_window_ptr_t render_window = 
        so_app::gl_window_t::create( so_app::gl_window_t(gli, wi) ) ;
    
    // 1. register normal window
    {
        auto res = _win_app_ptr->register_window( render_window ) ;
        if( so_app::no_success( res ) )
        {
            so_log::global::error( "[appx_application::create_window] : failed to create gl render window." ) ;
            return so_appx::failed ;
        }
    }

    // 2. register opengl window
    {
        auto const res = _rs_ptr->register_window( "gl_window_"+std::to_string(wid), render_window ) ;
        if( so_core::is_not( res ) )
        {
            so_log::global::error("[appx_application::create_window] : can not create render window") ;
            return so_appx::failed ;
        }
        so_thread::lock_guard_t lk( _shared_data_ptr->mtx ) ;
        _shared_data_ptr->rsystem_ptr = _rs_ptr ;
        _shared_data_ptr->asystem_ptr = _as_ptr ;
    }

    // manage window, micro window id and event listener
    {
        auto rwd = rwindow_data {
            wid, render_window, 
            so_app::window_message_receiver::create(
                "[appx_application::create_window] : render_window window_message_receiver" )
        } ;

        rwd.window_ptr->subscribe( rwd.receiver_ptr ) ;

        _rwindows.push_back( rwd ) ;
    }

    return so_appx::ok ;
}

//***********************************************************************
so_app::result appx_application::exec( void_t ) 
{
    typedef std::chrono::high_resolution_clock iclock_t ;

    auto shared_captured = _shared_data_ptr ;
    auto appx_captured = _appx_ptr ;

    auto update_funk = [=]( so_app::window_application::application_state_ptr_t as )
    {
        bool_t thread_running = true ;

        so_log::global::status( "[appx_application] : update thread online" ) ;
        
        update_data ud ;
        render_data rd ;

        while( thread_running )
        {
            if( shared_captured->shutdown_triggered )
                break ;

            iclock_t::time_point const t1 = iclock_t::now() ;
            
            so_device::global::device_system()->update() ;
            so_thread::global::update() ;
            
            

            shared_captured->rsystem_ptr->update() ;
            shared_captured->asystem_ptr->update() ;

            //
            // 1. swap the window event listener
            // 2. check for close events
            //
            for( auto & data : _rwindows )
            {
                data.receiver_ptr->swap() ;
                    
                so_app::close_message close_msg ;
                if( data.receiver_ptr->has_close_changed( close_msg ) )
                {
                    if( close_msg.close ) 
                    {
                        thread_running = false ;
                        break ;
                    }
                }
            }

            // we check the message receiver for a close event.
            // if so, break the loop. The shutdown code will be 
            // performed afterwards. Just be sure to keep the 
            // carrier application running until everything
            // has been shutdown.
            if( !thread_running ) break ;

            //
            // 1. Update all window events
            //

            for( auto & data : _rwindows )
            {
                if( !data.receiver_ptr->has_any_change() ) 
                    continue ;

                _appx_ptr->update( data.appx_wid, data.receiver_ptr ) ;
            }

            //
            // 2. Update apps
            //

            {
                auto const res = appx_captured->update( ud ) ;
                if( res == so_appx::terminate )
                {
                    for( auto & data : _rwindows )
                    {
                        data.window_ptr->send_close() ;
                    }
                }
            }

            
            // for each render window
            // wait for render sync
            shared_captured->rsystem_ptr->render_begin() ;
            shared_captured->asystem_ptr->render_begin() ;

            //
            // 3. Render apps
            //

            appx_captured->render( rd ) ;
                    
            //
            // 4. End the render system's frame
            //
            shared_captured->rsystem_ptr->render_end() ;
            shared_captured->asystem_ptr->render_end() ;
                
            iclock_t::duration const taken_dur = iclock_t::now() - t1 ;
            auto taken_dur_micro = std::chrono::duration_cast< std::chrono::microseconds >( taken_dur ) ;
            
            uint64_t const taken_micro = std::chrono::duration_cast< std::chrono::microseconds >( taken_dur ).count() ;
            uint64_t const taken_milli = std::chrono::duration_cast< std::chrono::milliseconds >( taken_dur ).count() ;
            uint64_t const taken_secs = std::chrono::duration_cast< std::chrono::seconds >( taken_dur ).count() ;
            double_t const dt = std::chrono::duration_cast< std::chrono::duration<double_t, std::ratio<1,1>> >( taken_dur_micro ).count() ;

            ud.microseconds = taken_micro ;
            ud.milliseconds = taken_milli ;
            ud.seconds = taken_secs ;
            ud.dt = dt ;
        }

        // perform the shutdown
        {
            _appx_ptr->shutdown() ;
            _rs_ptr->shutdown() ;
            as->update_finised() ;
        }

        so_log::global::status( "[appx_application] : update thread shutdown" ) ;
    };

    return _win_app_ptr->exec( update_funk ) ;
}

//***********************************************************************
so_appx::result appx_application::kill_window_spcific_data( void_t ) 
{
    for( auto & data : _rwindows )
    {
        data.receiver_ptr->destroy() ;
    }
    _rwindows.clear() ;

    return so_appx::ok ;
}

//***********************************************************************
so_appx::result appx_application::kill_window_application( void_t ) 
{
    if( _win_app_ptr != nullptr ) 
    {
        _win_app_ptr->destroy() ;
    }

    return so_appx::ok ;
}

//***********************************************************************
so_appx::result appx_application::trigger_update_thread_shutdown( void_t ) 
{
    if( _shared_data_ptr == nullptr )
        return so_appx::ok ;

    _shared_data_ptr->shutdown_triggered = true ;

    return so_appx::ok ;
}

//***********************************************************************
so_app::result appx_application::destroy( void_t ) 
{
    this_t::destroy( this ) ;
    return so_app::ok ;
}

