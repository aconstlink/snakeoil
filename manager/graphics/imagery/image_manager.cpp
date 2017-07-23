//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "image_manager.h"

#include <snakeoil/gpu/image/iimage_2d.h>

#include <snakeoil/gpx/system/system.h>
#include <snakeoil/gpx/driver/driver_async.h>

using namespace so_manager ;
using namespace so_manager::so_graphics ;

//**********************************************************************************************
image_manager::image_manager( so_gpx::iuser_system_ptr_t ptr ) : _gpx_ptr(ptr)
{
    so_log::log::error_and_exit( so_core::is_nullptr(ptr), 
        "[so_manager::image_manager::image_manager] : ptr must != nullptr" ) ;
}

//**********************************************************************************************
image_manager::image_manager( this_rref_t rhv ) 
{
    _i2d_res_mgr = std::move( rhv._i2d_res_mgr ) ;
    _i3d_res_mgr = std::move( rhv._i3d_res_mgr ) ;

    _gpx_ptr = rhv._gpx_ptr ;
    _shutdown_called = rhv._shutdown_called ;
    _shutdown_ready = std::move( rhv._shutdown_ready ) ;
}

//**********************************************************************************************
image_manager::~image_manager( void_t ) 
{
    _i2d_res_mgr.call_for_each( [&]( i2d_store_item_ptr_t item_ptr )
    {
        item_ptr->obj_ptr->destroy() ;
        so_manager::memory::dealloc( item_ptr ) ;
    } ) ;

    /// @todo 3d texture destruction is incomplete
    _i3d_res_mgr.call_for_each( [&]( i3d_store_item_ptr_t item_ptr )
    {} ) ;
}

//**********************************************************************************************
image_manager::this_ptr_t image_manager::create( this_rref_t rhv, so_memory::purpose_cref_t purpose) 
{
    return so_manager::memory::alloc( std::move(rhv), purpose ) ;
}

//**********************************************************************************************
void_t image_manager::destroy( this_ptr_t ptr ) 
{
    so_manager::memory::dealloc(ptr) ;
}

//**********************************************************************************************
bool_t image_manager::acquire( so_manager::key_in_t key_in,
    so_resource::purpose_in_t p, i2d_handle_out_t hnd_out ) 
{
    return _i2d_res_mgr.acquire( key_in, p, hnd_out ) ;
}

//**********************************************************************************************
bool_t image_manager::acquire( so_manager::key_in_t key_in,
    so_resource::purpose_in_t p, i3d_handle_out_t hnd_out )
{
    return _i3d_res_mgr.acquire( key_in, p, hnd_out ) ;
}

//**********************************************************************************************
bool_t image_manager::release( i2d_handle_rref_t hnd )
{
    return _i2d_res_mgr.release( std::move( hnd ) ) ;
}

//**********************************************************************************************
bool_t image_manager::release( i3d_handle_rref_t hnd )
{
    return _i3d_res_mgr.release( std::move( hnd ) ) ;
}

//**********************************************************************************************
so_manager::result image_manager::insert( so_manager::key_in_t key_in, i2d_manage_params_in_t mp )
{
    i2d_store_item_t si ;
    si.obj_ptr = mp.obj_ptr ;

    i2d_store_item_ptr_t si_ptr = so_manager::memory::alloc( std::move( si ),
        "[so_manager::image_manager::insert] : i2d_store_item_t : " + key_in ) ;

    auto const res = _i2d_res_mgr.insert( key_in, si_ptr ) ;
    if( so_log::log::error( so_resource::no_success( res ),
        "[so_manager::image_manager::insert] : insert" ) )
    {
        so_manager::memory::dealloc( si_ptr ) ;
        return so_manager::key_already_in_use;
    }

    _gpx_ptr->execute( [=]( so_gpx::so_driver::driver_async_ptr_t drv )
    {
        drv->create_image( mp.obj_ptr ) ;
    } ) ;

    return so_manager::ok ;
}

//**********************************************************************************************
so_manager::result image_manager::insert( so_manager::key_in_t key_in, i3d_manage_params_in_t mp )
{
    i3d_store_item_t si ;
    si.obj_ptr = mp.obj_ptr ;

    i3d_store_item_ptr_t si_ptr = so_manager::memory::alloc( std::move( si ),
        "[so_manager::image_manager::insert] : i3d_store_item_t : " + key_in ) ;

    auto const res = _i3d_res_mgr.insert( key_in, si_ptr ) ;
    if( so_log::log::error( so_resource::no_success( res ),
        "[so_manager::image_manager::insert] : insert" ) )
    {
        so_manager::memory::dealloc( si_ptr ) ;
        return so_manager::key_already_in_use;
    }

    _gpx_ptr->execute( [=]( so_gpx::so_driver::driver_async_ptr_t drv )
    {
        //drv->create_image( mp.obj_ptr ) ;
    } ) ;

    return so_manager::ok ;
}

//**********************************************************************************************
void_t image_manager::destroy( void_t ) 
{
    this_t::destroy( this ) ;
}

//**********************************************************************************************
so_manager::result image_manager::shutdown( void_t ) 
{
    if( _shutdown_called )
    {
        return _shutdown_ready.get_result() == so_gpu::mt_processing ?
            so_manager::not_ready : so_manager::ok ;
    }

    {
        so_thread::lock_guard_t lk( _mtx ) ;
        _shutdown_called = true ;
        _shutdown_ready.reset( so_gpu::mt_processing ) ;
    }

    _gpx_ptr->execute( [=]( so_gpx::so_driver::driver_async_ptr_t drv )
    {
        _i2d_res_mgr.call_for_each( [&]( i2d_store_item_ptr_t iptr )
        {
            drv->release_image( iptr->obj_ptr ) ;
        } ) ;

        // @todo release image_3d

        // @todo a flush should be enough!
        // because all we need to know is that the request has been 
        // flushed to the rendering system
        drv->flush() ;
        drv->finish( &_shutdown_ready ) ;
    } ) ;

    return so_manager::not_ready ;
}

//**********************************************************************************************
so_manager::result image_manager::alloc_and_upload_image_2d( i2d_handle_ref_t hnd )
{
    auto * img_ptr = hnd->obj_ptr ;

    _gpx_ptr->execute( [=]( so_gpx::so_driver::driver_async_ptr_t drv_ptr )
    {
        drv_ptr->alloc_image_memory( img_ptr,
            so_gpu::so_driver::image_alloc_info( true ) ) ;
    } )  ;

    return so_manager::ok ;
}