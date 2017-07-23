//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "variable_manager.h"

#include <snakeoil/gpx/driver/driver_async.h>
#include <snakeoil/gpx/system/iuser_system.h>
#include <snakeoil/gpu/variable/variable_set.h>
#include <snakeoil/core/macros/move.h>

using namespace so_manager ;
using namespace so_manager::so_graphics ;

//*************************************************************************************
variable_manager::this_ptr_t variable_manager::create( so_memory::purpose_cref_t p )
{
    return so_manager::memory::alloc( this_t(), p ) ;
}

//*************************************************************************************
variable_manager::this_ptr_t variable_manager::create( this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_manager::memory::alloc( std::move(rhv), p ) ;
}

//*************************************************************************************
void_t variable_manager::destroy( this_ptr_t ptr )
{
    return so_manager::memory::dealloc( ptr ) ;
}

//*************************************************************************************
variable_manager::variable_manager( void_t ) 
{}

//*************************************************************************************
variable_manager::variable_manager( so_gpx::iuser_system_ptr_t ptr ) : _gpxs_ptr(ptr)
{}

//*************************************************************************************
variable_manager::variable_manager( this_rref_t rhv )
{
    _varsets = std::move( rhv._varsets ) ;
    _varsets_shutdown = std::move( rhv._varsets_shutdown ) ;

    so_move_member_ptr( _gpxs_ptr, rhv ) ;

    _shutdown_ready = std::move( rhv._shutdown_ready ) ;
    _shutdown_called = rhv._shutdown_called ;
    
    rhv._shutdown_called = true ;
    rhv._shutdown_ready.reset( so_gpu::ok ) ;
}

//*************************************************************************************
variable_manager::~variable_manager( void_t )
{
    so_assert( _shutdown_called == true ) ;

    // variable sets
    {
        for( auto item : _varsets_shutdown )
        {
            so_gpu::variable_set::destroy( item.second.var_set_ptr ) ;
        }
        _varsets_shutdown.clear() ;
    }
}

//*************************************************************************************
so_manager::result variable_manager::destroy( void_t )
{
    this_t::destroy( this ) ;
    return so_manager::ok ;
}

//*************************************************************************************
so_manager::result variable_manager::shutdown( void_t )
{
    if( _shutdown_called )
    {
        return this_t::has_shutdown() ;
    }

    _shutdown_ready.reset( so_gpu::mt_processing ) ;

    {
        so_thread::lock_t lk( _mtx ) ;
        _varsets_shutdown = std::move( _varsets ) ;
    }   

    _gpxs_ptr->execute( [&]( so_gpx::so_driver::driver_async_ptr_t drv_ptr )
    {
        for( auto item : _varsets_shutdown )
        {
            drv_ptr->release_variable( item.second.var_set_ptr ) ;
        }

        drv_ptr->flush() ;
        drv_ptr->finish( &_shutdown_ready ) ;
    } ) ;

    _shutdown_called = true ;

    return so_manager::not_ready ;
}

//*************************************************************************************
so_manager::result variable_manager::has_shutdown( void_t )
{
    if( !_shutdown_called ) return this_t::shutdown() ;

    return _shutdown_ready.get_result() == so_gpu::mt_processing ?
                so_manager::not_ready : so_manager::ok ;
}

//*************************************************************************************
so_manager::result variable_manager::create_variable_set(
    so_manager::key_cref_t key_in, so_memory::purpose_cref_t p )
{
    so_gpu::variable_set_ptr_t var_set_ptr ;

    {
        so_thread::lock_guard_t lk( _mtx ) ;

        auto found = _varsets.find( key_in ) ;
        if(found != _varsets.end())
            return so_manager::key_already_in_use ;

        variable_set_data vsd ;
        vsd.var_set_ptr = so_gpu::variable_set::create( p ) ;

        _varsets[key_in] = vsd ;

        var_set_ptr = vsd.var_set_ptr ;
    }

    _gpxs_ptr->execute( [var_set_ptr]( so_gpx::so_driver::driver_async_ptr_t drv_ptr )
    {
        drv_ptr->create_variable(var_set_ptr) ;
    } ) ;

    return so_manager::ok ;
}

//*************************************************************************************
so_manager::result variable_manager::destroy_variable_set(
    so_manager::key_cref_t key_in )
{
    variable_set_data vsd ;

    {
        so_thread::lock_guard_t lk( _mtx ) ;

        auto found = _varsets.find( key_in ) ;
        if(found == _varsets.end())
            return so_manager::invalid_argument ;

        vsd = found->second ;

        _varsets.erase( found ) ;
    }

    _gpxs_ptr->execute( [vsd]( so_gpx::so_driver::driver_async_ptr_t drv_ptr )
    {
        drv_ptr->release_variable( vsd.var_set_ptr ) ;
        so_gpu::variable_set::destroy( vsd.var_set_ptr ) ;
    } ) ;

    return so_manager::ok ;
}

//*************************************************************************************
so_manager::result variable_manager::destroy_variable_set( so_gpu::variable_set_ptr_t var_set_ptr )
{
    if( var_set_ptr == nullptr )
        return so_manager::invalid_argument ;

    _gpxs_ptr->execute( [var_set_ptr]( so_gpx::so_driver::driver_async_ptr_t drv_ptr )
    {
        drv_ptr->release_variable( var_set_ptr ) ;
        so_gpu::variable_set::destroy( var_set_ptr ) ;
    } ) ;

    return so_manager::ok ;
}

//*************************************************************************************
so_manager::result variable_manager::manage_variable_set(
    so_manager::key_cref_t key_in, so_gpu::variable_set_ptr_t var_set_ptr )
{
    if( var_set_ptr == nullptr ) 
        return so_manager::invalid_argument ;

    so_thread::lock_guard_t lk(_mtx)  ;
    
    auto found = _varsets.find( key_in ) ;
    if( found != _varsets.end() )
        return so_manager::key_already_in_use ;

    variable_set_data vsd ;
    vsd.var_set_ptr = var_set_ptr ;

    _varsets[key_in] = vsd ;

    return so_manager::ok ;
}

//*************************************************************************************
so_gpu::variable_set_ptr_t variable_manager::get_variable_set(
    so_manager::key_cref_t key_in ) 
{
    so_gpu::variable_set_ptr_t ptr_out ;

    {
        so_thread::lock_guard_t lk(_mtx) ;
        auto found = _varsets.find( key_in ) ;
        if( found == _varsets.end() )
            return nullptr ;

        ptr_out = found->second.var_set_ptr ;
    }

    return ptr_out ;
}

//*************************************************************************************