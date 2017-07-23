//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_THREAD_JOB_JOB_SCHEDULER_H_
#define _SNAKEOIL_THREAD_JOB_JOB_SCHEDULER_H_

#include "job.h"

#include <snakeoil/std/container/vector.hpp>
#include <snakeoil/thread/mutex.h>

namespace so_thread
{
    class SNAKEOIL_THREAD_API job_scheduler
    {
        so_this_typedefs( job_scheduler ) ;

        so_typedefs( so_std::vector< std::thread >, threads ) ;

    public:

        so_typedefs( std::function < void_t ( so_thread::ijob_ptr_t ) >, finish_callback ) ;

    private:

        struct job_data
        {
            so_thread::ijob_ptr_t job_ptr ;
            finish_callback_t ff ;
        };
        so_typedef( job_data ) ;

        so_typedefs( so_std::vector< job_data_t >, jobs ) ;

    private:

        struct shared_data
        {
            so_this_typedefs( shared_data ) ;

        public:

            shared_data( void_t ) {}
            shared_data( this_cref_t ) = delete ;
            shared_data( this_rref_t rhv )
            {
                _jobs = std::move( rhv._jobs ) ;
                _jpos = rhv._jpos ;
                _running = rhv._running ;
            }
            ~shared_data( void_t ) {}

        public:

            so_thread::mutex_t _mtx_jobs ;
            so_thread::condition_variable_t _cv_jobs ;
            jobs_t _jobs ;
            size_t _jpos = 0 ;

            bool_t _running = true ;


            so_thread::mutex_t _mtx_online ;
            so_thread::condition_variable_t _cv_online ;
            size_t _threads_online = 0 ;
        };
        so_typedef( shared_data ) ;

    private:

        threads_t _threads ;
        shared_data_ptr_t _sd_ptr = nullptr ;
        
    public:

        job_scheduler( void_t ) ;
        job_scheduler( this_cref_t ) = delete ;
        job_scheduler( this_rref_t ) ;
        ~job_scheduler( void_t ) ;

    public:

        static this_ptr_t create( so_memory::purpose_cref_t ) ;
        static void_t destroy( this_ptr_t ) ;

    public:

        so_thread::result schedule( so_thread::ijob_ptr_t, finish_callback_t ) ;

    private:

        void_t wait_for_threads_online( size_t const num_threads ) ;

    };
    so_typedef( job_scheduler ) ;
}

#endif