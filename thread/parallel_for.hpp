//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_THREAD_PARALLEL_FOR_HPP_
#define _SNAKEOIL_THREAD_PARALLEL_FOR_HPP_

#include "range/range_1d.hpp"
#include "job/job_funk.h"
#include "job/job.h"

#include "global.h"
#include "scheduler.h"

#include <snakeoil/memory/arena/exp_arena.hpp>

#include <thread>

namespace so_thread
{
    template< typename T >
    void_t parallel_for( so_thread::range_1d< T > const & r, so_thread::funk_t<T> f )
    {
        size_t const num_threads = std::thread::hardware_concurrency() ;

        so_typedefs( so_thread::job_1d<T>, job ) ;

        if( size_t(r.difference()) < num_threads * 50 )
        {
            job_1d< T >( r, f ).execute() ;
            return ;
        }

        so_typedefs( so_thread::range_1d<T>, range ) ;
        so_typedefs( so_memory::so_experimental::arena< job_t >, job_arena ) ;
        
        static job_arena_t local_arena ;

        T const per_job = r.difference() / T(num_threads) ;
        T const rest_job = r.difference() - per_job * T( num_threads ) ;

        T start = r.begin() ;

        so_thread::mutex_t mtx ;
        so_thread::condition_variable_t cv ;

        size_t num_jobs = num_threads + (rest_job > 0 ? 1 : 0) ;

        // 1. issue number of threads jobs
        {
            for( size_t i = 0; i < num_threads; ++i )
            {
                T end = start + per_job ;
                range_t lr( start, end ) ;

                job_ptr_t jptr = local_arena.alloc( job_t( lr, f ) ) ;

                so_thread::global::job_scheduler()->schedule( jptr, [&]( so_thread::ijob_ptr_t j )
                {
                    {
                        so_thread::lock_guard_t lk( mtx ) ;
                        --num_jobs ;
                    }
                    cv.notify_all() ;

                    local_arena.dealloc( static_cast<job_ptr_t>(j) ) ;

                } ) ;

                start = end ;
            }
        }

        // 2. if required jobs do not divide equally, issue rest job
        {
            if( rest_job > 0 )
            {
                T end = start + rest_job ;
                range_t lr( start, end ) ;

                job_ptr_t jptr = local_arena.alloc( job_t( lr, f ) ) ;

                so_thread::global::job_scheduler()->schedule( jptr, [&]( so_thread::ijob_ptr_t j )
                {
                    {
                        so_thread::lock_t lk( mtx ) ;
                        --num_jobs ;
                    }
                    cv.notify_all() ;

                    local_arena.dealloc( static_cast<job_ptr_t>( j ) ) ;

                } ) ;
            }
        }

        // 3. wait for all jobs to be finished
        {
            so_thread::lock_t lk( mtx ) ;
            while( num_jobs > 0 ) cv.wait( lk ) ;
        }
    }
}

#endif