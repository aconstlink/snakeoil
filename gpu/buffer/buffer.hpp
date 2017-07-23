//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_BUFFER_HPP_
#define _SNAKEOIL_GPU_BUFFER_HPP_

#include "buffer_layout.h"
#include "../typedefs.h"

#include <snakeoil/std/container/vector.hpp>
#include <snakeoil/core/enums/type_traits.hpp>

namespace so_gpu
{
    template< typename element_t, typename alloc_t = so_memory::allocator< element_t > >
    class mem_buffer
    {
        typedef mem_buffer<element_t, alloc_t> __this_t ;
        so_this_typedefs( __this_t ) ;

        typedef element_t & element_ref_t ;
        typedef element_t const & element_cref_t ;

        typedef std::vector< element_t, alloc_t > elements_t ;
        
        typedef buffer_layout_element ble_t ;
        typedef ble_t const & ble_cref_t ;

    private:

        /// All elements in this buffer.
        elements_t _elements ;

        /// Determines what is in the _elements buffer.
        buffer_layout _layout ;

    public:

        mem_buffer( void_t ) : _elements(alloc_t("[so_memory] : buffer"))
        {}

        mem_buffer( alloc_t const & alloc ) : _elements(alloc)
        {}

        mem_buffer( this_rref_t rhv ) :
            _elements( std::move( rhv._elements ), alloc_t("[so_memory] : buffer") ),
            _layout( std::move( rhv._layout ) )
        {}
        
        mem_buffer( this_rref_t rhv, alloc_t const & alloc ) :
            _elements( std::move( rhv._elements ), alloc ),
            _layout( std::move( rhv._layout ) )
        {}

        ~mem_buffer( void_t )
        {}

        this_ref_t operator = ( this_rref_t rhv )
        {
            _elements = std::move( rhv._elements ) ;
            _layout = std::move( rhv._layout ) ;
            return *this ;
        }

    public:

        /// Returns the number of elements in this buffer.
        size_t size( void_t ) const { return _elements.size() ; }

        /// Returns the size in bytes for the whole buffer.
        size_t get_sib( void_t ) const {return _layout.get_sib()*size();} 

        /// Returns the size in bytes for one element.
        size_t get_element_sib( void_t ) const {return _layout.get_sib();} 

        /// Returns the raw void pointer to the internal buffer.
        void_cptr_t get_raw_pointer( void_t ) const 
        {
            return _elements.size() == 0 ? nullptr : 
                reinterpret_cast<void_cptr_t>(&_elements[0]);
        }
        
        void_cptr_t get_raw_pointer( size_t begin_index ) const 
        {
            return _elements.size() == 0 ? nullptr : 
                reinterpret_cast<void_cptr_t>(&_elements[begin_index]);
        }

        void_ptr_t get_raw_pointer( void_t ) 
        {
            return _elements.size() == 0 ? nullptr : 
                reinterpret_cast<void_ptr_t>(&_elements[0]);
        }

        /// Return the buffer layout.
        buffer_layout const & get_buffer_layout( void_t ) const { return _layout; }
        
    public:

        /// Reserves the given number of elements. The size is not chagned by that operation.
        void_t reserve( size_t num_elements )
        {
            _elements.reserve( num_elements ) ;
        }

        /// Resizes to the gives number of elements. The size of the internal buffer is changed and
        /// there will be num_elements new default constructed elements in the buffer.
        void_t resize( size_t num_elements )
        {
            _elements.resize( num_elements ) ;
        }

        void_t clear( void_t )
        {
            _elements.clear() ;
        }       

        element_ref_t operator[]( size_t index )
        {
            return _elements[index] ;
        }

        element_cref_t operator[]( size_t index ) const
        {
            return _elements[index] ;
        }

        /// Puts an element to the back of the internal buffer.
        this_ref_t add_element( element_cref_t elem )
        {
            _elements.push_back(elem) ;
            return *this ;
        }

        /// Puts an element to the back of the internal buffer.
        this_ref_t set_element( size_t index, element_cref_t elem )
        {
            _elements[index] = elem ;
            return *this ;
        }

        void_t set_element2( size_t index, element_cref_t elem )
        {
            _elements[index] = elem ;
        }

        void_t copy_elements( size_t self_start, size_t start_element, size_t num_elements, element_t * elements )
        {
            element_t * b = &(elements[start_element]) ;
            element_t * this_buffer = &(_elements[self_start]) ;

            for( size_t i=0; i<num_elements; ++i )
            {
                this_buffer[i] = b[i] ;
            }
        }

    public: // layout

        this_ref_t add_layout_element( so_core::type t, so_core::type_structure ts )
        {
            return add_layout_element( ble_t(t, ts) ) ;
        }

        template< typename type_t >
        this_ref_t add_layout_element( void_t )
        {
            using so_core::type_traits ;
            return add_layout_element( ble_t(type_traits<type_t>::core_type, type_traits<type_t>::core_type_structure) ) ;
        }

    private:

        /// Puts a layout element to the back of the internal layout.
        this_ref_t add_layout_element( ble_cref_t elem )
        {
            _layout.add_element(elem) ;
            return *this ;
        }
    };
}

#endif

