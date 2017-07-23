//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_THREAD_PRIMITIVE_SEMAPHORE_HPP_
#define _SNAKEOIL_THREAD_PRIMITIVE_SEMAPHORE_HPP_

#include "typedefs.h"
#include "api.h"
#include "result.h"

#include <snakeoil/core/assert.h>

namespace so_thread
{
    class SNAKEOIL_THREAD_API semaphore
    {
        so_this_typedefs( semaphore ) ;

    public:

        typedef std::function< bool_t ( size_t ) > comp_funk_t ;

    private:

        so_thread::mutex_t _mtx ;
        so_thread::condition_variable_t _cv ;

        size_t _count = 0 ;        

    public: // ctors

        semaphore( void_t ) : _count(0)
        {}

        semaphore( this_rref_t rhv )
        {
            *this = std::move(rhv) ;
        }

        semaphore( size_t c ) : _count(c)
        {}

        ~semaphore( void_t )
        {
            so_assert( _count == 0 ) ;
        }

    public: // operators

        this_ref_t operator = ( this_rref_t rhv ) 
        {
            _count = rhv._count ;
            return *this ;
        }

        bool_t operator <= ( size_t c )
        {
            return _count <= c ;
        }

        bool_t operator > (size_t c)
        {
            so_thread::lock_guard_t lk( _mtx ) ;
            return _count > c ;
        }

        bool_t operator >= ( size_t c )
        {
            so_thread::lock_guard_t lk( _mtx ) ;
            return _count >= c ;
        }

        bool_t operator == ( size_t c )
        {
            return _count == c ;
        }

    public:

        bool_t increment( void_t )
        {
            so_thread::lock_guard_t lk( _mtx ) ;

            ++_count ;

            return true ;
        }

        bool_t increment( size_t const max_count )
        {
            so_thread::lock_guard_t lk( _mtx ) ;

            bool_t const hit_max = _count < max_count ;
            _count = hit_max ? _count+1 : _count ;

            return true ;
        }

        bool_t decrement( void_t ) 
        {            
            so_thread::lock_guard_t lk( _mtx ) ;
            
            if( _count == 0 )
                return false ;

            --_count ;
            
            if( _count == 0 )
                _cv.notify_all() ;

            return true ;
        }       
        
        bool_t decrement( comp_funk_t funk )
        {
            so_thread::lock_guard_t lk( _mtx ) ;

            if(_count == 0)
                return false ;

            --_count ;

            if(_count == 0)
                _cv.notify_all() ;

            return funk( _count ) ;
        }
        
        /// wait until semaphore becomes 0
        void_t wait( void_t )
        {
            so_thread::lock_t lk( _mtx ) ;
            while( _count > 0 ) _cv.wait( lk ) ;
        }
    };
    so_typedef( semaphore ) ;
}

#endif

