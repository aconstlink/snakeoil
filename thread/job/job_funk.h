//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_THREAD_JOB_JOB_FUNK_H_
#define _SNAKEOIL_THREAD_JOB_JOB_FUNK_H_

#include "../typedefs.h"
#include "../range/range_1d.hpp"

#include <functional>

namespace so_thread
{
    template< typename T >
    using funk_t = std::function< void_t ( so_thread::range_1d<T> const & ) > ;
}

#endif