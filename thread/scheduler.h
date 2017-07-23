//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_THREAD_SCHEDULER_H_
#define _SNAKEOIL_THREAD_SCHEDULER_H_

#include "task/itask_scheduler.h"
#include "job/job_scheduler.h"

namespace so_thread
{
    struct SNAKEOIL_THREAD_API scheduler
    {
        so_this_typedefs( scheduler ) ;

    private:

        static job_scheduler_ptr_t _job_scheduler_ptr ;
        static itask_scheduler_ptr_t _task_scheduler_ptr ;
        static size_t _ref_count ;

    public:

        static so_thread::result init( void_t ) ;
        static so_thread::result deinit( void_t ) ;
        
        static void_t assert_scheduler( void_t ) ;

        static void_t make_move( void_t ) ;

    public:

        static void_t update( void_t ) ;
        static so_thread::itask_scheduler_ptr_t task_scheduler( void_t ) ;
        static so_thread::itask_scheduler_ptr_t ts( void_t ) ;


        static so_thread::job_scheduler_ptr_t job_scheduler( void_t ) ;
        static so_thread::job_scheduler_ptr_t js( void_t ) ;
    };
}

#endif
