//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "task_graph.h"
#include "task_scheduler.h"

#include "serial_executor.h"
#include "async_executor.h"

using namespace so_thread ;

//*************************************************************************************
task_scheduler::task_scheduler( void_t ) 
{

}

//*************************************************************************************
task_scheduler::task_scheduler( this_rref_t rhv )
{
    _asyncs = std::move(rhv._asyncs) ;
    _serials = std::move(rhv._serials) ;
}

//*************************************************************************************
task_scheduler::~task_scheduler( void_t )
{

}

//*************************************************************************************
task_scheduler::this_ptr_t task_scheduler::create( so_memory::purpose_cref_t p )
{
    return so_thread::memory::alloc( this_t(), p ) ;
}

//*************************************************************************************
task_scheduler::this_ptr_t task_scheduler::create( this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_thread::memory::alloc( std::move(rhv), p ) ;
}

//*************************************************************************************
void_t task_scheduler::destroy( this_ptr_t ptr )
{
    so_thread::memory::dealloc( ptr ) ;
}

//*************************************************************************************
void_t task_scheduler::update( void_t )
{
    // update all serial tasks
    {
        tasks_t serials ;
        {
            so_thread::lock_guard_t lk(_mtx_serial) ;
            serials = std::move( _serials ) ;
        }
        so_thread::serial_executor se ;
        se.consume( std::move(serials) ) ;
    }

    // update all async tasks
    {
        tasks_t asyncs ;
        {
            so_thread::lock_guard_t lk( _mtx_async ) ;
            asyncs = std::move( _asyncs ) ;
        }
        so_thread::async_executor ae ;
        ae.consume_and_wait( std::move( asyncs ) ) ;
    }
}

//*************************************************************************************
void_t task_scheduler::async_now( so_thread::itask_ptr_t tptr, so_thread::sync_object_ptr_t sptr )
{
    /*
    std::async( std::launch::async, [=]( void_t )
    {
        so_thread::async_executor_t ae ;
        ae.consume_and_wait( tptr ) ;

        so_thread::sync_object::set_and_signal( sptr ) ;
    } ) ;
    */

    // @todo make async again. future will block on destruction
    std::thread t( [=] ( void_t )
    {
        so_thread::async_executor_t ae ;
        ae.consume_and_wait( tptr ) ;

        so_thread::sync_object::set_and_signal( sptr ) ;
    } ) ;

    t.detach( ) ;
    
}

//*************************************************************************************
void_t task_scheduler::async_now( so_thread::task_graph_rref_t tg, so_thread::sync_object_ptr_t sptr ) 
{
    // invalidate the end task pointer.
    tg.end_moved() ;
    this->async_now( tg.begin_moved(), sptr ) ;
}

//*************************************************************************************
void_t task_scheduler::serial_now( so_thread::itask_ptr_t tptr, so_thread::sync_object_ptr_t sptr )
{
    
    std::thread t([=] (void_t)
    {
        so_thread::serial_executor_t se ;
        se.consume( tptr ) ;

        so_thread::sync_object::set_and_signal( sptr ) ;
    } ) ;
    t.detach() ;
}

//*************************************************************************************
void_t task_scheduler::async_on_update( so_thread::itask_ptr_t ptr )
{
    if( so_core::is_nullptr(ptr) )
        return ;

    so_thread::lock_guard_t lk(_mtx_async) ;
    _asyncs.push_back( ptr ) ;
}

//*************************************************************************************
void_t task_scheduler::serial_on_update( so_thread::itask_ptr_t ptr )
{
    if( so_core::is_nullptr( ptr ) )
        return ;

    so_thread::lock_guard_t lk( _mtx_serial ) ;
    _serials.push_back( ptr ) ;
}

//*************************************************************************************
void_t task_scheduler::destroy( void_t ) 
{
    this_t::destroy( this ) ;
}
