//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "render_system.h"

#include "../window/iwindow.h"
#include "../technique/technique.h"

#include <snakeoil/gpu/manager/gpu_manager.h>
#include <snakeoil/thread/mutex.h>
#include <snakeoil/log/log.h>

#include <thread>

using namespace so_gpx ;

//**********************************************************************************************
void_t render_system::shared_object::sync_00_wait( void_t )
{    
    so_thread::lock_t lk( mtx_00 ) ;
    while( so_core::is_not( sync_00 ) && so_core::is_not( shutdown ) ) cv_00.wait( lk ) ;
    sync_00 = false ;
}

//**********************************************************************************************
void_t render_system::shared_object::sync_00_send( void_t )
{
    {
        so_thread::lock_guard_t lk( mtx_00 ) ;
        sync_00 = true ;
    }
    cv_00.notify_all() ;
}

//**********************************************************************************************
void_t render_system::shared_object::sync_01_wait( void_t )
{
    so_thread::lock_t lk( mtx_01 ) ;
    while( so_core::is_not( sync_01 ) && so_core::is_not( shutdown ) ) cv_01.wait( lk ) ;
    sync_01 = false ;
}

//**********************************************************************************************
void_t render_system::shared_object::sync_01_send( void_t )
{
    {
        so_thread::lock_guard_t lk( mtx_01 ) ;
        sync_01 = true ;
    }
    cv_01.notify_all() ;
}

//**********************************************************************************************
void_t render_system::shared_object::send_shutdown( void_t )
{
    so_thread::lock_t lk( mtx_sd ) ;
    shutdown = true ;
    shutdown_ok = false ;
    sync_00_send() ;
    sync_01_send() ;
}

//**********************************************************************************************
void_t render_system::shared_object::wait_for_shutdown( void_t )
{
    so_thread::lock_t lk( mtx_sd ) ;
    while( so_core::is_not( shutdown_ok ) ) cv_shutdown.wait( lk ) ;
}

//**********************************************************************************************
void_t render_system::shared_object::send_shutdown_ok( void_t )
{
    {
        so_thread::lock_t lk( mtx_sd ) ;
        shutdown_ok = true ;
    }
    cv_shutdown.notify_all() ;
}

//**********************************************************************************************
void_t render_system::shared_object::flip_scheduled( void_t )
{
    so_thread::lock_guard_t lk( mtx_scheduled ) ;
    read_buffer_id = ++read_buffer_id & 1 ;
}

//**********************************************************************************************
render_system::schedule_datas_ref_t render_system::shared_object::read_buffer( void_t )
{
    return scheduled[ read_buffer_id ] ;
}

//**********************************************************************************************
render_system::schedule_datas_ref_t render_system::shared_object::write_buffer( void_t )
{
    return scheduled[ (read_buffer_id+1) & 1 ] ;
}

//**********************************************************************************************
render_system::render_system( void_t )
{
    _service_ptr = so_gpx::service_t::create( 
        "[render_system::render_system] : global service" ) ;
}

//**********************************************************************************************
render_system::render_system( this_rref_t rhv )
{
    _wis = std::move( rhv._wis ) ;
    so_move_member_ptr( _service_ptr, rhv ) ;
}

//**********************************************************************************************
render_system::~render_system( void_t )
{
    for( auto & td : _tecs )
    {
        so_gpx::technique_t::destroy( td.second.tptr ) ;
    }

    for( auto & wi : _wis )
    {
        if( so_core::is_not_nullptr( wi.shared_ptr->service_ptr ) )
            wi.shared_ptr->service_ptr->destroy() ;

        if( so_core::is_not_nullptr( wi.shared_ptr->gpu_mgr_ptr ) )
            so_gpu::gpu_manager::destroy( wi.shared_ptr->gpu_mgr_ptr ) ;

        so_gpx::memory::dealloc( wi.shared_ptr ) ;
    }

    if( so_core::is_not_nullptr(_service_ptr) )
        _service_ptr->destroy() ;
}

//**********************************************************************************************
render_system::this_ptr_t render_system::create( so_memory::purpose_cref_t p )
{
    return so_gpx::memory::alloc( this_t(), p ) ;
}

//**********************************************************************************************
render_system::this_ptr_t render_system::create( this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_gpx::memory::alloc( std::move( rhv ), p ) ;
}

//**********************************************************************************************
void_t render_system::destroy( this_ptr_t ptr )
{
    so_gpx::memory::dealloc( ptr ) ;
}

//**********************************************************************************************
bool_t render_system::register_window( so_std::string_cref_t name, so_gpx::iwindow_ptr_t wnd )
{
    auto const iter = std::find_if( _wis.begin(), _wis.end(), [&]( window_info_cref_t wi )
    {
        return wi.name == name || wi.wnd_ptr == wnd ;
    } ) ;

    if( iter != _wis.end() )
        return false ;

    auto * shared_ptr = so_gpx::memory::alloc( this_t::shared_object_t(),
        "[render_system::render_system] : shared object" ) ;

    shared_ptr->service_ptr = so_gpx::service_t::create( 
        "[render_system::register_window] : window service for " + name ) ;

    shared_ptr->gpu_mgr_ptr = so_gpu::gpu_manager_t::create( 
        "[render_system::register_window] : gpu_manager for " + name ) ;

    window_info_t wi ;
    wi.name = name ;
    wi.wnd_ptr = wnd ;
    wi.shared_ptr = shared_ptr ;

    so_gpx::iglobal_service_ptr_t service_ptr = _service_ptr ;

    _wis.emplace_back( wi ) ;

    so_gpx::window_id_t const wid( _wis.size() - 1 ) ;

    std::thread( [shared_ptr, wid, wnd, service_ptr]( void_t )
    {
        wnd->render_thread_begin() ;

        so_std::vector< schedule_data_t > stage_00_0 ;
        so_std::vector< schedule_data_t > stage_00_1 ;
        so_std::vector< so_gpx::technique_ptr_t > stage_01 ;

        while( so_core::is_not( shared_ptr->shutdown ) )
        {
            // part 0: wait for the user sync to be ready
            // in this part, all gpu creation and transfer should be done
            shared_ptr->sync_00_wait() ;

            shared_ptr->flip_scheduled() ;
            shared_ptr->write_buffer().clear() ;

            stage_00_0.reserve( shared_ptr->read_buffer().size() ) ;
            stage_00_1.reserve( shared_ptr->read_buffer().size() ) ;

            stage_01.reserve( shared_ptr->read_buffer().size() ) ;

            // 0 : proceed with states of unfinished techniques
            {
                for( auto & sd : stage_00_0 )
                {
                    auto const res = sd.tptr->check_part_00_finished( sd.ts, wid ) ;

                    if( res == so_gpx::technique_transition_result::in_transition )
                    {
                        stage_00_1.push_back( sd ) ;
                    }
                    else
                    {
                        auto const iter = std::find_if(
                            shared_ptr->read_buffer().begin(), shared_ptr->read_buffer().end(), [&]( schedule_data_t const & v )
                        {
                            return v.tptr == sd.tptr ;
                        } ) ;

                        if( iter == shared_ptr->read_buffer().end() )
                            shared_ptr->read_buffer().push_back( sd ) ;
                    }
                }
                stage_00_0.clear() ;

                for( auto & sd : stage_00_1 )
                    stage_00_0.push_back( sd ) ;

                stage_00_1.clear() ;
            }
            
            // 1. proceed with states of new techniques
            // if a technique is ready, is will be scheduled for stage 1
            {
                for( auto & sd : shared_ptr->read_buffer() )
                {
                    auto const res = sd.tptr->part_00( sd.ts, wid, wnd->get_api(), 
                        service_ptr, shared_ptr->service_ptr, shared_ptr->gpu_mgr_ptr ) ;

                    // async states need to be scheduled again, automatically
                    if( res != so_gpx::technique_transition_result::goal_reached )
                    {
                        auto const iter = std::find_if( 
                            stage_00_0.begin(), stage_00_0.end(), [&]( schedule_data_t const & v )
                        {
                            return v.tptr == sd.tptr ;
                        } ) ;

                        if( iter == stage_00_0.end() )
                            stage_00_0.push_back( sd ) ;
                    }
                }
            }

            shared_ptr->sync_01_send() ;
            
            // part 1: send the user that all data is created/transfered.
            // the user can now overwrite the old data in the driver side
            // and schedule more techniques
            {
                for( auto & sd : shared_ptr->read_buffer() )
                {
                    sd.tptr->part_01_render( wid, wnd->get_api() ) ;
                }
            }

            wnd->end_frame() ;
        }

        // clear out all technique buffers

        while( true )
        {
            if( shared_ptr->read_buffer().size() == 0 )
                break ;

            for( auto & sd : shared_ptr->read_buffer() )
            {
                auto const res = sd.tptr->part_00( so_gpx::technique_schedule_goal::for_shutdown, 
                    wid, wnd->get_api(), service_ptr, shared_ptr->service_ptr, shared_ptr->gpu_mgr_ptr ) ;

                if( res != so_gpx::technique_transition_result::goal_reached )
                    shared_ptr->write_buffer().push_back( sd ) ;

                sd.tptr->part_01_render_pseudo( wid ) ;
            }

            shared_ptr->flip_scheduled() ;
            shared_ptr->write_buffer().clear() ;
        }

        wnd->render_thread_end() ;

        shared_ptr->send_shutdown_ok() ;

    } ).detach() ;

    return true ;
}

//**********************************************************************************************
so_gpx::technique_id_t render_system::register_technique( so_gpx::iplug_factory_ptr_t fac_ptr )
{
    auto * tptr = so_gpx::technique_t::create( so_gpx::technique_t( fac_ptr ), 
        "[render_system::register_technique]" ) ;

    this_t::technique_data_t d ;
    d.tptr = tptr ;

    technique_id_t ret_id ;

    {
        so_thread::lock_guard_t lk( _mtx_tecs ) ;
        _tecs[ _cur_id ] = d ;
        ret_id = _cur_id++ ;
    }

    return ret_id ;
}

//**********************************************************************************************
so_gpx::window_id_t render_system::get_window_id( so_std::string_cref_t name )
{
    auto const iter = std::find_if( _wis.begin(), _wis.end(), [&]( window_info_cref_t wi )
    {
        return wi.name == name  ;
    } ) ;

    return iter == _wis.end() ? window_id_t( -1 ) : window_id_t( iter - _wis.begin() ) ;
}

//**********************************************************************************************
so_gpx::result render_system::shutdown( void_t )
{
    for( auto & w : _wis )
    {
        w.shared_ptr->send_shutdown() ;
    }

    for( auto & w : _wis )
    {
        w.shared_ptr->wait_for_shutdown() ;
    }

    return so_gpx::ok ;
}

//**********************************************************************************************
so_gpx::iglobal_service_ptr_t render_system::get_service( void_t ) 
{
    return _service_ptr ;
}

//**********************************************************************************************
so_gpx::ilocal_service_ptr_t render_system::get_service( window_id_t wid ) 
{
    if( wid >= _wis.size() )
        return nullptr ;

    return _wis[wid].shared_ptr->service_ptr ;
}

//**********************************************************************************************
so_gpx::result render_system::schedule( so_gpx::technique_id_t tid )
{
    return this_t::schedule_for( tid, window_id_t(0), technique_schedule_goal::for_exec ) ;
}

//**********************************************************************************************
so_gpx::result render_system::schedule( so_gpx::technique_id_t tid, so_gpx::window_id_t wid )
{
    return this_t::schedule_for( tid, wid, technique_schedule_goal::for_exec ) ;
}

//**********************************************************************************************
so_gpx::result render_system::schedule_for_init( so_gpx::technique_id_t tid )
{
    return this_t::schedule_for( tid, window_id_t( 0 ), technique_schedule_goal::for_online ) ;
}

//**********************************************************************************************
so_gpx::result render_system::schedule_for_init( so_gpx::technique_id_t tid, so_gpx::window_id_t wid )
{
    return this_t::schedule_for( tid, wid, technique_schedule_goal::for_online ) ;
}

//**********************************************************************************************
so_gpx::result render_system::schedule_for_release( so_gpx::technique_id_t tid, so_gpx::window_id_t wid )
{
    return this_t::schedule_for( tid, wid, technique_schedule_goal::for_shutdown ) ;
}

//**********************************************************************************************
so_gpx::result render_system::schedule_for_release( so_gpx::technique_id_t tid )
{
    return this_t::schedule_for( tid, window_id_t( 0 ), technique_schedule_goal::for_shutdown ) ;
}

//**********************************************************************************************
so_gpx::result render_system::schedule_for( so_gpx::technique_id_t tid, so_gpx::window_id_t wid, 
    so_gpx::technique_schedule_goal goal )
{
    if( _wis.size() == 0 )
        return so_gpx::failed ;

    so_gpx::technique_ptr_t tptr = nullptr ;
    
    {
        so_thread::lock_guard_t lk( _mtx_tecs ) ;
        auto const iter = _tecs.find( tid ) ;
        if( iter == _tecs.end() )
            return so_gpx::invalid_argument ;

        tptr = iter->second.tptr ;
    }

    {
        schedule_data_t sd ;
        sd.ts = goal ;
        sd.tptr = tptr ;

        so_thread::lock_guard_t lk( _wis[ wid ].shared_ptr->mtx_scheduled ) ;
        _wis[ wid ].shared_ptr->write_buffer().push_back( sd ) ;
    }

    return so_gpx::ok ;
}

//**********************************************************************************************
so_gpx::result render_system::schedule_for_init_and_wait( so_gpx::technique_id_t tid )
{
    {
        auto const res = this_t::schedule_for_init( tid ) ;
        if( so_gpx::no_success( res ) )
            return res ;
    }

    return this_t::wait_for_init( tid ) ;
}

//**********************************************************************************************
so_gpx::result render_system::wait_for_init( so_gpx::technique_id_t tid )
{
    if( _wis.size() == 0 )
        return so_gpx::failed ;

    so_gpx::technique_ptr_t tptr = nullptr ;

    {
        so_thread::lock_guard_t lk( _mtx_tecs ) ;
        auto const iter = _tecs.find( tid ) ;
        if( iter == _tecs.end() )
            return so_gpx::invalid_argument ;

        tptr = iter->second.tptr ;
    }

    auto const res = tptr->wait_for_init( so_gpx::window_id_t( 0 ) ) ;
    return so_core::is_not( res ) ? so_gpx::failed : so_gpx::ok  ;
}

//**********************************************************************************************
void_t render_system::send_sync_00( void_t )
{
    for( auto & wnd : _wis )
    {
        wnd.shared_ptr->sync_00_send() ;
    }
}

//**********************************************************************************************
void_t render_system::wait_for_sync_01( void_t )
{
    for( auto & wnd : _wis )
    {
        wnd.shared_ptr->sync_01_wait() ;
    }
}

//**********************************************************************************************
void_t render_system::update_gpu_techniques( void_t )
{
    size_t wid = 0 ;
    for( auto & w : _wis )
    {
        for( auto & sd : w.shared_ptr->read_buffer() )
        {
            sd.tptr->part_01_update( window_id_t(wid) ) ;
        }
        ++wid ;
    }
}

//**********************************************************************************************
void_t render_system::update( void_t )
{
    // sync point 0 : wait for user thread(s) in render threads
    this_t::send_sync_00() ;

    // gpu transfer happens here until render threads signal sync_01
}

//**********************************************************************************************
void_t render_system::render_begin( void_t )
{
    // sync point 1 : wait for render thread(s)
    this_t::wait_for_sync_01() ;

    
}

//**********************************************************************************************
void_t render_system::render_end( void_t )
{
    // [update thread] user render data and update <-> [render thread] on_render
    this_t::update_gpu_techniques() ;
}