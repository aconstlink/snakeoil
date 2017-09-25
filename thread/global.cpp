//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "global.h"

#include "job/job_scheduler.h"
#include "task/task_scheduler.h"

#include <snakeoil/log/global.h>

using namespace so_thread ;

mutex_t global::_mtx ;
global * global::_ptr = nullptr ;

//***********************************************************************
global::global( void_t )
{
    _job_scheduler_ptr = so_thread::job_scheduler_t::create( 
        "[global::global] : job_scheduler" ) ;

    _task_scheduler_ptr = so_thread::task_scheduler_t::create( 
        "[global::global] : task_scheduler" ) ;
}

//***********************************************************************
global::global( this_rref_t rhv )
{
    so_move_member_ptr( _job_scheduler_ptr, rhv ) ;
    so_move_member_ptr( _task_scheduler_ptr, rhv ) ;
}

//***********************************************************************
global::~global( void_t )
{
    if( so_core::is_not_nullptr( _task_scheduler_ptr ) )
    {
        _task_scheduler_ptr->destroy() ;
        _task_scheduler_ptr = nullptr ;
    }

    so_thread::job_scheduler_t::destroy( _job_scheduler_ptr ) ;
}

//***********************************************************************
global::this_ptr_t global::create( this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_thread::memory::alloc( std::move( rhv ), p ) ;
}

//***********************************************************************
void_t global::destroy( this_ptr_t ptr )
{
    so_thread::memory::dealloc( ptr ) ;
}

//***********************************************************************
global::this_ptr_t global::init( void_t )
{
    if( so_core::is_not_nullptr( _ptr ) )
        return this_t::_ptr ;

    {
        so_thread::lock_guard_t lk( this_t::_mtx ) ;

        if( so_core::is_not_nullptr( _ptr ) )
            return this_t::_ptr ;

        _ptr = this_t::create( this_t(),
            "[so_thread::global::init] : global singleton lazy initialization" ) ;

        so_log::global::status( "[online] : snakeoil thread" ) ;
    }

    return this_t::_ptr ;
}

//***********************************************************************
void_t global::deinit( void_t )
{
    if( so_core::is_nullptr( _ptr ) )
        return ;

    this_t::destroy( _ptr ) ;
    _ptr = nullptr ;
}

//***********************************************************************
void_t global::update( void_t )
{
    this_t::get()->_task_scheduler_ptr->update() ;
}

//***********************************************************************
global::this_ptr_t global::get( void_t )
{
    return this_t::init() ;
}

//***********************************************************************
job_scheduler_ptr_t global::job_scheduler( void_t )
{
    return this_t::get()->_job_scheduler_ptr ;
}

//***********************************************************************
itask_scheduler_ptr_t global::task_scheduler( void_t )
{
    return this_t::get()->_task_scheduler_ptr ;
}

//***********************************************************************