//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "framebuffer_manager.h"

#include <snakeoil/gpu/framebuffer/framebuffer_2d.h>
#include <snakeoil/gpu/framebuffer/framebuffer_3d.h>

#include <snakeoil/gpx/system/system.h>
#include <snakeoil/gpx/driver/driver_async.h>
#include <snakeoil/gpx/event/event.h>

using namespace so_manager ;
using namespace so_manager::so_graphics ;

//**********************************************************************************************
framebuffer_manager::framebuffer_manager( so_gpx::iuser_system_ptr_t ptr ) 
{
    so_assert( ptr != nullptr ) ;
    _gpx_ptr = ptr ;
}

//**********************************************************************************************
framebuffer_manager::framebuffer_manager( this_rref_t rhv ) 
{
    _gpx_ptr = rhv._gpx_ptr ;
    _2ds = std::move( rhv._2ds ) ;
    _3ds = std::move( rhv._3ds ) ;
    _shutdown_called = rhv._shutdown_called ;
}

//**********************************************************************************************
framebuffer_manager::~framebuffer_manager( void_t ) 
{
    for( auto item : _2ds )
    {
        item.second.obj_ptr->destroy() ;
    }

    for( auto item : _3ds )
    {
        //item.second.obj_ptr->destroy() ;
    }
}

//**********************************************************************************************
framebuffer_manager::this_ptr_t framebuffer_manager::create( this_rref_t rhv, so_memory::purpose_cref_t purpose) 
{
    return so_manager::memory::alloc( std::move(rhv), purpose ) ;
}

//**********************************************************************************************
void_t framebuffer_manager::destroy( this_ptr_t ptr ) 
{
    so_manager::memory::dealloc(ptr) ;
}

//**********************************************************************************************
so_manager::result framebuffer_manager::manage( 
    so_manager::key_cref_t key_in, so_gpu::framebuffer_2d_ptr_t obj_ptr) 
{
    if( obj_ptr == nullptr ) 
        return so_manager::invalid_argument ;

    if( _shutdown_called == true )
        return so_manager::failed ;

    {
        so_thread::lock_guard_t lk( _2d_mtx ) ;

        auto iter = _2ds.find( key_in ) ;
        if( iter != _2ds.end() )
        {
            return ( iter->second.obj_ptr == obj_ptr ) ? 
                so_manager::ok : so_manager::invalid_argument ;
        }

        framebuffer_2d_data i2d ;
        i2d.obj_ptr = obj_ptr ;

        _2ds[key_in] = i2d ;
    }

    _gpx_ptr->execute( [=]( so_gpx::so_driver::driver_async_ptr_t drv )
    {
        drv->create_framebuffer( obj_ptr ) ;
    } ) ;

    return so_manager::ok ;
}

//**********************************************************************************************
so_manager::result framebuffer_manager::manage( 
    so_manager::key_cref_t key_in, so_gpu::framebuffer_3d_ptr_t obj_ptr ) 
{
    if( obj_ptr == nullptr ) 
        return so_manager::invalid_argument ;

    if( _shutdown_called == true )
        return so_manager::failed ;

    {
        so_thread::lock_guard_t lk( _3d_mtx ) ;

        auto iter = _3ds.find( key_in ) ;
        if( iter != _3ds.end() )
        {
            return ( iter->second.obj_ptr == obj_ptr ) ? 
                so_manager::ok : so_manager::invalid_argument ;
        }

        framebuffer_3d_data i3d ;
        i3d.obj_ptr = obj_ptr ;

        _3ds[key_in] = i3d ;
    }

    _gpx_ptr->execute( [=]( so_gpx::so_driver::driver_async_ptr_t /*drv*/ )
    {
        //drv->create_framebuffer( obj_ptr ) ;
    } ) ;

    // do not support 3d images at the moment
    return so_manager::failed ;
}

//**********************************************************************************************
so_manager::result framebuffer_manager::has_framebuffer_2d( so_manager::key_cref_t key_in ) 
{
    so_thread::lock_guard_t lk( _2d_mtx ) ;
    auto iter = _2ds.find( key_in ) ;
    return ( iter != _2ds.end() ) ? so_manager::ok : so_manager::invalid_argument ;
}

//**********************************************************************************************
so_manager::result framebuffer_manager::has_framebuffer_3d( so_manager::key_cref_t key_in ) 
{
    so_thread::lock_guard_t lk( _3d_mtx ) ;
    auto iter = _3ds.find( key_in ) ;
    return ( iter != _3ds.end() ) ? so_manager::ok : so_manager::invalid_argument ;
}

//**********************************************************************************************
so_manager::result framebuffer_manager::get_framebuffer_2d( 
    so_manager::key_cref_t key_in, framebuffer_2d_info_ref_t info_out ) 
{
    if( _shutdown_called == true )
        return so_manager::failed ;

    {
        so_thread::lock_guard_t lk( _2d_mtx ) ;

        auto iter = _2ds.find( key_in ) ;
        if( iter == _2ds.end() )
            return so_manager::invalid_argument ;

        info_out.obj_ptr = iter->second.obj_ptr ;
    }

    return so_manager::ok ;
}

//**********************************************************************************************
so_manager::result framebuffer_manager::get_framebuffer_3d( 
    so_manager::key_cref_t key_in, framebuffer_3d_info_ref_t info_out ) 
{
    if( _shutdown_called == true )
        return so_manager::failed ;

    {
        so_thread::lock_guard_t lk( _3d_mtx ) ;

        auto iter = _3ds.find( key_in ) ;
        if( iter == _3ds.end() )
            return so_manager::invalid_argument ;

        info_out.obj_ptr = iter->second.obj_ptr ;
    }

    return so_manager::ok ;
}

//**********************************************************************************************
so_manager::result framebuffer_manager::validate_and_wait( so_manager::key_cref_t key_in ) 
{
    so_gpu::framebuffer_2d_ptr_t fb_ptr = nullptr ;

    {
        so_thread::lock_guard_t lk(_2d_mtx) ;
        auto iter = _2ds.find( key_in ) ;
        if( iter == _2ds.end() )
            return so_manager::invalid_argument ;

        fb_ptr = iter->second.obj_ptr ;
    }

    so_gpx::event_t evt ;
    evt.reset( so_gpu::mt_processing ) ;

    so_gpx::event_ptr_t evt_ptr = &evt ;
    _gpx_ptr->execute( [=]( so_gpx::so_driver::driver_async_ptr_t drv )
    {
        drv->validate( fb_ptr, evt_ptr ) ;
    } ) ;

    auto const res = evt.wait_for_signal() ;

    return so_gpu::no_success(res) ? so_manager::failed : so_manager::ok  ;
}

//**********************************************************************************************
void_t framebuffer_manager::destroy( void_t ) 
{
    this_t::destroy( this ) ;
}

//**********************************************************************************************
so_manager::result framebuffer_manager::shutdown( void_t ) 
{
    if( _shutdown_called == true )
        return so_manager::ok ;

    _shutdown_called = true ;

    _gpx_ptr->execute( [=]( so_gpx::so_driver::driver_async_ptr_t drv )
    {
        for( auto item : _2ds )
        {
            drv->release_framebuffer( item.second.obj_ptr ) ;
        }

        // for 3d
        /*for( auto item : _image_3ds )
        {
            drv->release_image( item.second.obj_ptr ) ;
        }*/

        this->_shutdown_complete = true ;
    } ) ;

    return so_manager::ok ;
}

//**********************************************************************************************
so_manager::result framebuffer_manager::has_shutdown( void_t ) 
{
    if( _shutdown_called == false )
    {
        this_t::shutdown() ;
    }
    return _shutdown_complete ? so_manager::ok : so_manager::not_ready ;
}

//**********************************************************************************************
