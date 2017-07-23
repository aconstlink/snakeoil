//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "audio_system.h"

#include "../plug/iplug.h"
#include "../technique/audio_technique.h"

#include <snakeoil/audio/api/iapi.h>
#include <snakeoil/audio/api/null/null_api.h>
#include <snakeoil/audio/api/api_creator.h>

#include <snakeoil/log/log.h>

#include <algorithm>
#include <chrono>

using namespace so_audiox ;

//**********************************************************************************************
void_t audio_system::shared_object::sync_00_wait( void_t )
{    
    so_thread::lock_t lk( mtx_00 ) ;
    while( so_core::is_not( sync_00 ) && so_core::is_not( shutdown ) ) cv_00.wait( lk ) ;
    sync_00 = false ;
}

//**********************************************************************************************
void_t audio_system::shared_object::sync_00_send( void_t )
{
    {
        so_thread::lock_guard_t lk( mtx_00 ) ;
        sync_00 = true ;
    }
    cv_00.notify_all() ;
}

//**********************************************************************************************
void_t audio_system::shared_object::sync_01_wait( void_t )
{
    so_thread::lock_t lk( mtx_01 ) ;
    while( so_core::is_not( sync_01 ) && so_core::is_not( shutdown ) ) cv_01.wait( lk ) ;
    sync_01 = false ;
}

//**********************************************************************************************
void_t audio_system::shared_object::sync_01_send( void_t )
{
    {
        so_thread::lock_guard_t lk( mtx_01 ) ;
        sync_01 = true ;
    }
    cv_01.notify_all() ;
}

//**********************************************************************************************
void_t audio_system::shared_object::send_shutdown( void_t )
{
    so_thread::lock_t lk( mtx_sd ) ;
    shutdown = true ;
    sync_00_send() ;
    sync_01_send() ;
}

//**********************************************************************************************
void_t audio_system::shared_object::wait_for_shutdown( void_t )
{
    so_thread::lock_t lk( mtx_sd ) ;
    while( so_core::is_not( shutdown_ok ) ) cv_shutdown.wait( lk ) ;
}

//**********************************************************************************************
void_t audio_system::shared_object::send_shutdown_ok( void_t )
{
    {
        so_thread::lock_guard_t lk( mtx_sd ) ;
        shutdown_ok = true ;
    }
    cv_shutdown.notify_all() ;
}

//**********************************************************************************************
void_t audio_system::shared_object::flip_scheduled( void_t )
{
    so_thread::lock_guard_t lk( mtx_scheduled ) ;
    read_buffer_id = ++read_buffer_id & 1 ;
}

//**********************************************************************************************
audio_system::schedule_datas_ref_t audio_system::shared_object::read_buffer( void_t )
{
    return scheduled[ read_buffer_id ] ;
}

//**********************************************************************************************
audio_system::schedule_datas_ref_t audio_system::shared_object::write_buffer( void_t )
{
    return scheduled[ (read_buffer_id+1) & 1 ] ;
}

//*************************************************************************************
audio_system::audio_system( void_t ) 
{
    _service_ptr = so_audiox::service_t::create( "[audio_system::audio_system] : service" ) ;

    so_audio::api_creator_t::apis_t apis ;
    so_audio::api_creator_t::create_defaults( apis ) ;

    for( auto * ptr : apis ) 
    {
        if( so_core::is_nullptr(ptr) ) continue ;

        this_t::shared_object_ptr_t sdp = so_audiox::memory::alloc( this_t::shared_object_t(),
            "[audio_system::audio_system] : shared data" ) ;

        sdp->aptr = ptr ;
        
        so_audiox::api_id_t const aid( _apis.size() ) ;
        so_audiox::iglobal_service_ptr_t service_ptr = _service_ptr ;

        // create audio thread
        std::thread t( [sdp, aid, service_ptr]( void_t )
        {
            // initialize audio api in its respective thread
            {
                auto const res = sdp->aptr->initialize() ;
                if( so_core::is_not(res) )
                {
                    so_log::log::error( "[audio_system::audio_system] : initialize for " +
                        so_audio::to_string( sdp->aptr->get_type() ) ) ;
                    return ;
                }
            }

            {
                so_thread::lock_guard_t lk(sdp->mtx_init) ;
                sdp->has_init = true ;
            }
            sdp->cv_init.notify_all() ;

            so_std::vector< schedule_data_t > stage_00 ;
            so_std::vector< so_audiox::audio_technique_ptr_t > stage_01 ;

            typedef std::chrono::high_resolution_clock lclock_t ;
            lclock_t::time_point tp_begin = lclock_t::now() ;

            while( sdp->running )
            {                
                // part 0: wait for the user sync to be ready
                // in this part, all sound creation and transfer should be done
                sdp->sync_00_wait() ;

                sdp->flip_scheduled() ;
                sdp->write_buffer().clear() ;

                stage_00.reserve( sdp->read_buffer().size() ) ;
                stage_01.reserve( sdp->read_buffer().size() ) ;

                // 0 : proceed with states of unfinished techniques
                {
                    for( auto & sd : stage_00 )
                    {
                        sd.tptr->part_00( sd.ts, aid, sdp->aptr,
                            service_ptr, sdp->service_ptr ) ;
                    }
                    stage_00.clear() ;
                }

                // 1. proceed with states of new techniques
                // if a technique is ready, is will be scheduled for stage 1
                {
                    for( size_t i=0; i<sdp->read_buffer().size(); ++i )
                    //for( auto & sd : sdp->read_buffer() )
                    {
                        this_t::schedule_data_t & sd = sdp->read_buffer()[ i ] ;

                        auto const res = sd.tptr->part_00( sd.ts, aid, sdp->aptr,
                            service_ptr, sdp->service_ptr ) ;

                        if( res == so_audiox::technique_state::execute )
                        {
                            stage_01.push_back( sd.tptr ) ;
                        }
                        else if(
                            res != so_audiox::technique_state::raw ||
                            res != so_audiox::technique_state::ready )
                        {
                            auto const iter = std::find_if(
                                stage_00.begin(), stage_00.end(), [&]( schedule_data_t const & v )
                            {
                                return v.tptr == sd.tptr ;
                            } ) ;

                            if( iter == stage_00.end() )
                                stage_00.push_back( sd ) ;
                        }
                    }
                }

                // part 1: send that all data is ready and has
                // been transfered
                sdp->sync_01_send() ;

                {
                    so_audiox::iplug::execute_info ei ;
                    ei.dt = double_t( std::chrono::duration_cast<std::chrono::milliseconds>( 
                        lclock_t::now() - tp_begin ).count() ) / 1000.0 ;

                    tp_begin = lclock_t::now() ;

                    for( auto * tptr : stage_01 )
                    {
                        tptr->part_01_render( ei ) ;
                    }

                    for( auto * tptr : stage_01 )
                    {
                        tptr->part_01_render_end() ;
                    }

                    stage_01.clear() ;
                }
            }

            //for( size_t i=0; i<sdp->read_buffer().size(); ++i )
            for( auto & td : sdp->read_buffer() )
            {
                td.tptr->part_00( technique_state::release, aid, sdp->aptr,
                    service_ptr, sdp->service_ptr ) ;
            }
            sdp->aptr->release() ;

        } ) ;

        sdp->t = std::move(t) ;

        _apis.push_back( sdp ) ;
    }

    for( auto * ptr : _apis )
    {
        so_thread::lock_t lk( ptr->mtx_init ) ;
        while( so_core::is_not(ptr->has_init) ) ptr->cv_init.wait( lk ) ;
    }
}

//*************************************************************************************
audio_system::audio_system( this_rref_t rhv )
{
    _apis = std::move( rhv._apis ) ;
    _tecs = std::move( rhv._tecs ) ;
    
    so_move_member_ptr( _service_ptr, rhv ) ;
}

//*************************************************************************************
audio_system::~audio_system( void_t )
{
    // 1. schedule all techs for release
    for( auto & t : _tecs )
    {
        schedule_data_t sd ;
        sd.ts = technique_state::release ;
        sd.tptr = t.second.tptr ;

        so_audiox::api_id_t aid = sd.tptr->get_used_api() ;
        if( aid != so_audiox::api_id_t( -1 ) )
        {
            _apis[aid]->write_buffer().push_back( sd ) ;
        }
    }
    
    // 2. shutdown all running api threads
    for( auto * e : _apis )
    {
        e->running = false ;
        e->sync_00_send() ;

        if( e->t.joinable() )
            e->t.join() ;

        e->aptr->destroy() ;
    }

    // 3. destroy all registered techniques
    for( auto & t : _tecs )
    {
        so_audiox::audio_technique_t::destroy( t.second.tptr ) ;
    }

    for( size_t i=0; i<_apis.size(); ++i )
    {
        so_audiox::memory::dealloc( _apis[i] ) ;
    }

    if( so_core::is_not_nullptr( _service_ptr ) )
        _service_ptr->destroy() ;
}

//*************************************************************************************
audio_system::this_ptr_t audio_system::create( so_memory::purpose_cref_t p )
{
    return so_audiox::memory::alloc( this_t(), p ) ;
}

//*************************************************************************************
void_t audio_system::destroy( this_ptr_t ptr )
{
    so_audiox::memory::dealloc( ptr ) ;
}

//*************************************************************************************
so_audiox::service_ptr_t audio_system::get_service( void_t )
{
    return _service_ptr ;
}

//*************************************************************************************
void_t audio_system::update( void_t ) 
{
    // sync point 0 : wait for user thread(s) in render threads
    this_t::send_sync_00() ;

    // gpu transfer happens here until render threads signal sync_01
}

//*************************************************************************************
void_t audio_system::render_begin( void_t )
{
    // sync point 1 : wait for render thread(s)
    this_t::wait_for_sync_01() ;

}

//*************************************************************************************
void_t audio_system::render_end( void_t )
{
    for( size_t i=0; i<_apis.size(); ++i )
    {
        this_t::shared_object_ptr_t so = _apis[ i ] ;
        
        for( size_t j=0; j<so->read_buffer().size(); ++j )
        //for( auto & sd : w.shared_ptr->read_buffer() )
        {
            schedule_data_t & sd = so->read_buffer()[ j ] ;
            sd.tptr->part_01_update() ;
        }
    }
}

//*************************************************************************************
technique_id_t audio_system::register_technique( so_audiox::iplug_factory_ptr_t fac_ptr ) 
{
    if( so_core::is_nullptr(fac_ptr) )
        return so_audiox::technique_id_t( -1 ) ;

    this_t::technique_data_t td ;
    td.tptr = so_audiox::audio_technique_t::create( std::move( so_audiox::audio_technique_t( fac_ptr ) ),
        "[audio_system::register_technique] : audio_technique" ) ;

    so_audiox::technique_id_t tid ;

    {
        so_thread::lock_guard_t lk( _mtx_tech ) ;

        tid = _cur_tech_id++ ;

        _tecs[tid] = td ;
        
    }

    return tid ;
}

//*************************************************************************************
so_audiox::result audio_system::schedule( technique_id_t tid ) 
{
    return this_t::schedule_for( technique_state::execute, tid ) ;
}

//*************************************************************************************
so_audiox::result audio_system::schedule_for_init( so_audiox::technique_id_t tid ) 
{
    return this_t::schedule_for( technique_state::init, tid ) ;
}

//*************************************************************************************
so_audiox::result audio_system::schedule_for_release( so_audiox::technique_id_t tid ) 
{
    return this_t::schedule_for( technique_state::release, tid ) ;
}

//*************************************************************************************
so_audiox::result audio_system::schedule_for( technique_state ts, technique_id_t tid ) 
{
    if( _apis.size() == 0 )
        return so_audiox::failed ;

    so_audiox::audio_technique_ptr_t tptr = nullptr ;

    {
        so_thread::lock_guard_t lk( _mtx_tech ) ;
        auto const iter = _tecs.find( tid ) ;
        if( iter == _tecs.end() )
            return so_audiox::invalid_argument ;

        tptr = iter->second.tptr ;
    }

    {
        schedule_data_t sd ;
        sd.ts = ts ;
        sd.tptr = tptr ;

        so_audiox::api_id_t aid = tptr->get_used_api() ;
        if( aid == so_audiox::api_id_t(-1) )
        {
            for( auto & api_data : _apis )
            {
                so_thread::lock_guard_t lk( api_data->mtx_scheduled ) ;
                api_data->write_buffer().push_back( sd ) ;
            }
        }
        else 
        {
            _apis[aid]->write_buffer().push_back( sd ) ;
        }
    }

    return so_audiox::ok ;
}

//**********************************************************************************************
void_t audio_system::send_sync_00( void_t )
{
    for( auto & api : _apis )
    {
        api->sync_00_send() ;
    }
}

//**********************************************************************************************
void_t audio_system::wait_for_sync_01( void_t )
{
    for( auto & api : _apis )
    {
        api->sync_01_wait() ;
    }
}

//**********************************************************************************************
so_audiox::result audio_system::schedule_for_init_and_wait( so_audiox::technique_id_t tid )
{
    {
        auto const res = this_t::schedule_for_init( tid ) ;
        if( so_audiox::no_success( res ) )
            return res ;
    }

    return this_t::wait_for_init( tid ) ;
}

//**********************************************************************************************
so_audiox::result audio_system::wait_for_init( so_audiox::technique_id_t tid )
{
    if( _apis.size() == 0 )
        return so_audiox::failed ;

    so_audiox::audio_technique_ptr_t tptr = nullptr ;

    {
        so_thread::lock_guard_t lk( _mtx_tech ) ;
        auto const iter = _tecs.find( tid ) ;
        if( iter == _tecs.end() )
            return so_audiox::invalid_argument ;

        tptr = iter->second.tptr ;
    }

    auto const res = tptr->wait_for_init() ;
    return so_core::is_not( res ) ? so_audiox::failed : so_audiox::ok  ;
}