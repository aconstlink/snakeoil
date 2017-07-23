//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_MEMORY_GUARDS_MALLOC_GUARD_HPP_
#define _SNAKEOIL_MEMORY_GUARDS_MALLOC_GUARD_HPP_

#include "../typedefs.h"
#include <snakeoil/core/macros/move.h>

#include <cstdlib>

namespace so_memory
{
    /// designed to be used within tiny scopes for
    /// temp. string processing for example.
    /// allocates raw memory.
    /// does not call c/dtors.
    /// frees on destruction.
    template< typename type_in >
    class malloc_guard
    {
        so_this_typedefs( malloc_guard<type_in> ) ;
        so_typedefs( type_in, type ) ;

    private: // data

        type_t * _mem_ptr = nullptr ;
        size_t _num_elements = 0 ;

    private:
        
        malloc_guard( this_cref_t ){}

    public:

        malloc_guard( void_t )
        {}

        malloc_guard( size_t num_elements )
        {
            _mem_ptr = so_memory::memory::alloc_raw<type_t>(sizeof(type_t) * num_elements) ;
            _num_elements = num_elements ;
        }

        malloc_guard( type_cptr_t src_ptr, size_t num_elements )
        {
            _mem_ptr = so_memory::memory::alloc_raw<type_t>( sizeof( type_t ) * num_elements ) ;
            _num_elements = num_elements ;

            memcpy( _mem_ptr, src_ptr, num_elements ) ;
        }

        malloc_guard( this_rref_t rhv )
        {
            *this = std::move(rhv) ;
        }
        
        ~malloc_guard( void_t )
        {
            so_memory::memory::dealloc_raw( _mem_ptr ) ;
        }

    public: // operator 

        this_ref_t operator = ( this_rref_t rhv )
        {
            so_assert( _mem_ptr == nullptr ) ;
            so_move_member_ptr( _mem_ptr, rhv ) ;
            _num_elements = rhv._num_elements ;
            return *this ;
        }

        type_ref_t operator [] ( size_t i ) 
        {
            so_assert( _mem_ptr != nullptr && i < _num_elements ) ;
            return _mem_ptr[i] ;
        }

        type_cref_t operator [] ( size_t i ) const
        {
            so_assert( _mem_ptr != nullptr && i < _num_elements ) ;
            return _mem_ptr[i] ;
        }

        bool_t equals( size_t i, type_cref_t rhv ) const
        {
            so_assert( _mem_ptr != nullptr && i < _num_elements ) ;
            return _mem_ptr[i] == rhv ;
        }

        size_t size( void_t ) const 
        {
            return _num_elements ;
        }

    public:

        type_ptr_t get( void_t ) { return _mem_ptr ; }
        type_cptr_t get( void_t ) const { return _mem_ptr ; }

        operator type_ptr_t( void_t ){ return _mem_ptr ; }
        operator type_cptr_t( void_t ) const { return _mem_ptr ; }
    };
}

#endif

