//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "scheduler.h"

#include "task/task_scheduler.h"

#include <snakeoil/log/log.h>
#include <snakeoil/core/assert.h>

using namespace so_thread ;

so_thread::itask_scheduler_ptr_t scheduler::_task_scheduler_ptr = nullptr ;
so_thread::job_scheduler_ptr_t scheduler::_job_scheduler_ptr = nullptr ;

size_t scheduler::_ref_count = 0 ;

//*************************************************************************************
so_thread::result scheduler::init( void_t ) 
{
    if( so_core::is_not_nullptr(_task_scheduler_ptr) )
        return so_thread::ok ;
        
    _task_scheduler_ptr = so_thread::task_scheduler_t::create( 
            "[so_thread::scheduler::init] : default task scheduler") ;

    _job_scheduler_ptr = so_thread::job_scheduler_t::create( 
        "[so_thread::scheduler::init] : default job scheduler" ) ;

    _ref_count = 1 ;

    return so_thread::ok ;
}

//*************************************************************************************
so_thread::result scheduler::deinit( void_t ) 
{
    if( so_log::log::error( _ref_count == 0, 
        "[so_thread::scheduler::deinit] : ref count == 0" ) )
        return so_thread::ok ;

    if( --_ref_count > 0 ) 
        return so_thread::ok ;

    if( so_core::is_not_nullptr( _task_scheduler_ptr ) )
    {
        _task_scheduler_ptr->destroy();
        _task_scheduler_ptr = nullptr ;
    }

    {
        so_thread::job_scheduler_t::destroy( _job_scheduler_ptr ) ;
        _job_scheduler_ptr = nullptr ;
    }

    return so_thread::ok ;
}

//*************************************************************************************
void_t scheduler::assert_scheduler( void_t ) 
{
    if( so_log::log::error( _task_scheduler_ptr == nullptr, 
        "[so_thread::scheduler::assert_scheduler] : Call scheduler::init in your main." ) )
    {
        so_assert( false ) ;
    }    
}

//*************************************************************************************
void_t scheduler::make_move( void_t ) 
{
    ++_ref_count ;
}

//*************************************************************************************
void_t scheduler::update( void_t ) 
{
    if( so_log::log::error( so_core::is_nullptr(_task_scheduler_ptr),
        "[so_thread::scheduler::assert_scheduler] : Call scheduler::init in your main." ) )
        return ;

    _task_scheduler_ptr->update() ;
}

//*************************************************************************************
so_thread::itask_scheduler_ptr_t scheduler::task_scheduler( void_t )
{
    return _task_scheduler_ptr ;
}

//*************************************************************************************
so_thread::itask_scheduler_ptr_t scheduler::ts( void_t )
{
    return _task_scheduler_ptr ;
}

//*************************************************************************************
so_thread::job_scheduler_ptr_t scheduler::job_scheduler( void_t )
{
    return _job_scheduler_ptr ;
}

//*************************************************************************************
so_thread::job_scheduler_ptr_t scheduler::js( void_t )
{
    return _job_scheduler_ptr ;
}
