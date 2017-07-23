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

        semaphore_ptr_t _s ;

    public:

        semaphore_guard( semaphore_ptr_t s ) : _s(s)
        {
            _s->increment() ;
        }

        semaphore_guard( this_rref_t rhv )
        {
            *this = std::move(rhv) ;
        }

        ~semaphore_guard( void_t )
        {
            _s->decrement() ;
        }
    
    public:

        this_ref_t operator = ( this_rref_t rhv )
        {
            so_move_member_ptr( _s, rhv ) ; 
            return *this ;
        }
    };
    so_typedef( semaphore_guard ) ;
}

#endif
