//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_THREAD_TASK_ASYNC_EXECUTOR_H_
#define _SNAKEOIL_THREAD_TASK_ASYNC_EXECUTOR_H_

#include "itask.h"

#include <future>

namespace so_thread
{
    class SNAKEOIL_THREAD_API async_executor
    {
        so_this_typedefs( async_executor ) ;

        typedef std::future<void_t> future_t ;
        so_typedefs( so_std::vector<future_t>, futures ) ;

        so_typedefs( so_std::vector< itask_ptr_t >, tasks ) ;

    public:

        /// blocks until the task graph is processed.
        /// all tasks will be destroyed after completion.
        void_t consume_and_wait( so_thread::itask_ptr_t ) ;

        void_t consume_and_wait( tasks_rref_t ) ;

    };
    so_typedef( async_executor ) ;
}

#endif
