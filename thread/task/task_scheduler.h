//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_THREAD_TASK_TASK_SCHEDULER_H_
#define _SNAKEOIL_THREAD_TASK_TASK_SCHEDULER_H_

#include "itask_scheduler.h"

#include "../primitive/sync_object.hpp"

namespace so_thread
{
    class SNAKEOIL_THREAD_API task_scheduler : public itask_scheduler
    {
        so_this_typedefs( task_scheduler ) ;

        so_typedefs( so_std::vector< itask_ptr_t >, tasks ) ;

    private:

        so_thread::mutex_t _mtx_serial ;
        tasks_t _serials ;

        so_thread::mutex_t _mtx_async ;
        tasks_t _asyncs ;

    public:

        task_scheduler( void_t ) ;
        task_scheduler( this_rref_t ) ;
        virtual ~task_scheduler( void_t ) ;

    public:

        static this_ptr_t create( so_memory::purpose_cref_t ) ;
        static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
        static void_t destroy( this_ptr_t ) ;


    public:

        virtual void_t update( void_t ) ;

    public:

        virtual void_t async_now( so_thread::itask_ptr_t, so_thread::sync_object_ptr_t = nullptr ) ;
        virtual void_t async_now( so_thread::task_graph_rref_t, so_thread::sync_object_ptr_t = nullptr ) ;
        virtual void_t serial_now( so_thread::itask_ptr_t, so_thread::sync_object_ptr_t = nullptr ) ;
        virtual void_t async_on_update( so_thread::itask_ptr_t ) ;
        virtual void_t serial_on_update( so_thread::itask_ptr_t ) ;

    public:

        virtual void_t destroy( void_t ) ;
    };
    so_typedef( task_scheduler ) ;
}

#endif
