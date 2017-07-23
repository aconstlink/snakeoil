//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_THREAD_ITASK_SCHEDULER_H_
#define _SNAKEOIL_THREAD_ITASK_SCHEDULER_H_

#include "../protos.h"
#include "../api.h"
#include "../typedefs.h"

#include "itask.h"

namespace so_thread
{
    class SNAKEOIL_THREAD_API itask_scheduler
    {
    public:

        /// will spawn all *_on_update scheduled tasks.
        virtual void_t update( void_t ) = 0 ;

    public:

        /// spawns a new thread now and consumes the passed task graph 
        /// asynchronously for each task
        virtual void_t async_now( so_thread::itask_ptr_t, 
            so_thread::sync_object_ptr_t = nullptr ) = 0 ;

        virtual void_t async_now( so_thread::task_graph_rref_t, 
            so_thread::sync_object_ptr_t = nullptr ) = 0 ;

        // spawns a new thread and consumes the passed task graph serially
        virtual void_t serial_now( so_thread::itask_ptr_t, 
            so_thread::sync_object_ptr_t = nullptr ) = 0 ;

        /// Executes on update call and consumes the passed task graph 
        /// asynchronously. Each task is executed in a separate thread.
        /// @note the update will block until all these tasks are executed.
        virtual void_t async_on_update( so_thread::itask_ptr_t ) = 0 ;

        /// executes the task graph serially during the update call. 
        /// @note the update will block until all these tasks are executed.
        /// only use this scheduling method if tasks really need to be 
        /// executed on a very specific time.
        virtual void_t serial_on_update( so_thread::itask_ptr_t ) = 0 ;

    public:

        virtual void_t destroy( void_t ) = 0 ;
    };
    so_typedef( itask_scheduler ) ;
}

#endif
