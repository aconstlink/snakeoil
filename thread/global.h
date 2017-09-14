//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "api.h"
#include "protos.h"
#include "typedefs.h"

namespace so_thread
{
    class SNAKEOIL_THREAD_API global
    {
        so_this_typedefs( global ) ;

    private:

        job_scheduler_ptr_t _job_scheduler_ptr = nullptr ;
        itask_scheduler_ptr_t _task_scheduler_ptr = nullptr ;

    private:

        static this_ptr_t _ptr ;

    public:

        global( void_t ) ;
        global( this_cref_t ) = delete ;
        global( this_rref_t ) ;
        ~global( void_t ) ;

    public:

        static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
        static void_t destroy( this_ptr_t ) ;

    public:

        static void_t init( void_t ) ;
        static void_t deinit( void_t ) ;
        static void_t update( void_t ) ;
        static this_ptr_t get( void_t ) ;
        static job_scheduler_ptr_t job_scheduler( void_t ) ;
        static itask_scheduler_ptr_t task_scheduler( void_t ) ;
    };
    so_typedef( global ) ;
}