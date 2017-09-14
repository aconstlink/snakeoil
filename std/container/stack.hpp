//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_STD_CONTAINER_STACK_HPP_
#define _SNAKEOIL_STD_CONTAINER_STACK_HPP_

#include <snakeoil/core/types.hpp>
#include <snakeoil/core/assert.h>
#include <snakeoil/core/macros/typedef.h>

#include <vector>
#include <functional>

namespace so_std
{
    using namespace so_core::so_types ;

    /// not thread safe stack
    template< typename O, size_t pre_alloc >
    class stack
    {
        typedef so_std::stack< O, pre_alloc > this_stack_t ;
        so_this_typedefs( this_stack_t ) ;
        
        so_typedefs( O, object ) ;
        typedef std::vector< object_t > stack_t ;
        
        typedef std::function< void_t( object_cref_t, bool_t const, bool_t const ) > for_each_funk_t ;

    private:

        std::vector< object_t > _stack ;

        /// one above top of stack
        /// top of stack is last pushed
        size_t _cur_pos = 0 ;

    private:

        /// checks the size of the stack and resizes
        /// by pre_alloc
        void_t check_and_resize( void_t )
        {
            if( _cur_pos < _stack.size() ) return ; 
            _stack.resize( _stack.size() + pre_alloc ) ;
        }

    public:

        stack( void_t )
        {
            _stack.resize(pre_alloc) ;
        }

        stack( this_cref_t rhv )
        {
            *this = rhv ;
        }

        stack( this_rref_t rhv )
        {
            *this = std::move(rhv) ;
        }

        this_ref_t operator = ( this_cref_t rhv )
        {
            _cur_pos = rhv._cur_pos ;
            _stack = rhv._stack ;
            return *this ;
        }

        this_ref_t operator = ( this_rref_t rhv )
        {
            _cur_pos = rhv._cur_pos ;
            rhv._cur_pos = 0 ;
            _stack = std::move( rhv._stack ) ;
            return *this ;
        }

    public:
        
        void_t push( object_cref_t rhv )
        {
            check_and_resize() ;
            _stack[_cur_pos] = rhv ;
            ++_cur_pos ;
        }

        object_rref_t pop( void_t )
        {
            so_assert( _cur_pos > 0 ) ;
            --_cur_pos ;
            return std::move(_stack[_cur_pos]) ;
        }

        void_t pop( object_ref_t obj_out )
        {
            so_assert( _cur_pos > 0 ) ;
            --_cur_pos ;
            obj_out = std::move(_stack[_cur_pos]) ;
        }

        size_t size( void_t ) const 
        { 
            return _cur_pos ; 
        }

        bool_t has_item( void_t ) const
        {
            return this_t::size() != 0 ;
        }

        object_cref_t top( void_t ) const 
        {
            so_assert( _cur_pos != 0 ) ;
            return _stack[_cur_pos-1] ;
        }

        bool_t top( object_ref_t out_ ) const 
        {
            if( _cur_pos == 0 ) return false ;
            out_ = top() ;
            return true ;
        }

    public:

        void_t bottom_to_top( for_each_funk_t funk )
        {
            if( _cur_pos == 0 ) return ;

            for( size_t i=0; i<_cur_pos; ++i )
            {
                funk( _stack[i], i==0, i==_cur_pos-1 ) ;
            }
        }

        void_t top_to_bottom( for_each_funk_t funk )
        {
            if( _cur_pos == 0 ) return ;

            for( size_t i=_cur_pos-1; i!=size_t(-1); --i )
            {
                funk( _stack[i], i == 0, i == _cur_pos - 1 ) ;
            }
        }
    };
}

#endif

