//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_THREAD_TASK_SERIAL_EXECUTOR_H_
#define _SNAKEOIL_THREAD_TASK_SERIAL_EXECUTOR_H_

#include "itask.h"

namespace so_thread
{
    class SNAKEOIL_THREAD_API serial_executor
    {
        so_this_typedefs( serial_executor ) ;

        so_typedefs( so_std::vector<itask_ptr_t>, tasks ) ;

    public:

        void_t consume( itask_ptr_t ) ;
        void_t consume( tasks_rref_t ) ;
    };
    so_typedef( serial_executor ) ;
}

#endif
