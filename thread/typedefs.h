//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_THREAD_TYPEDEFS_H_
#define _SNAKEOIL_THREAD_TYPEDEFS_H_

#include "protos.h"
#include "mutex.h"

#include <snakeoil/memory/global.h>
#include <snakeoil/memory/allocator.hpp>

#include <snakeoil/std/string/string.hpp>
#include <snakeoil/std/container/map.hpp>
#include <snakeoil/std/container/vector.hpp>

#include <snakeoil/core/types.hpp>
#include <snakeoil/core/macros/typedef.h>
#include <snakeoil/core/macros/move.h>
#include <snakeoil/core/boolean.h>
#include <snakeoil/core/checks.h>

#include <thread>
#include <functional>


namespace so_thread
{
    using namespace so_core::so_types ;

    so_typedefs( std::thread, thread ) ;

    using memory = so_memory::global ;

    template< typename T >
    using allocator = so_memory::allocator<T, so_thread::memory > ;

    typedef std::function< void_t ( void_t ) > void_funk_t ;

    namespace so_internal
    {
        typedef so_std::map< so_std::string, itask_ptr_t > task_map_t ;
    }

    so_typedefs( so_internal::task_map_t, task_map ) ;
}

#endif

