//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "technique.h"

#include "../plug/iplug_factory.h"

#include <snakeoil/gpu/api/iapi.h>

#include <snakeoil/thread/global.h>
#include <snakeoil/thread/task/tasks.h>
#include <snakeoil/thread/scheduler.h>

#include <snakeoil/log/global.h>

using namespace so_gpx ;

//***************************************************************
technique::technique( so_gpx::iplug_factory_ptr_t fptr ) : _plug_fac_ptr(fptr)
{
    so_log::global::error_and_exit( so_core::is_nullptr(fptr), 
        "[technique::technique] : factory must not be nullptr" ) ;
}

//***************************************************************
technique::technique( this_rref_t rhv )
{
    so_move_member_ptr( _plug_fac_ptr, rhv ) ;
    _plugs = std::move( rhv._plugs) ;
}

//***************************************************************
technique::~technique( void_t )
{
    for( auto * p : _plugs )
    {
        if( so_core::is_nullptr( p ) ) continue ;

        _plug_fac_ptr->destroy_plug( p->plug_ptr ) ;
        so_gpx::memory::dealloc( p ) ;
    }
    _plugs.clear() ;

    if( so_core::is_not_nullptr( _plug_fac_ptr ) )
    {
        _plug_fac_ptr->destroy() ;
    }
}

//***************************************************************
technique::this_ptr_t technique::create( this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_gpx::memory::alloc( std::move( rhv ), p ) ;
}

//***************************************************************
void_t technique::destroy( this_ptr_t ptr )
{
    so_gpx::memory::dealloc( ptr ) ;
}

//***************************************************************
so_gpx::technique_transition_result technique::part_00( technique_schedule_goal goal,
    so_gpx::window_id_t wid, so_gpu::iapi_ptr_t api_ptr, 
    so_gpx::iglobal_service_ptr_t service_ptr, so_gpx::ilocal_service_ptr_t wservice_ptr, 
    so_gpu::gpu_manager_ptr_t gpu_mgr_ptr )
{
    plug_data_ptr_t pd ;

    {
        bool_t is_new = true ;

        {
            so_thread::lock_guard_t lk( _mtx_plugs ) ;
            is_new = so_core::is_not( wid < _plugs.size() && _plugs[ wid ] != nullptr ) ;
        }
        
        if( is_new )
        {
            plug_data ipd ;
            ipd.api_ptr = api_ptr ;
            ipd.wid = wid ;

            ipd.rs = technique_rest_state::offline ;
            ipd.ts = technique_transition_state::none ;
            ipd.sg = goal ;

            ipd.plug_ptr = _plug_fac_ptr->create_plug( api_ptr->get_type(), api_ptr ) ;

            if( so_core::is_nullptr( ipd.plug_ptr ) )
            {
                so_log::global::error( "[technique::part_00] : unable to create plug for api_type " +
                    so_gpu::to_string( api_ptr->get_type() ) + " on wid " + std::to_string( wid ) ) ;
                return so_gpx::technique_transition_result::failed ;
            }

            // inform the plug about the creation
            {
                so_gpx::iplug_t::create_info_t ci ;
                ci.gs_ptr = service_ptr ;
                ci.ls_ptr = wservice_ptr ;
                ipd.plug_ptr->on_create( ci ) ;
            }

            {
                so_thread::lock_guard_t lk( _mtx_plugs ) ;
                if( _plugs.size() <= wid || _plugs[ wid ] == nullptr )
                {
                    _plugs.resize( wid + 1, nullptr ) ;
                    _plugs[ wid ] = so_gpx::memory::alloc( std::move( ipd ),
                        "[technique::part_00] : plug_data" ) ;
                }
                else
                {
                    ipd.plug_ptr->destroy() ;
                }
            }
        }

        {
            so_thread::lock_guard_t lk( _mtx_plugs ) ;
            pd = _plugs[ wid ] ;
        }

        if( so_core::is_nullptr( pd ) )
        {
            return so_gpx::technique_transition_result::failed ;
        }
    }

    

    // do not do anything if the technique is 
    // other then none transition
    // => we can not break the transition state
    if( pd->ts != so_gpx::technique_transition_state::none )
    {
        return so_gpx::technique_transition_result::in_transition ;
    }

    switch( pd->rs )
    {
    case so_gpx::technique_rest_state::online:
        
        if( goal == so_gpx::technique_schedule_goal::for_shutdown )
        {
            pd->ts = so_gpx::technique_transition_state::releasing ;
            
            pd->plug_ptr->on_release() ;

            pd->rs = so_gpx::technique_rest_state::released ;
            pd->ts = so_gpx::technique_transition_state::none ;
        }
        else if( goal == so_gpx::technique_schedule_goal::for_exec )
        {
            {
                so_gpx::iplug_t::update_info_t ui ;
                pd->plug_ptr->on_update(ui) ;
            }

            pd->ts = so_gpx::technique_transition_state::executing ;
            // on_execute will be called in part_01 i.e. after the sync point
        }
        else if( goal == so_gpx::technique_schedule_goal::for_reload )
        {
            pd->ts = so_gpx::technique_transition_state::reloading ;

            so_thread::global::task_scheduler()->async_now( so_thread::void_funk_task_t::create( [=] ( void_t )
            {
                iplug_t::load_info_t li ;
                li.reload = true ;
                auto const res = pd->plug_ptr->on_load( li ) ;
                if( res != so_gpx::plug_result::ok )
                {
                    // if the operation fails, just go back to the previous state.
                    pd->rs = so_gpx::technique_rest_state::online ;
                    pd->ts = so_gpx::technique_transition_state::none ;

                    so_log::global::warning( "[technique::part_00] : technique reload failed for wid : " +
                        std::to_string( wid ) + "in rest state online" ) ;

                    return ;
                }

                pd->rs = so_gpx::technique_rest_state::reloaded ;
                pd->ts = so_gpx::technique_transition_state::none ;

            }, "[technique::part_00] : loading task" ) ) ;
        }

        break ;

    case so_gpx::technique_rest_state::offline:

        if( goal == so_gpx::technique_schedule_goal::for_online ||
            goal == so_gpx::technique_schedule_goal::for_exec )
        {
            pd->ts = so_gpx::technique_transition_state::loading ;

            so_thread::global::task_scheduler()->async_now( so_thread::void_funk_task_t::create( [=]( void_t )
            {
                iplug_t::load_info_t li ;
                auto const res = pd->plug_ptr->on_load( li ) ;
                if( res != so_gpx::plug_result::ok )
                {
                    pd->rs = so_gpx::technique_rest_state::offline ;
                    pd->ts = so_gpx::technique_transition_state::none ;

                    so_log::global::error( "[technique::part_00] : technique load failed for wid : " +
                        std::to_string( wid ) + "in rest state offline" ) ;

                    return ;
                }

                pd->rs = so_gpx::technique_rest_state::loaded ;
                pd->ts = so_gpx::technique_transition_state::none ;

            }, "[technique::part_00] : loading task" ) ) ;
        }
        break ;

    case so_gpx::technique_rest_state::loaded:

        if( goal == so_gpx::technique_schedule_goal::for_shutdown )
        {
            pd->ts = so_gpx::technique_transition_state::unloading ;

            pd->plug_ptr->on_unload() ;

            pd->rs = so_gpx::technique_rest_state::unloaded ;
            pd->ts = so_gpx::technique_transition_state::none ;
        }        
        else if( goal == so_gpx::technique_schedule_goal::for_online ||
            goal == so_gpx::technique_schedule_goal::for_exec )
        {
            pd->ts = so_gpx::technique_transition_state::initializing ;

            so_gpx::iplug_t::init_info_t ii ;
            ii.mgr = gpu_mgr_ptr ;

            pd->plug_ptr->on_initialize( ii ) ;

            pd->rs = so_gpx::technique_rest_state::initialized ;
            pd->ts = so_gpx::technique_transition_state::none ;
        }

        break ;

    case so_gpx::technique_rest_state::reloaded:

        if( goal == so_gpx::technique_schedule_goal::for_shutdown )
        {
            pd->ts = so_gpx::technique_transition_state::releasing ;

            pd->plug_ptr->on_release() ;

            pd->rs = so_gpx::technique_rest_state::released ;
            pd->ts = so_gpx::technique_transition_state::none ;
        }        
        else if( goal == so_gpx::technique_schedule_goal::for_online ||
            goal == so_gpx::technique_schedule_goal::for_exec )
        {
            pd->ts = so_gpx::technique_transition_state::initializing ;

            so_gpx::iplug_t::init_info_t ii ;
            ii.reinit = true ;
            ii.mgr = gpu_mgr_ptr ;

            pd->plug_ptr->on_initialize( ii ) ;

            pd->rs = so_gpx::technique_rest_state::initialized ;
            pd->ts = so_gpx::technique_transition_state::none ;
        }

        break ;

    case so_gpx::technique_rest_state::initialized:

        if( goal == so_gpx::technique_schedule_goal::for_shutdown )
        {
            pd->ts = so_gpx::technique_transition_state::releasing ;

            pd->plug_ptr->on_release() ;

            pd->rs = so_gpx::technique_rest_state::released ;
            pd->ts = so_gpx::technique_transition_state::none ;
        }
        else if( goal == so_gpx::technique_schedule_goal::for_online ||
            goal == so_gpx::technique_schedule_goal::for_exec || 
            goal == so_gpx::technique_schedule_goal::for_reload )
        {
            pd->rs = so_gpx::technique_rest_state::online ;
            pd->ts = so_gpx::technique_transition_state::none ;
        }

        break ;

    case so_gpx::technique_rest_state::unloaded:

        if( goal == so_gpx::technique_schedule_goal::for_shutdown )
        {
            pd->rs = so_gpx::technique_rest_state::offline ;
            pd->ts = so_gpx::technique_transition_state::none ;
        }
        else if( goal == so_gpx::technique_schedule_goal::for_online ||
            goal == so_gpx::technique_schedule_goal::for_exec )
        {
            pd->ts = so_gpx::technique_transition_state::loading ;

            so_thread::global::task_scheduler()->async_now( so_thread::void_funk_task_t::create( [=]( void_t )
            {
                iplug_t::load_info_t li ;
                auto const res = pd->plug_ptr->on_load( li ) ;
                if( res != so_gpx::plug_result::ok )
                {
                    pd->rs = so_gpx::technique_rest_state::offline ;
                    pd->ts = so_gpx::technique_transition_state::none ;

                    so_log::global::error( "[technique::part_00] : technique load failed for wid : " +
                        std::to_string( wid ) + "in rest unloaded" ) ;

                    return ;
                }

                pd->rs = so_gpx::technique_rest_state::loaded ;
                pd->ts = so_gpx::technique_transition_state::none ;

            }, "[technique::part_00] : loading task" ) ) ;
        }

        break ;

    case so_gpx::technique_rest_state::released:

        if( goal == so_gpx::technique_schedule_goal::for_shutdown )
        {
            pd->ts = so_gpx::technique_transition_state::unloading ;

            pd->plug_ptr->on_unload() ;

            pd->rs = so_gpx::technique_rest_state::unloaded ;
            pd->ts = so_gpx::technique_transition_state::none ;
        }
        else if( goal == so_gpx::technique_schedule_goal::for_online ||
            goal == so_gpx::technique_schedule_goal::for_exec )
        {
            pd->ts = so_gpx::technique_transition_state::initializing ;

            so_gpx::iplug_t::init_info_t ii ;
            ii.mgr = gpu_mgr_ptr ;
            pd->plug_ptr->on_initialize( ii ) ;

            pd->rs = so_gpx::technique_rest_state::initialized ;
            pd->ts = so_gpx::technique_transition_state::none ;
        }

        break ;
    }

    {
        bool_t const res1 =
            ( goal == so_gpx::technique_schedule_goal::for_online ) &&
            ( pd->rs == so_gpx::technique_rest_state::online );

        bool_t const res2 =
            ( goal == so_gpx::technique_schedule_goal::for_exec ) &&
            ( pd->rs == so_gpx::technique_rest_state::online ) && 
            ( pd->ts == so_gpx::technique_transition_state::executing ) ;

        bool_t const res3 =
            ( goal == so_gpx::technique_schedule_goal::for_shutdown ) &&
            ( pd->rs == so_gpx::technique_rest_state::offline );

        if( res1 || res2 || res3 ) 
            return so_gpx::technique_transition_result::goal_reached ;
    }    

    if( pd->ts == so_gpx::technique_transition_state::loading ||
        pd->ts == so_gpx::technique_transition_state::unloading )
        return so_gpx::technique_transition_result::in_transition ;

    return so_gpx::technique_transition_result::state_changed ;
}

//***************************************************************
so_gpx::technique_transition_result technique::check_part_00_finished( 
    technique_schedule_goal goal, so_gpx::window_id_t wid )
{
    plug_data_ptr_t pd = _plugs[ wid ] ;
    if( so_core::is_nullptr( pd ) ) return so_gpx::technique_transition_result::failed ;

    {
        bool_t const res1 =
            ( goal == so_gpx::technique_schedule_goal::for_online ) &&
            ( pd->rs == so_gpx::technique_rest_state::online );

        bool_t const res2 =
            ( goal == so_gpx::technique_schedule_goal::for_exec ) &&
            ( pd->rs == so_gpx::technique_rest_state::online ) &&
            ( pd->ts == so_gpx::technique_transition_state::executing ) ;

        bool_t const res3 =
            ( goal == so_gpx::technique_schedule_goal::for_shutdown ) &&
            ( pd->rs == so_gpx::technique_rest_state::offline );

        if( res1 || res2 || res3 )
            return so_gpx::technique_transition_result::goal_reached ;
    }

    if( pd->ts == so_gpx::technique_transition_state::none )
        return so_gpx::technique_transition_result::state_changed ;

    return so_gpx::technique_transition_result::in_transition ;
}

//***************************************************************
void_t technique::part_01_render( so_gpx::window_id_t wid, 
    so_gpx::schedule_instance_cref_t si, so_gpu::iapi_ptr_t api_ptr )
{
    plug_data_ptr_t pd = _plugs[ wid ] ;
    if( so_core::is_nullptr( pd ) ) return ;
    

    if( pd->ts != so_gpx::technique_transition_state::executing ) return ;

    {
        so_gpx::iplug_t::execute_info ei ;
        ei.rnd_id = si.render_id ;

        pd->plug_ptr->on_execute( ei ) ;
    }
}

//***************************************************************
void_t technique::part_01_render_end( so_gpx::window_id_t wid, so_gpx::schedule_instance_cref_t )
{
    plug_data_ptr_t pd = _plugs[ wid ] ;
    if( so_core::is_nullptr( pd ) ) return ;


    if( pd->ts != so_gpx::technique_transition_state::executing ) return ;
    pd->ts = so_gpx::technique_transition_state::none ;
}

//***************************************************************
void_t technique::part_01_render_pseudo( so_gpx::window_id_t wid )
{
    plug_data_ptr_t pd = _plugs[ wid ] ;
    if( so_core::is_nullptr( pd ) ) return ;

    if( pd->ts != so_gpx::technique_transition_state::executing ) return ;

    pd->ts = so_gpx::technique_transition_state::none ;
}

//***************************************************************
so_gpx::result technique::part_01_update( so_gpx::window_id_t wid )
{
    plug_data_ptr_t pd = _plugs[ wid ] ;

    if( so_core::is_nullptr(pd) )
    {
        return so_gpx::failed ;
    }

    // required for instanced rendering
    // i.e. multiple render calls of the same technique
    if( pd->updated ) return so_gpx::ok ;

    if( pd->rs != so_gpx::technique_rest_state::online ) return so_gpx::ok ;

    if( pd->ts != so_gpx::technique_transition_state::none &&
        pd->ts != so_gpx::technique_transition_state::executing ) return so_gpx::ok ;

    pd->updated = true ;

    return so_gpx::ok ;
}

//***************************************************************
so_gpx::result technique::part_01_update_end( so_gpx::window_id_t wid )
{
    plug_data_ptr_t pd = _plugs[ wid ] ;

    if( so_core::is_nullptr( pd ) )
    {
        return so_gpx::failed ;
    }

    pd->updated = false ;

    return so_gpx::ok ;
}

//***************************************************************
bool_t technique::wait_for_init( so_gpx::window_id_t wid )
{
    if( _plugs.size() >= wid )
        return false ;

    auto * p = _plugs[ wid ] ;

    so_thread::lock_t lk( p->mtx_init ) ;
    while( so_core::is_not( p->init_flag ) ) p->cv_init.wait( lk ) ;

    return true ;
}