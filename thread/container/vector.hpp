//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_THREAD_CONTAINER_VECTOR_HPP_
#define _SNAKEOIL_THREAD_CONTAINER_VECTOR_HPP_

#include "../typedefs.h"
#include "../mutex.h"

namespace so_thread
{
    template< typename T >
    class vector
    {
        so_this_typedefs( vector<T> ) ;

        so_typedefs( T, type ) ;
        so_typedefs( so_std::vector<T>, container ) ;

    private:

        mutable so_thread::mutex_t _mtx ;
        container_t _cont ;

    public:

        vector( void_t ) 
        {}

        vector( this_rref_t rhv )
        {
            so_thread::lock_guard_t lk(_mtx) ;
            _cont = std::move( rhv._cont ) ;
        }

        vector( this_cref_t rhv ) 
        {
            so_thread::lock_guard_t lk( _mtx ) ;
            _cont = rhv._cont ;
        }

        ~vector( void_t )
        {}

    public: 

        this_ref_t push_back( type_cref_t v )
        {
            so_thread::lock_guard_t lk( _mtx ) ;
            _cont.push_back( v ) ;

            return *this ;
        }

        size_t size( void_t ) const
        {
            so_thread::lock_guard_t lk( _mtx ) ;
            return _cont.size() ;
        }

        container_t move_out( void_t )
        {
            so_thread::lock_guard_t lk(_mtx) ;
            return std::move( _cont ) ;
        }
    };
}

#endif
