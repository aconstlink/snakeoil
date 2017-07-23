//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "audio_technique.h"
#include "../plug/iplug_factory.h"
#include "../plug/iplug.h"

#include <snakeoil/audio/api/iapi.h>

#include <snakeoil/thread/task/tasks.h>
#include <snakeoil/thread/scheduler.h>

#include <snakeoil/log/log.h>

using namespace so_audiox ;

//*************************************************************************************
audio_technique::audio_technique( so_audiox::iplug_factory_ptr_t fptr ) 
{
    so_log::log::error_and_exit( so_core::is_nullptr(fptr), 
        "[audio_technique::audio_technique] : nullptr is invalid" ) ;

    _fac_ptr = fptr ;
}

//*************************************************************************************
audio_technique::audio_technique( this_rref_t rhv )
{
    so_move_member_ptr( _fac_ptr, rhv ) ;
}

//*************************************************************************************
audio_technique::~audio_technique( void_t )
{
    if( _datas.size() > 0 )
    {
        _fac_ptr->destroy_plug( _datas[0]->plug_ptr ) ;
        so_audiox::memory::dealloc( _datas[0] ) ;
    }
}

//*************************************************************************************
audio_technique::this_ptr_t audio_technique::create( this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_audiox::memory::alloc( std::move(rhv), p ) ;
}

//*************************************************************************************
void_t audio_technique::destroy( this_ptr_t ptr )
{
    so_audiox::memory::dealloc( ptr ) ;
}

//*************************************************************************************
so_audiox::technique_state audio_technique::part_00( technique_state target, so_audiox::api_id_t aid, so_audio::iapi_ptr_t aptr,
    so_audiox::iglobal_service_ptr_t service_ptr, so_audiox::ilocal_service_ptr_t wservice_ptr )
{
    if( _datas.size() == 0 )
    {
        plug_data pd ;
        pd.api_ptr = aptr ;
        pd.aid = aid ;
        pd.ts = technique_state::raw ;
        pd.plug_ptr = _fac_ptr->create_plug( aptr->get_type(), aptr,
            service_ptr, wservice_ptr ) ;

        if( so_core::is_nullptr( pd.plug_ptr ) )
        {
            return technique_state::failed ;
        }

        {
            so_thread::lock_guard_t lk( _mtx_plugs ) ;
            if( _datas.size() == 0 )
            {
                _datas.push_back( so_audiox::memory::alloc( std::move( pd ),
                    "[audio_technique::part_00] : plug_data" ) ) ;
            }
            else
            {
                pd.plug_ptr->destroy() ;
            }
        }
    }

    if( _datas[0]->aid != aid ) return technique_state::failed ;
    
    //
    // state transitions
    //

    plug_data_ptr_t pd = _datas[0] ;
    so_log::log::error_and_exit( so_core::is_nullptr( pd ), 
        "[audio_technique::part_00] : plug_data must not be nullptr" ) ;

    // 1. handle update state
    {
        switch( pd->ts_update )
        {
        case technique_state::transfer:
            pd->ts = technique_state::transfer ;
            break ;
        }
        pd->ts_update = technique_state::ready ;
    }

    // 2. handle target state
    {
        switch( target )
        {
        case technique_state::release:
            if( pd->ts == technique_state::ready )
            {
                pd->ts = technique_state::release ;
            }
            else if( pd->ts == technique_state::raw )
            {
                return technique_state::raw ;
            }
            break ;
            
        default:
            break ;
        }
    }


    switch( pd->ts )
    {
    case technique_state::raw:

        pd->ts = technique_state::loading ;

        so_thread::scheduler::ts()->async_now( so_thread::void_funk_task_t::create( [=]( void_t )
        {
            auto const res = pd->plug_ptr->on_load() ;
            if( res != so_audiox::plug_result::ok )
            {
                pd->ts = technique_state::failed ;
                return ;
            }

            pd->ts = technique_state::init ;

        }, "[technique::part_00] : loading task" ) ) ;

        break ;

    case technique_state::loading:
    // loading, do nothing
    break ;

    case technique_state::release:
    {
        {
            so_thread::lock_guard_t lk( pd->mtx_init ) ;
            pd->init_flag = false ;
        }

        auto const res = pd->plug_ptr->on_release() ;
        if( res != so_audiox::plug_result::ok )
        {
            pd->ts = technique_state::failed ;
            break ;
        }
        pd->ts = technique_state::raw ;
        break ;
    }

    case technique_state::init:
    {
        auto const res = pd->plug_ptr->on_initialize() ;
        if( res != so_audiox::plug_result::ok )
        {
            pd->ts = technique_state::failed ;
            break ;
        }

        pd->ts = technique_state::transfer ;
        // no break is intentional here

        {
            so_thread::lock_guard_t lk( pd->mtx_init ) ;
            pd->init_flag = true ;
        }
        pd->cv_init.notify_all() ;
    }

    case technique_state::transfer:
    {
        auto const res = pd->plug_ptr->on_transfer() ;
        if( res != so_audiox::plug_result::ok )
        {
            pd->ts = technique_state::failed ;
            break ;
        }
        pd->ts = technique_state::ready ;
    }
    break ;
    default:
    break ;
    }

    if( pd->ts == technique_state::ready )
    {
        pd->ts = target == technique_state::execute ? target : pd->ts ;
    }

    return pd->ts ;
}

//*************************************************************************************
so_audiox::technique_state audio_technique::part_01_render( so_audiox::iplug::execute_info_cref_t ei )
{
    plug_data_ptr_t pd = _datas[0] ;

    switch( pd->ts )
    {
        case technique_state::execute:
        {
            auto const res = pd->plug_ptr->on_execute( ei ) ;
            pd->ts = technique_state::execute_done ;
        }
        break ;

        default:
        break ;
    }

    return pd->ts ;
}

//*************************************************************************************
so_audiox::technique_state audio_technique::part_01_render_end( void_t ) 
{
    plug_data_ptr_t pd = _datas[0] ;

    switch( pd->ts )
    {
    case technique_state::execute_done:
        pd->ts = technique_state::ready ;
        break ;

    default:
        break ;
    }

    return pd->ts ;
}

//*************************************************************************************
so_audiox::result audio_technique::part_01_update( void_t )
{
    plug_data_ptr_t pd = _datas[0] ;

    if( so_core::is_nullptr( pd ) )
    {
        return so_audiox::failed ;
    }

    switch( pd->ts )
    {
    case technique_state::ready:
    case technique_state::execute:
    case technique_state::executing:
    {
        if( pd->ts_update == so_audiox::technique_state::ready )
        {
            pd->ts_update = so_audiox::technique_state::updated ;

            auto const res = pd->plug_ptr->on_update() ;
            if( res == so_audiox::plug_result::need_transfer )
            {
                pd->ts_update = so_audiox::technique_state::transfer ;
            }
        }
        break;
    }

    default:
        break ;
    }

    return so_audiox::ok ;
}

//*************************************************************************************
so_audiox::api_id_t audio_technique::get_used_api( void_t ) const 
{
    if( _datas.size() == 0 ) return so_audiox::api_id_t(-1) ;
    return _datas[0]->aid ;
}

//***************************************************************
bool_t audio_technique::wait_for_init( void_t )
{
    if( _datas.size() == 0 )
        return false ;

    auto * p = _datas[0] ;

    so_thread::lock_t lk( p->mtx_init ) ;
    while( so_core::is_not( p->init_flag ) ) p->cv_init.wait( lk ) ;

    return true ;
}