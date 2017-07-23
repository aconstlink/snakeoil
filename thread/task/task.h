//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_THREAD_TASK_H_
#define _SNAKEOIL_THREAD_TASK_H_

#include "itask.h"
#include "../mutex.h"

#include <vector>

namespace so_thread
{
    class SNAKEOIL_THREAD_API task : public itask
    {
        so_this_typedefs( task ) ;

        so_typedefs( itask::tasks_t, tasks ) ;
        
    private:

        so_thread::mutex_t _mtx ;
        
        /// tasks outgoing from this task
        tasks_t _tasks_out ;

        /// tasks incoming into this task
        tasks_t _tasks_in ;
        size_t _num_tasks_in = 0 ;

    private:

        task( this_cref_t ) {}

    public:

        task( void_t ) ;
        task( this_rref_t ) ;
        virtual ~task( void_t ) ;

    public:

        virtual itask_ptr_t then( itask_ptr_t ) final ;
        virtual void_t execute( locked_tasks_ref_t ) final ;

    public: // interface

        virtual void_t run( void_t ) = 0 ;
        virtual void_t destroy( void_t ) = 0 ;

    private:

        virtual size_t connect_incoming( itask_ptr_t ) final ;
        virtual size_t disconnect_incoming( itask_ptr_t ) final ;

    };
}

#endif
