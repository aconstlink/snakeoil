//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_THREAD_PROTOS_H_
#define _SNAKEOIL_THREAD_PROTOS_H_

#include <snakeoil/core/macros/typedef.h>

namespace so_thread
{
    so_class_proto_typedefs( itask_scheduler ) ;
    so_class_proto_typedefs( task_scheduler ) ;
    so_class_proto_typedefs( job_scheduler ) ;
    so_class_proto_typedefs( itask ) ;
    so_class_proto_typedefs( task_graph ) ;

    so_class_proto_typedefs( worker_thread ) ;
    so_class_proto_typedefs( sync_object ) ;

    so_class_proto_typedefs( ithread_task_scheduler ) ;
    so_class_proto_typedefs( thread_task_scheduler ) ;
    so_class_proto_typedefs( iserial_task_scheduler ) ;
    so_class_proto_typedefs( serial_task_scheduler ) ;

    so_class_proto_typedefs( ijob ) ;
}

#endif

