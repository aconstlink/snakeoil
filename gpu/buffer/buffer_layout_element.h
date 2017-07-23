//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_BUFFER_LAYOUT_ELEMENT_H_
#define _SNAKEOIL_GPU_BUFFER_LAYOUT_ELEMENT_H_

#include "../typedefs.h"

#include <snakeoil/core/enums/types.h>

namespace so_gpu
{
    /// This object represents a single element in a buffer.
    /// A buffer is composed of multiple object described by a buffer layout.
    /// @see buffer_layout
    /// @see buffer
    class buffer_layout_element
    {
        typedef buffer_layout_element this_t ;
        typedef this_t const & this_cref_t ;
        typedef this_t & this_ref_t ;

    private:

        so_core::type _t = so_core::type::undefined ;
        so_core::type_structure _ts = so_core::type_structure::undefined ;
        
    public:

        buffer_layout_element( void_t ) {}

        buffer_layout_element( so_core::type t, so_core::type_structure ts )
        {
            _t = t ;
            _ts = ts ;
        }

        buffer_layout_element( this_cref_t rhv ) 
        {
            *this = rhv ;
        }

        this_ref_t operator=( this_cref_t rhv )
        {
            _t = rhv._t ;
            _ts = rhv._ts ;
            return *this ;
        }

        bool_t operator==( this_cref_t rhv ) const
        {
            return rhv._t == _t && rhv._ts == _ts ;
        }

    public:

        so_core::type get_type( void_t ) const {return _t;}
        so_core::type_structure get_type_structure( void_t ) const {return _ts;}

        /// Returns the size in bytes of this element.
        size_t get_sib( void_t ) const { return this_t::calc_sib(); }

    private:

        /// calculates the size in bytes of this element
        size_t calc_sib( void_t ) const{ return so_core::size_of(_t) * so_core::size_of(_ts); }
    };
}

#endif

