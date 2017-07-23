//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "task.h"

#include <snakeoil/core/assert.h>

#include <algorithm>

using namespace so_thread ;

//*************************************************************************************
task::task( void_t )
{
}

//*************************************************************************************
task::task( this_rref_t rhv )
{
    so_thread::lock_guard_t lk(rhv._mtx) ;

    _tasks_out = std::move(rhv._tasks_out) ;
    _tasks_in = std::move(rhv._tasks_in) ;
    _num_tasks_in = rhv._num_tasks_in ;
}

//*************************************************************************************
task::~task( void_t )
{
    so_assert( _num_tasks_in == 0 ) ;
    so_assert( _tasks_out.size() == 0 ) ;
}

//*************************************************************************************
void_t task::execute( locked_tasks_ref_t tasks_follow ) 
{
    {
        so_thread::lock_guard_t lk( _mtx );
        so_assert( _num_tasks_in == 0 ) ;
    }

    so_std::vector< itask_ptr_t > this_tasks ;

    // 1. run
    this->run() ;

    // 2. move all following tasks out
    {
        so_thread::lock_guard_t lk( _mtx ) ;
        this_tasks = std::move( _tasks_out ) ;
    }
    
    // 3. disconnect
    for( auto * task_ptr : this_tasks )
    {
        if( itask::connection_accessor::disconnect( this, task_ptr ) == 0 )
            tasks_follow.push_back( task_ptr ) ;
    }
}

//*************************************************************************************
itask_ptr_t task::then( itask_ptr_t ptr ) 
{
    if( so_core::is_nullptr(ptr) )
        return this ;
    
    if( this == ptr ) 
        return this ;

    // outgoing
    {
        so_thread::lock_guard_t lk( _mtx );
        auto found = std::find( _tasks_out.begin(), _tasks_out.end(), ptr );
        if( found != _tasks_out.end() )
            return ptr;

        _tasks_out.push_back( ptr );
    }

    // incoming
    {
        itask::connection_accessor::connect( this, ptr ) ;
    }

    return ptr ;
}

//*************************************************************************************
size_t task::connect_incoming( itask_ptr_t incoming_ptr )
{
    if( so_core::is_nullptr(incoming_ptr) ) 
        return _num_tasks_in ;
 
    so_thread::lock_guard_t lk(_mtx) ;

    auto iter = std::find( _tasks_in.begin(), _tasks_in.end(), incoming_ptr ) ;
    if( iter != _tasks_in.end() )
        return _num_tasks_in ;

    _tasks_in.push_back( incoming_ptr ) ;
    
    return ++_num_tasks_in ;
}

//*************************************************************************************
size_t task::disconnect_incoming( itask_ptr_t incoming_ptr )
{
    if( so_core::is_nullptr( incoming_ptr ) )
        return _num_tasks_in;

    so_thread::lock_guard_t lk( _mtx );

    so_assert( _num_tasks_in != 0 ) ;

    auto iter = std::find( _tasks_in.begin(), _tasks_in.end(), incoming_ptr );
    if( iter == _tasks_in.end() )
        return _num_tasks_in;

    size_t const index = iter - _tasks_in.begin() ;

    _tasks_in[index] = _tasks_in.back() ;
    _tasks_in.back() = nullptr ;    

    return --_num_tasks_in ;
}
