//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "geometry_manager.h"
#include "buffer_creator.h"

#include <snakeoil/log/log.h>

#include <snakeoil/gpx/system/system.h>
#include <snakeoil/gpx/driver/driver_async.h>
#include <snakeoil/gpx/event/event.h>

using namespace so_manager ;
using namespace so_manager::so_graphics ;

//**********************************************************************************************
geometry_manager::this_ptr_t geometry_manager::create( this_rref_t rhv, so_memory::purpose_cref_t p ) 
{
    return so_manager::memory::alloc( std::move(rhv), p ) ;
}

//**********************************************************************************************
void_t geometry_manager::destroy( this_ptr_t ptr ) 
{
    so_manager::memory::dealloc( ptr ) ;
}

//**********************************************************************************************
geometry_manager::geometry_manager( so_gpx::iuser_system_ptr_t ptr ) 
{
    so_log::log::error_and_exit( so_core::is_nullptr(ptr), 
        "[so_manager::geometry_manager::geometry_manager] : is nullptr" ) ;

    _gpxs = ptr ;
}

//**********************************************************************************************
geometry_manager::geometry_manager( this_rref_t rhv )
{
    _gpxs = rhv._gpxs ;
    _shutdown_called = rhv._shutdown_called ;
    _shutdown_ready = std::move( rhv._shutdown_ready ) ;

    _counter = rhv._counter ;
    _res_mgr = std::move( rhv._res_mgr ) ;
}

//**********************************************************************************************
geometry_manager::~geometry_manager( void_t ) 
{
    _res_mgr.call_for_each( [&]( store_item_ptr_t item_ptr )
    {
        item_ptr->vb->destroy() ;
        if( so_core::is_not_nullptr(item_ptr->ib) )
            item_ptr->ib->destroy() ;

        so_gpx::event::destroy( item_ptr->gpu_memory_evt_ptr ) ;
        so_manager::memory::dealloc( item_ptr ) ;
    } ) ;
}

//**********************************************************************************************
so_manager::result geometry_manager::destroy( void_t ) 
{
    this_t::destroy( this ) ;
    return so_manager::ok ;
}

//**********************************************************************************************
so_manager::result geometry_manager::shutdown( void_t ) 
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

    _gpxs->execute( [&]( so_gpx::so_driver::driver_async_ptr_t drv )
    {
        _res_mgr.call_for_each( [&]( store_item_ptr_t iptr )
        {
            drv->release_buffer( iptr->vb ) ;
            drv->release_buffer( iptr->ib ) ;
        } ) ;
        
        // @todo a flush should be enough!
        // because all we need to know is that the request has been 
        // flushed to the rendering system
        drv->flush() ;
        drv->finish( &_shutdown_ready ) ;
    } ) ;

    return so_manager::not_ready ;
}

//**********************************************************************************************
bool_t geometry_manager::acquire( so_manager::key_cref_t key_in,
    so_resource::purpose_cref_t p, handle_out_t hnd_out )
{
    return _res_mgr.acquire( key_in, p, hnd_out ) ;
}

//**********************************************************************************************
bool_t geometry_manager::release( handle_rref_t hnd )
{
    return _res_mgr.release( std::move( hnd ) ) ;
}

//**********************************************************************************************
so_manager::result geometry_manager::insert( 
    so_manager::key_cref_t key_in, manage_params_cref_t mp )
{
    store_item si ;
    si.vb = mp.vb ;
    si.ib = mp.ib ;
    si.pt = mp.pt ;
    si.gpu_memory_evt_ptr = so_gpx::event::create(
        "[so_manager::geometry_manager::insert]" ) ;

    store_item_ptr_t si_ptr = so_manager::memory::alloc( std::move( si ),
        "[so_manager::geometry_manager::insert] : store_item" ) ;

    auto const res = _res_mgr.insert( key_in, si_ptr ) ;
    if( so_log::log::error( so_resource::no_success( res ),
        "[so_manager::geometry_manager::insert] : insert" ) )
    {
        so_gpx::event_t::destroy( si.gpu_memory_evt_ptr ) ;        
        so_manager::memory::dealloc( si_ptr ) ;
        return so_manager::key_already_in_use;
    }

    return so_manager::ok ;
}

//**********************************************************************************************
so_manager::result geometry_manager::insert_and_create( so_manager::key_cref_t key_in, 
    manage_params_cref_t mp )
{
    {
        auto const res = this_t::insert( key_in, mp ) ;
        if (so_manager::no_success( res )) return res ;
    }

    this_t::handle_t hnd ;
    auto const res = this_t::acquire( key_in, 
        "[geometry_manager::insert_and_create] : create buffers", hnd ) ;
    
    if( so_core::is_not(res) )
    {
        so_log::log::error( "[geometry_manager::insert_and_create] : "
            "can not acquire handle for " + key_in ) ;

        return so_manager::failed ;
    }

    this_t::store_item_t d ;
    d.vb = hnd->vb ;
    d.ib = hnd->ib ;
    d.pt = hnd->pt ;
    d.gpu_memory_evt_ptr = hnd->gpu_memory_evt_ptr ;

    auto const rres = d.gpu_memory_evt_ptr->reset( so_gpu::invalid ) ;
    if( so_core::is_not(rres) )
    {
        so_log::log::error( "[geometry_manager::insert_and_create] : " 
            "can not reset gpx event for gpu buffer creation for : " + key_in ) ;
    }

    _gpxs->execute( [d]( so_gpx::so_driver::driver_async_ptr_t drv_ptr )
    {
        drv_ptr->create_buffer( d.vb ) ;
        drv_ptr->create_buffer( d.ib ) ;
    } ) ;

    return so_manager::ok ;
}

//**********************************************************************************************
so_manager::result geometry_manager::construct_from( so_geo::flat_tri_mesh_cref_t mesh_in, 
                so_manager::key_cref_t key_in ) 
{
    this_t::handle_t hnd ;

    // STEP 1: Reserve the manager entry
    {
        auto const res = _res_mgr.reserve_and_acquire( key_in, 
            "[so_manager::geometry_manager::construct_from]", hnd ) ;
        
        if( res != so_resource::reserved )
            return so_manager::key_already_in_use ;
    }

    store_item to_be_stored ;

    // STEP 2: Create the rendering buffers
    {
        auto bc = so_manager::so_graphics::buffer_creator( mesh_in ) ;
        to_be_stored.pt = bc.get_primitive_type() ;
        to_be_stored.vb = bc.move_vertex_buffer() ;
        to_be_stored.ib = bc.move_index_buffer() ;
    }

    // STEP 3: Validity check
    {
        if( to_be_stored.pt == so_gpu::primitive_type::undefined )
        {
            if( to_be_stored.vb != nullptr ) to_be_stored.vb->destroy() ;
            if( to_be_stored.ib != nullptr ) to_be_stored.ib->destroy() ;

            _res_mgr.release( std::move(hnd), [=]( store_item_ptr_t ptr )
            {
                so_manager::memory::dealloc( ptr ) ;
                return true ;
            } ) ;
            return so_manager::failed ;
        }
    }

    // STEP 4: create rendering objects 
    {
        _gpxs->execute( [=]( so_gpx::so_driver::driver_async_ptr_t drv_ptr )
        {
            drv_ptr->create_buffer( to_be_stored.vb ) ;
            if( to_be_stored.ib != nullptr )
                drv_ptr->create_buffer( to_be_stored.ib ) ;
        } ) ;

        to_be_stored.gpu_memory_evt_ptr = so_gpx::event::create(
            "[so_manager::geometry_manager::construct_from]") ;
    }

    // STEP 5: store the geometry
    {
        this_t::store_item_ptr_t new_ptr = so_manager::memory::alloc( std::move(to_be_stored), 
            "[so_manager::geometry_manager::construct_from] : store_item" ) ;

        this_t::store_item_ptr_t old_ptr = nullptr ;
        auto const res = _res_mgr.exchange_self( hnd, new_ptr, old_ptr ) ;

        so_log::log::error_and_exit( so_core::is_not(res), 
            "[so_manager::geometry_manager::construct_from] : exchange must succeed" ) ;

        so_log::log::error_and_exit( so_core::is_not_nullptr(old_ptr), 
            "[so_manager::geometry_manager::construct_from] : old_ptr must be nullptr" ) ;
    }

    return so_manager::ok ;
}

//**********************************************************************************************
so_manager::result geometry_manager::construct_from( so_geo::flat_line_mesh_cref_t wire_in, 
                so_manager::key_cref_t key_in ) 
{
    this_t::handle_t hnd ;

    // STEP 1: Reserve the manager entry
    {
        auto const res = _res_mgr.reserve_and_acquire( key_in, 
            "[so_manager::geometry_manager::construct_from]", hnd ) ;

        if( res != so_resource::reserved )
            return so_manager::key_already_in_use ;
    }

    store_item to_be_stored ;

    // STEP 2: Create the rendering buffers
    {
        auto bc = so_manager::so_graphics::buffer_creator( wire_in ) ;
        to_be_stored.pt = bc.get_primitive_type() ;
        to_be_stored.vb = bc.move_vertex_buffer() ;
        to_be_stored.ib = bc.move_index_buffer() ;
    }

    // STEP 3: Validity check
    {
        if( to_be_stored.pt == so_gpu::primitive_type::undefined )
        {
            if( to_be_stored.vb != nullptr ) to_be_stored.vb->destroy() ;
            if( to_be_stored.ib != nullptr ) to_be_stored.ib->destroy() ;

            _res_mgr.release( std::move(hnd), [=]( store_item_ptr_t ptr )
            {
                so_manager::memory::dealloc( ptr ) ;
                return true ;
            } ) ;
            return so_manager::failed ;
        }
    }

    // STEP 4: create rendering objects 
    {
        _gpxs->execute( [=]( so_gpx::so_driver::driver_async_ptr_t drv_ptr )
        {
            drv_ptr->create_buffer( to_be_stored.vb ) ;
            if( to_be_stored.ib != nullptr )
                drv_ptr->create_buffer( to_be_stored.ib ) ;
        } ) ;

        to_be_stored.gpu_memory_evt_ptr = so_gpx::event::create(
            "[so_manager::geometry_manager::construct_from]") ;
    }
    
    // STEP 5: store the geometry
    {
        this_t::store_item_ptr_t new_ptr = so_manager::memory::alloc( std::move(to_be_stored), 
            "[so_manager::geometry_manager::construct_from] : store_item" ) ;

        this_t::store_item_ptr_t old_ptr = nullptr ;
        auto const res = _res_mgr.exchange_self( hnd, new_ptr, old_ptr ) ;

        so_log::log::error_and_exit( so_core::is_not(res), 
            "[so_manager::geometry_manager::construct_from] : exchange must succeed" ) ;

        so_log::log::error_and_exit( so_core::is_not_nullptr(old_ptr), 
            "[so_manager::geometry_manager::construct_from] : old_ptr must be nullptr" ) ;
    }

    return so_manager::ok ;
}

//**********************************************************************************************
so_manager::result geometry_manager::upload( so_manager::key_cref_t key_in ) 
{
    this_t::handle_t hnd ;
    if( so_core::is_not(_res_mgr.acquire( key_in, "[so_manager::geometry_manager::upload]", hnd ) ) )
    {
        so_log::log::error( "[so_manager::geometry_manager::upload] : invalid key : " + key_in ) ;
        return so_manager::invalid_argument ; 
    }

    bool_t has_gpu_memory = false ;
    {
        if( hnd->gpu_memory_evt_ptr->get_result() == so_gpu::mt_processing )
            return so_manager::ok ;

        if( hnd->gpu_memory_evt_ptr->get_result() == so_gpu::ok )
            has_gpu_memory = true ;

        hnd->gpu_memory_evt_ptr->reset( so_gpu::mt_processing ) ;
    }
     
    this_t::store_item_t d ;
    d.vb = hnd->vb ;
    d.ib = hnd->ib ;
    d.pt = hnd->pt ;
    d.gpu_memory_evt_ptr = hnd->gpu_memory_evt_ptr ;

    // @todo capture rref of hnd -> not possible with C++11
    _gpxs->execute( [this,has_gpu_memory, d]( so_gpx::so_driver::driver_async_ptr_t drv_ptr )
    {
        if( has_gpu_memory )
        {
            drv_ptr->load_buffer_memory( d.vb, so_gpu::so_driver::memory_load_info() ) ;
            drv_ptr->load_buffer_memory( d.ib, so_gpu::so_driver::memory_load_info() ) ;
        }
        else 
        {
            drv_ptr->alloc_buffer_memory( d.vb, so_gpu::so_driver::memory_alloc_info(true) ) ;
            drv_ptr->alloc_buffer_memory( d.ib, so_gpu::so_driver::memory_alloc_info(true) ) ;
        }
        
        d.gpu_memory_evt_ptr->send_signal( so_gpu::ok ) ;
        
    } ) ;

    return so_manager::ok ;
}

//**********************************************************************************************
so_manager::result geometry_manager::upload_and_wait( so_manager::key_cref_t key_in )
{
    auto const res = this_t::upload( key_in ) ;
    if( so_manager::no_success( res ) )
    {
        so_log::log::error("[so_manager::geometry_manager::upload_and_wait] : init upload failed") ;
        return res ;
    }

    this_t::handle_t hnd ;
    auto const hres = this_t::acquire( key_in, "[so_manager::geometry_manager::upload_and_wait]", hnd ) ;
    if( so_core::is_not(hres) )
    {
        so_log::log::error("[so_manager::geometry_manager::upload_and_wait] : "
            "failed to acquire handle for " + key_in ) ;
        return so_manager::failed ;
    }

    auto const gres = hnd->gpu_memory_evt_ptr->wait_for_signal() ;
    if( so_gpu::no_success( gres ) )
    {
        so_log::log::error( "[so_manager::geometry_manager::upload_and_wait] : "
            "failed to upload " + key_in ) ;

        return so_manager::failed ;
    }

    return so_manager::ok ;
}