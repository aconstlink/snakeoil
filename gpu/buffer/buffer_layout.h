//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_BUFFER_LAYOUT_H_
#define _SNAKEOIL_GPU_BUFFER_LAYOUT_H_

#include "buffer_layout_element.h"


#include <snakeoil/log/global.h>
#include <snakeoil/core/assert.h>

#include <algorithm>

namespace so_gpu
{
    class buffer_layout
    {
        so_this_typedefs( buffer_layout ) ;

    public:

        typedef buffer_layout_element layout_element_t ;
        typedef layout_element_t const & layout_element_cref_t ;

        typedef so_std::vector< layout_element_t > layout_elements_t ;

    private:

        /// all layout elements of this buffer layout.
        layout_elements_t _elements ;
        
        /// size in bytes for all layout elements.
        /// very simple to be implemented and saves the ugly iteration 
        /// of the element collection.
        size_t _sib ;

    public:

        buffer_layout( void_t )
        {
            _sib = 0;
        }

        buffer_layout( this_rref_t rhv )
        {
            *this = std::move(rhv) ;
        }

        this_ref_t operator = ( this_rref_t rhv )
        {
            _elements = std::move( rhv._elements ) ;
            _sib = rhv._sib ;
            return *this ;
        }

    public:

        this_ref_t add_element( layout_element_cref_t element ) 
        { 
            _elements.push_back(element) ; 
            _sib += element.get_sib(); 
            return *this ;
        }

        this_ref_t remove_element( layout_element_cref_t element ) 
        {
            layout_elements_t::iterator iter = std::find( _elements.begin(), _elements.end(), element ) ;
            if( iter != _elements.end() ) _elements.erase(iter) ;
            return *this ;
        }

        this_ref_t remove_element( size_t index ) 
        {
            so_log::global::error_and_exit( _elements.size() > index, "Out of range." ) ;
            return this_t::remove_element(_elements[index]) ;
        }

    public:

        /// Returns the number of layout elements in this layout.
        size_t get_num_elements( void_t ) const { return _elements.size() ; }

        /// Returns the size in bytes of all layout elements of this layout.
        size_t get_sib( void_t ) const { return _sib; }

        layout_element_cref_t get_element( size_t index ) const{
            so_assert( _elements.size() > index ) ;
            return _elements[index] ;
        }

    public:

        bool_t operator == ( this_cref_t rhv ) const { return _elements == rhv._elements ; }


    };
}

#endif

