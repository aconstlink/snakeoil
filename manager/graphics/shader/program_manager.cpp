//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "program_manager.h"
#include "shader_manager.h"

#include <snakeoil/gpu/program/program.h>
#include <snakeoil/gpu/program/config.h>
#include <snakeoil/gpx/system/system.h>
#include <snakeoil/gpx/system/iuser_system.h>
#include <snakeoil/gpx/driver/driver_async.h>
#include <snakeoil/gpx/event/event.h>

#include <snakeoil/log/log.h>

#include <snakeoil/core/macros/move.h>

using namespace so_manager ;
using namespace so_manager::so_graphics ;

//**********************************************************************************************
program_manager::this_ptr_t program_manager::create( this_rref_t rhv, so_memory::purpose_cref_t p ) 
{
    return so_manager::memory::alloc( std::move(rhv), p ) ;
}

//**********************************************************************************************
void_t program_manager::destroy( this_ptr_t ptr ) 
{
    so_manager::memory::dealloc( ptr ) ;
}

//**********************************************************************************************
program_manager::program_manager( so_manager::so_graphics::shader_manager_ptr_t ptr, 
    so_gpx::iuser_system_ptr_t gpx_ptr ) 
{
    _shader_mgr_ptr = ptr ;
    _gpxs_ptr = gpx_ptr ;
}

//**********************************************************************************************
program_manager::program_manager( this_rref_t rhv ) 
{   
    so_move_member_ptr( _gpxs_ptr, rhv ) ;
    so_move_member_ptr( _shader_mgr_ptr, rhv ) ;

    _key_to_program = std::move( rhv._key_to_program ) ;
    _key_to_program_shutdown = std::move( rhv._key_to_program_shutdown ) ;

    _key_to_config = std::move( rhv._key_to_config ) ;
    _key_to_config_shutdown = std::move( rhv._key_to_config_shutdown ) ;

    _shutdown_ready = std::move( rhv._shutdown_ready ) ;

    _shutdown_called = rhv._shutdown_called ;
    
    rhv._shutdown_called = true ;
    rhv._shutdown_ready.reset( so_gpu::ok ) ;

    _unique_counter = rhv._unique_counter ;
}

//**********************************************************************************************
program_manager::~program_manager( void_t ) 
{
    so_assert( _shutdown_called == true ) ;

    // programs
    {
        for( auto item : _key_to_program_shutdown )
        {
            so_gpu::program::destroy( item.second.program_ptr ) ;
            so_gpx::event::destroy( item.second.link_status ) ;
        }
        _key_to_program_shutdown.clear() ;
    }

    // configs
    {
        for( auto item : _key_to_config_shutdown )
        {
            so_gpu::config::destroy( item.second.config_ptr ) ;
        }
        _key_to_config_shutdown.clear() ;
    }
}

//**********************************************************************************************
so_manager::result program_manager::compose_program( so_manager::key_cref_t key_in, shader_keys_cref_t shk  ) 
{
    so_gpu::vertex_shader_ptr_t vs_ptr = nullptr ;
    so_gpu::geometry_shader_ptr_t gs_ptr = nullptr ;
    so_gpu::pixel_shader_ptr_t ps_ptr = nullptr ;

    {
        vs_ptr = _shader_mgr_ptr->get_vertex_shader( shk.vs_key ) ;
        if( so_log::log::error( vs_ptr == nullptr, 
            "[program_manager::compose_program] : invalid vertex shader key: " + shk.vs_key) ) 
            return so_manager::invalid_argument ;

        if( shk.gs_key != "" )
        {
            gs_ptr = _shader_mgr_ptr->get_geometry_shader( shk.gs_key ) ;
            if( so_log::log::error( vs_ptr == nullptr, 
                "[program_manager::compose_program] : invalid geometry shader key: " + shk.gs_key) ) 
                return so_manager::invalid_argument ;
        }
        
        if( shk.ps_key != "" )
        {
            ps_ptr = _shader_mgr_ptr->get_pixel_shader( shk.ps_key ) ;
            if( so_log::log::error( vs_ptr == nullptr, 
                "[program_manager::compose_program] : invalid pixel shader key: " + shk.ps_key) ) 
                return so_manager::invalid_argument ;
        }        
    }

    // all shaders are know and exist, so lock and create program
    {
        so_thread::lock_t lk(_mtx) ;
    
        {
            auto iter = _key_to_program.find( key_in ) ;
            if( iter != _key_to_program.end() ) 
                return so_manager::program_already_exists ;
        }

        so_gpu::program_ptr_t prog_ptr = so_gpu::program::create(
            "[program_manager::compose_program] : program" ) ;

        so_gpx::event_ptr_t link_event_ptr = so_gpx::event::create( 
            "[program_manager::compose_program] : event" ) ;

        prog_ptr->set_shader( vs_ptr ) ;
        prog_ptr->set_shader( gs_ptr ) ;
        prog_ptr->set_shader( ps_ptr ) ;

        program_data pd ;
        pd.program_ptr = prog_ptr ;
        pd.link_status = link_event_ptr ;

        _key_to_program[key_in] = pd ;

        _gpxs_ptr->execute( [=]( so_gpx::so_driver::driver_async_ptr_t drv_ptr )
        {
            drv_ptr->create_program( pd.program_ptr ) ;
        } ) ;
    }

    return so_manager::ok ;
}

//**********************************************************************************************
so_manager::result program_manager::link_program( so_manager::key_cref_t key_in ) 
{
    so_thread::lock_t lk( _mtx ) ;
    
    auto iter = _key_to_program.find( key_in ) ;
    if( iter == _key_to_program.end() ) 
        return so_manager::program_does_not_exist ;

    if( !iter->second.link_status->reset( so_gpu::mt_processing ) )
        return so_manager::program_already_linking ;

    _gpxs_ptr->execute( [=]( so_gpx::so_driver::driver_async_ptr_t drv_ptr )
    {
        drv_ptr->link( iter->second.program_ptr, iter->second.link_status ) ;
    } ) ;

    return so_manager::ok ;
}

//**********************************************************************************************
so_manager::result program_manager::compose_and_link_program( 
    so_manager::key_cref_t key_in, shader_keys_cref_t shk ) 
{
    auto comp_res = this_t::compose_program( key_in, shk ) ;
    if( so_manager::no_success(comp_res) ) return comp_res ;
    return this_t::link_program( key_in ) ;
}

//**********************************************************************************************
so_gpu::result program_manager::get_link_status( so_manager::key_cref_t key_in ) 
{
    so_thread::lock_t lk(_mtx) ;

    auto iter = _key_to_program.find( key_in ) ;
    if( iter == _key_to_program.end() ) 
        return so_gpu::invalid_argument ;

    return iter->second.link_status->get_result() ;
}

//**********************************************************************************************
so_gpu::result program_manager::wait_for_link( so_manager::key_cref_t key_in ) 
{
    so_gpx::event_ptr_t evt_ptr = nullptr ;
    {
        so_thread::lock_t lk(_mtx) ;

        auto iter = _key_to_program.find( key_in ) ;
        if( iter == _key_to_program.end() ) 
            return so_gpu::invalid_argument ;

        evt_ptr = iter->second.link_status ;
    }

    return evt_ptr->wait_for_signal() ;
}

//**********************************************************************************************
so_gpu::program_ptr_t program_manager::get_program( so_manager::key_cref_t key_in ) 
{
    so_thread::lock_t lk(_mtx) ;

    auto iter = _key_to_program.find( key_in ) ;
    if( iter == _key_to_program.end() ) 
        return nullptr ;

    return iter->second.program_ptr ;
}

//**********************************************************************************************
so_manager::result program_manager::compose_config( so_manager::key_cref_t config_key_in, so_manager::key_cref_t prog_key_in ) 
{
    so_thread::lock_t lk( _mtx ) ;
    {
        auto iter = _key_to_config.find( config_key_in ) ;
        if( iter != _key_to_config.end() ) 
            return so_manager::invalid_argument ;
    }

    auto iter = _key_to_program.find( prog_key_in ) ;
    if( iter == _key_to_program.end() ) 
        return so_manager::invalid_argument ;

    so_gpu::config_ptr_t config_ptr = so_gpu::config::create(
        "[program_manager::compose_config] : config" ) ;

    config_ptr->set_program( iter->second.program_ptr ) ;

    config_data cd ;
    cd.config_ptr = config_ptr ;

    _key_to_config[config_key_in] = cd ;

    _gpxs_ptr->execute( [=]( so_gpx::so_driver::driver_async_ptr_t drv_ptr )
    {
        drv_ptr->create_config( cd.config_ptr ) ;
    } ) ;

    return so_manager::ok ;
}

//**********************************************************************************************
so_gpu::config_ptr_t program_manager::get_config( so_manager::key_cref_t key_in ) 
{
    so_thread::lock_t lk( _mtx ) ;
    auto iter = _key_to_config.find( key_in ) ;
    if( iter == _key_to_config.end() ) return nullptr ;

    return iter->second.config_ptr ;
}

//**********************************************************************************************
so_manager::result program_manager::destroy( void_t ) 
{
    this_t::destroy( this ) ;
    return so_manager::ok ;
}

//**********************************************************************************************
so_manager::result program_manager::shutdown( void_t ) 
{
    if( _shutdown_called ) 
    {
        return this_t::has_shutdown() ;
    }

    _shutdown_ready.reset( so_gpu::mt_processing ) ;
    
    {
        so_thread::lock_t lk( _mtx ) ;
        _key_to_program_shutdown = std::move(_key_to_program) ;
    }
    {
        so_thread::lock_t lk( _mtx ) ;
        _key_to_config_shutdown = std::move(_key_to_config) ;
    }

    _gpxs_ptr->execute( [&]( so_gpx::so_driver::driver_async_ptr_t drv )
    {
        for( auto item : _key_to_config_shutdown )
        {
            drv->release_config( item.second.config_ptr ) ;
        }

        for( auto item : _key_to_program_shutdown )
        {
            drv->release_program( item.second.program_ptr ) ;
        }

        drv->flush() ;
        drv->finish( &_shutdown_ready ) ;
    } ) ;
    
    _shutdown_called = true ;

    return so_manager::not_ready ;
}

//**********************************************************************************************
so_manager::result program_manager::has_shutdown( void_t ) 
{
    if( !_shutdown_called ) return this_t::shutdown() ;

    return _shutdown_ready.get_result() == so_gpu::mt_processing ?
                so_manager::not_ready : so_manager::ok ;
}

//**********************************************************************************************
so_manager::key_t program_manager::compose_unique_config_key( so_manager::key_cref_t key_in ) 
{
    so_thread::lock_guard_t lk(_mtx) ;

    return key_in + "." + std::to_string(_unique_counter++) ;
}