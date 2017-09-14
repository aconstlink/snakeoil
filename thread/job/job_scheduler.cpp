//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "job_scheduler.h"

#include <snakeoil/log/global.h>

using namespace so_thread ;

//***********************************************************************
job_scheduler::job_scheduler( void_t )
{
    _sd_ptr = so_thread::memory::alloc( shared_data_t(), 
        "[job_scheduler::job_scheduler] : shared_data" ) ;

    this_t::shared_data_ptr_t captured_ptr = _sd_ptr ;

    size_t const num_threads = std::thread::hardware_concurrency() << 1 ;

    _threads.resize( num_threads ) ;
    for( size_t i = 0; i < num_threads; ++i )
    {
        _threads[ i ] = std::thread( [captured_ptr]( void_t )
        {
            //so_log::global::status( "Test: hello thread" ) ;

            {
                so_thread::lock_t lk( captured_ptr->_mtx_online ) ;
                captured_ptr->_threads_online++ ;
            }
            captured_ptr->_cv_online.notify_all() ;

            while( captured_ptr->_running )
            {
                this_t::job_data_t jd ;

                {
                    so_thread::lock_t lk( captured_ptr->_mtx_jobs ) ;
                    while( captured_ptr->_jpos == 0 && captured_ptr->_running ) 
                        captured_ptr->_cv_jobs.wait( lk ) ;

                    if( so_core::is_not( captured_ptr->_running ) ) break ;

                    jd = captured_ptr->_jobs[ --captured_ptr->_jpos ] ;
                }

                jd.job_ptr->execute() ;
                jd.ff( jd.job_ptr ) ;
            }

        } ) ;
    }

    this_t::wait_for_threads_online( num_threads ) ;
}

//***********************************************************************
job_scheduler::job_scheduler( this_rref_t rhv )
{
    _threads = std::move( rhv._threads ) ;
    so_move_member_ptr( _sd_ptr, rhv ) ;
}

//***********************************************************************
job_scheduler::~job_scheduler( void_t )
{
    if( so_core::is_not_nullptr(_sd_ptr) )
    {
        _sd_ptr->_running = false ;
        _sd_ptr->_cv_jobs.notify_all() ;
    }
    
    for( auto & t : _threads )
    {
        if( t.joinable() ) t.join() ;
    }

    if( so_core::is_not_nullptr( _sd_ptr ) )
    {
        so_thread::memory::dealloc( _sd_ptr ) ;
    }
}

//***********************************************************************
so_thread::result job_scheduler::schedule( so_thread::ijob_ptr_t jptr, finish_callback_t ff )
{
    if( so_core::is_nullptr( jptr ) ) 
        return so_thread::invalid_argument;

    job_data_t jd ;
    jd.ff = ff ;
    jd.job_ptr = jptr ;

    {
        so_thread::lock_guard_t lk( _sd_ptr->_mtx_jobs ) ;

        if( _sd_ptr->_jpos >= _sd_ptr->_jobs.size() )
            _sd_ptr->_jobs.resize( _sd_ptr->_jobs.size() + 100 ) ;

        _sd_ptr->_jobs[_sd_ptr->_jpos++] = jd ;
    }

    _sd_ptr->_cv_jobs.notify_all() ;

    return so_thread::ok;
}

//***********************************************************************
job_scheduler::this_ptr_t job_scheduler::create( so_memory::purpose_cref_t p )
{
    return so_thread::memory::alloc( this_t(), p ) ;
}

//***********************************************************************
void_t job_scheduler::destroy( this_ptr_t ptr )
{
    so_thread::memory::dealloc( ptr ) ;
}

//***********************************************************************
void_t job_scheduler::wait_for_threads_online( size_t const num_threads )
{
    so_thread::lock_t lk( _sd_ptr->_mtx_online ) ;
    while( _sd_ptr->_threads_online != num_threads ) _sd_ptr->_cv_online.wait( lk ) ;
}