//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_THREAD_MUTEX_H_
#define _SNAKEOIL_THREAD_MUTEX_H_

#include <mutex>
#include <condition_variable>

namespace so_thread
{
    typedef std::mutex mutex_t ;
    typedef std::lock_guard< mutex_t > lock_guard_t ;
    typedef std::unique_lock< mutex_t > lock_t ;
    typedef std::condition_variable condition_variable_t ;
}

#endif
