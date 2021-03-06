//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_MEMORY_MEMORY_H_
#define _SNAKEOIL_MEMORY_MEMORY_H_

#include "typedefs.h"
#include "protos.h"

#include "so_api.h"

#include <functional>
#include <mutex>

namespace so_memory
{
    class SNAKEOIL_MEMORY_API memory
    {
        typedef memory this_t ;
        
    private:

        static manager_ptr_t _manager ;

    public:

        static void init( void_t ) ;
        static void init( manager_ptr_t ptr ) ;
        static void dump_to_std( void_t ) ;

        template< typename T >
        static T * alloc( void_t )
        {
            void_ptr_t ptr = this_t::alloc( sizeof(T) ) ;
            return new(ptr)T() ;
        }

        template< typename T >
        static T * alloc( so_memory::purpose_cref_t purpose )
        {
            void_ptr_t ptr = this_t::alloc( sizeof(T), purpose ) ;
            return new(ptr)T() ;
        }

        template< typename T >
        static T * alloc( T const & acopy )
        {
            void_ptr_t ptr = this_t::alloc( sizeof(T) ) ;
            return new(ptr)T(acopy) ;
        }

        template< typename T >
        static T * alloc( T const & acopy, so_memory::purpose_cref_t purpose )
        {
            void_ptr_t ptr = this_t::alloc( sizeof(T), purpose ) ;
            return new(ptr)T(acopy) ;
        }

        template< typename T >
        static T * alloc( T && amove )
        {
            void_ptr_t ptr = this_t::alloc( sizeof(T) ) ;
            return new(ptr)T(std::move(amove)) ;
        }

        template< typename T >
        static T * alloc( T && amove, so_memory::purpose_cref_t purpose )
        {
            void_ptr_t ptr = this_t::alloc( sizeof(T), purpose ) ;
            return new(ptr)T(std::move(amove)) ;
        }

        template< typename T >
        static void_t dealloc( T * ptr )
        {
            if( ptr == nullptr ) return ;
            (*ptr).~T() ;
            this_t::dealloc( reinterpret_cast<void_ptr_t>(ptr) ) ;
        }

        template< typename T >
        static T * alloc( size_t n )
        {
            T* ptr = this_t::alloc_raw<T>(n) ;
            for( size_t i=0; i<n; ++i ) new(ptr+i)T() ;
            return ptr ;
        }

        template< typename T >
        static T * alloc( size_t n, so_memory::purpose_cref_t purpose )
        {
            T* ptr = this_t::alloc_raw<T>(n, purpose) ;
            for( size_t i=0; i<n; ++i ) new(ptr+i)T() ;
            return ptr ;
        }

        template< typename T >
        static T * alloc_raw( size_t n )
        {
            return reinterpret_cast<T*>( this_t::alloc( sizeof(T)*n ) ) ;
        }

        template< typename T >
        static T * alloc_raw( size_t n, so_memory::purpose_cref_t purpose )
        {
            return reinterpret_cast<T*>( this_t::alloc( sizeof(T)*n, purpose ) ) ;
        }

        template< typename T >
        static void_t dealloc( T* ptr, size_t n )
        {
            if( ptr == nullptr ) return ;
            for( size_t i=0; i<n; ++i ) (*(ptr+1)).~T() ;
            this_t::dealloc_raw<T>(ptr) ;
        }

        template< typename T >
        static void_t dealloc_raw( T* ptr )
        {
            this_t::dealloc( reinterpret_cast<void_ptr_t>(ptr) ) ;
        }

        static void_ptr_t alloc( size_t sib, so_memory::purpose_cref_t purpose ) ;
        static void_ptr_t alloc( size_t sib ) ;
        static void_t dealloc( void_ptr_t ) ;
        static size_t get_sib( void_t ) ;

        static bool_t get_purpose( void_ptr_t, so_memory::purpose_ref_t ) ;

    public:

        template< typename T >
        static T* address( T & ref )
        {
            return &ref ;
        }

        template< typename T >
        static T const * address( T const & ref )
        {
            return &ref ;
        }

    };
}

#endif

