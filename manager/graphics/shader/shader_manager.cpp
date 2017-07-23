//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "shader_manager.h"
#include "../../code/code_manager.h"

#include <snakeoil/gpx/event/event.h>
#include <snakeoil/gpx/system/system.h>
#include <snakeoil/gpx/driver/driver_async.h>

#include <snakeoil/gpx/driver/command/command_list.h>
#include <snakeoil/gpu/shader/vertex_shader.h>
#include <snakeoil/gpu/shader/geometry_shader.h>
#include <snakeoil/gpu/shader/pixel_shader.h>

#include <snakeoil/log/log.h>

using namespace so_manager ;
using namespace so_manager::so_graphics ;

//*****************************************************************************************************
shader_manager::shader_manager( code_manager_ptr_t cptr, so_gpx::iuser_system_ptr_t gpxs_ptr ) :
    _vs_manager(gpxs_ptr), _gs_manager(gpxs_ptr), _ps_manager(gpxs_ptr)
{
    so_assert( cptr != nullptr ) ;
    so_assert( gpxs_ptr != nullptr ) ;

    _code_mgr_ptr = cptr ;
    _gpxs_ptr = gpxs_ptr ;
}

//*****************************************************************************************************
shader_manager::shader_manager( this_rref_t rhv ) :
    _vs_manager( std::move(rhv._vs_manager) ), _gs_manager( std::move(rhv._gs_manager) ), 
    _ps_manager( std::move(rhv._ps_manager) )
{
    _code_mgr_ptr = rhv._code_mgr_ptr ;
    rhv._code_mgr_ptr = nullptr ;
    _gpxs_ptr = rhv._gpxs_ptr ;
    rhv._gpxs_ptr = nullptr ;    
    _shutdown_called = rhv._shutdown_called ;
}

//*****************************************************************************************************
shader_manager::~shader_manager( void_t ) 
{

}

//*****************************************************************************************************
shader_manager::this_ptr_t shader_manager::create( this_rref_t rhv, so_memory::purpose_cref_t p ) 
{
    return so_manager::memory::alloc( std::move(rhv), p ) ;
}

//*****************************************************************************************************
void_t shader_manager::destroy( this_ptr_t ptr ) 
{
    so_manager::memory::dealloc( ptr ) ;
}

//*****************************************************************************************************
so_manager::result shader_manager::destroy( void_t ) 
{
    this_t::destroy( this ) ;
    return so_manager::ok ;
}

//*****************************************************************************************************
so_manager::result shader_manager::shutdown( void_t ) 
{
    if( _shutdown_called ) 
    {
        return this_t::has_shutdown() ;
    }

    so_thread::lock_t lk( _mtx ) ;

    _vs_manager.shutdown() ;
    _gs_manager.shutdown() ;
    _ps_manager.shutdown() ;

    _shutdown_called = true ;

    return so_manager::not_ready ;
}

//*****************************************************************************************************
so_manager::result shader_manager::has_shutdown( void_t ) 
{
    if( _vs_manager.shutdown_ready() == so_manager::not_ready ) 
        return so_manager::not_ready ;
    
    if( _gs_manager.shutdown_ready() == so_manager::not_ready ) 
        return so_manager::not_ready ;

    if( _ps_manager.shutdown_ready() == so_manager::not_ready ) 
        return so_manager::not_ready ;

    return so_manager::ok ;
}

//*****************************************************************************************************
so_manager::result shader_manager::compose_shader( 
    so_gpu::shader_type sht, so_manager::key_cref_t key, so_manager::key_list_cref_t code_keys ) 
{
    // compose 

    so_manager::string_t code ;
    if( so_manager::no_success( _code_mgr_ptr->compose( code_keys, code ) ) )
    {
        so_log::log::error("[shader_manager::compose_shader] : code keys invalid.") ;
        return so_manager::failed ;
    }

    // set the code

    so_thread::lock_t lk(_mtx) ;

    switch( sht ) 
    {
    case so_gpu::shader_type::vertex_shader:       
        return _vs_manager.replace_code( key, code ) ;
    case so_gpu::shader_type::geometry_shader:
        return _gs_manager.replace_code( key, code ) ;
    case so_gpu::shader_type::pixel_shader:
        return _ps_manager.replace_code( key, code ) ;
    default:
        return so_manager::invalid_argument ;
    }

    return so_manager::ok ;
}

//*****************************************************************************************************
so_manager::result shader_manager::compose_and_compile_shader( so_gpu::shader_type sht, so_manager::key_cref_t key_in, 
                so_manager::key_list_cref_t code_keys ) 
{
    if( so_manager::no_success(this_t::compose_shader( sht, key_in, code_keys)) )
        return so_manager::failed ;

    return this_t::compile_shader( sht, key_in ) ;
}

//*****************************************************************************************************
so_manager::result shader_manager::compile_shader( so_gpu::shader_type sht, so_manager::key_cref_t key_in ) 
{
    so_thread::lock_t lk(_mtx) ;

    switch( sht ) 
    {
    case so_gpu::shader_type::vertex_shader:
        return _vs_manager.compile_shader( key_in ) ;
    case so_gpu::shader_type::geometry_shader:
        return _gs_manager.compile_shader( key_in ) ;
    case so_gpu::shader_type::pixel_shader:
        return _ps_manager.compile_shader( key_in ) ;
    default:
        return so_manager::invalid_argument ;
    }

    return so_manager::ok ;
}

//*****************************************************************************************************
so_gpu::result shader_manager::get_compilation_status( so_gpu::shader_type sht, so_manager::key_cref_t key_in ) 
{
    so_thread::lock_t lk(_mtx) ;

    switch( sht ) 
    {
    case so_gpu::shader_type::vertex_shader:
        return _vs_manager.get_compilation_status( key_in ) ;
    case so_gpu::shader_type::geometry_shader:
        return _gs_manager.get_compilation_status( key_in ) ;
    case so_gpu::shader_type::pixel_shader:
        return _ps_manager.get_compilation_status( key_in ) ;
    default:
        return so_gpu::invalid_argument ;
    }
    return so_gpu::invalid_argument ;
}

//*****************************************************************************************************
so_gpu::result shader_manager::wait_for_compilation( so_gpu::shader_type sht, so_manager::key_cref_t key_in ) 
{
    so_gpx::event_ptr_t evt_ptr = nullptr ;

    // core managers are not thread safe, so the event
    // needs to be pulled out and waited in this function.
    // otherwise, the whole manager would stall due to one shader 
    // waiting for compilation.
    {
        so_thread::lock_t lk(_mtx) ;

        switch( sht ) 
        {
        case so_gpu::shader_type::vertex_shader:
            evt_ptr = _vs_manager.get_compilation_status_event( key_in ) ;
            break ;
        case so_gpu::shader_type::geometry_shader:
            evt_ptr = _gs_manager.get_compilation_status_event( key_in ) ;
            break ;
        case so_gpu::shader_type::pixel_shader:
            evt_ptr = _ps_manager.get_compilation_status_event( key_in ) ;
            break ;
        default:
            return so_gpu::invalid_argument ;
        }
    }

    if( evt_ptr == nullptr ) return so_gpu::invalid_argument ;

    return evt_ptr->wait_for_signal() ;
}

//*****************************************************************************************************
so_gpu::vertex_shader_ptr_t shader_manager::get_vertex_shader( so_manager::key_cref_t key_in ) 
{
    so_thread::lock_t lk(_mtx) ;
    return _vs_manager.get_shader( key_in ) ;
}

//*****************************************************************************************************
so_gpu::geometry_shader_ptr_t shader_manager::get_geometry_shader( so_manager::key_cref_t key_in )
{
    so_thread::lock_t lk(_mtx) ;
    return _gs_manager.get_shader( key_in ) ;
}

//*****************************************************************************************************
so_gpu::pixel_shader_ptr_t shader_manager::get_pixel_shader( so_manager::key_cref_t key_in )
{
    so_thread::lock_t lk(_mtx) ;
    return _ps_manager.get_shader( key_in ) ;
}

//*****************************************************************************************************
