//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_THREAD_PRIMITIVE_SEMAPHORE_GUARD_HPP_
#define _SNAKEOIL_THREAD_PRIMITIVE_SEMAPHORE_GUARD_HPP_

#include "semaphore.hpp"

namespace so_thread
{
    class semaphore_guard
    {
        so_this_typedefs( semaphore_guard ) ;

    private:

        semaphore_ref_t _s ;

    public:

        semaphore_guard( semaphore_ref_t s ) : _s(s)
        {
            _s.increment() ;
        }

        ~semaphore_guard( void_t )
        {
            _s.decrement() ;
        }

        semaphore_guard( this_cref_t ) = delete ;
        semaphore_guard( this_rref_t rhv ) = delete ;
    };
    so_typedef( semaphore_guard ) ;
}

#endif
