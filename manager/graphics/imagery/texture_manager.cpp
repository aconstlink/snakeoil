//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "texture_manager.h"

#include <snakeoil/gpu/texture/texture_2d.h>
#include <snakeoil/gpu/texture/texture_3d.h>

#include <snakeoil/gpx/system/system.h>
#include <snakeoil/gpx/driver/driver_async.h>

using namespace so_manager ;
using namespace so_manager::so_graphics ;

//**********************************************************************************************
texture_manager::texture_manager( so_gpx::iuser_system_ptr_t ptr ) : _gpx_ptr(ptr)
{
    so_log::log::error_and_exit( so_core::is_nullptr(ptr),
        "[so_manager::texture_manager::texture_manager] : gpx ptr must != nullptr" ) ;
}

//**********************************************************************************************
texture_manager::texture_manager( this_rref_t rhv ) 
{
    _t2d_res_mgr = std::move( rhv._t2d_res_mgr ) ;
    _t3d_res_mgr = std::move( rhv._t3d_res_mgr ) ;
    _gpx_ptr = rhv._gpx_ptr ;
    _shutdown_called = rhv._shutdown_called ;
    _shutdown_ready = std::move( rhv._shutdown_ready ) ;
}

//**********************************************************************************************
texture_manager::~texture_manager( void_t ) 
{
    _t2d_res_mgr.call_for_each( [&]( t2d_store_item_ptr_t item_ptr )
    {
        item_ptr->obj_ptr->destroy() ;
        so_manager::memory::dealloc( item_ptr ) ;
    } ) ;

    /// @todo 3d texture destruction is incomplete
    _t3d_res_mgr.call_for_each( [&]( t3d_store_item_ptr_t item_ptr )
    {} ) ;
}

//**********************************************************************************************
texture_manager::this_ptr_t texture_manager::create( this_rref_t rhv, so_memory::purpose_cref_t purpose) 
{
    return so_manager::memory::alloc( std::move(rhv), purpose ) ;
}

//**********************************************************************************************
void_t texture_manager::destroy( this_ptr_t ptr ) 
{
    so_manager::memory::dealloc(ptr) ;
}

//**********************************************************************************************
bool_t texture_manager::acquire( so_manager::key_cref_t key_in,
    so_resource::purpose_cref_t p, t2d_handle_out_t hnd_out ) 
{
    return _t2d_res_mgr.acquire( key_in, p, hnd_out ) ;
}

//**********************************************************************************************
bool_t texture_manager::acquire( so_manager::key_cref_t key_in,
    so_resource::purpose_cref_t p, t3d_handle_out_t hnd_out )
{
    return _t3d_res_mgr.acquire( key_in, p, hnd_out ) ;
}

//**********************************************************************************************
bool_t texture_manager::release( t2d_handle_rref_t hnd )
{
    return _t2d_res_mgr.release( std::move( hnd ) ) ;
}

//**********************************************************************************************
bool_t texture_manager::release( t3d_handle_rref_t hnd )
{
    return _t3d_res_mgr.release( std::move( hnd ) ) ;
}

//**********************************************************************************************
so_manager::result texture_manager::insert( so_manager::key_in_t key_in, t2d_manage_params_in_t mp )
{
    t2d_store_item_t si ;
    si.obj_ptr = mp.obj_ptr ;
    
    t2d_store_item_ptr_t si_ptr = so_manager::memory::alloc( std::move( si ),
        "[so_manager::texture_manager::insert] : t2d_store_item_t : " + key_in ) ;

    auto const res = _t2d_res_mgr.insert( key_in, si_ptr ) ;
    if( so_log::log::error( so_resource::no_success( res ),
        "[so_manager::texture_manager::insert] : insert" ) )
    {
        so_manager::memory::dealloc( si_ptr ) ;
        return so_manager::key_already_in_use;
    }

    _gpx_ptr->execute( [=]( so_gpx::so_driver::driver_async_ptr_t drv )
    {
        drv->create_texture( mp.obj_ptr ) ;
    } ) ;

    return so_manager::ok ;
}

//**********************************************************************************************
so_manager::result texture_manager::insert( so_manager::key_in_t key_in, t3d_manage_params_in_t mp )
{
    t3d_store_item_t si ;
    si.obj_ptr = mp.obj_ptr ;

    t3d_store_item_ptr_t si_ptr = so_manager::memory::alloc( std::move( si ),
        "[so_manager::texture_manager::insert] : t3d_store_item_t : " + key_in ) ;

    auto const res = _t3d_res_mgr.insert( key_in, si_ptr ) ;
    if( so_log::log::error( so_resource::no_success( res ),
        "[so_manager::texture_manager::insert] : insert" ) )
    {
        so_manager::memory::dealloc( si_ptr ) ;
        return so_manager::key_already_in_use;
    }

    _gpx_ptr->execute( [=]( so_gpx::so_driver::driver_async_ptr_t drv )
    {
        //drv->create_texture( mp.obj_ptr ) ;
    } ) ;

    return so_manager::ok ;
}

//**********************************************************************************************
void_t texture_manager::destroy( void_t ) 
{
    this_t::destroy( this ) ;
}

//**********************************************************************************************
so_manager::result texture_manager::shutdown( void_t ) 
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
        _t2d_res_mgr.call_for_each( [&]( t2d_store_item_ptr_t iptr )
        {
            drv->release_texture( iptr->obj_ptr ) ;
        } ) ;
        
        // for 3d
        /*for( auto item : _image_3ds )
        {
            drv->release_image( item.second.obj_ptr ) ;
        }*/

        // @todo a flush should be enough!
        // because all we need to know is that the request has been 
        // flushed to the rendering system
        drv->flush() ;
        drv->finish( &_shutdown_ready ) ;
    } ) ;

    return so_manager::not_ready ;
}