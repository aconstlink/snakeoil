//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_BUFFER_VERTEX_BUFFER_LAYOUT_H_
#define _SNAKEOIL_GPU_BUFFER_VERTEX_BUFFER_LAYOUT_H_

#include "../enums/vertex_attributes.h"

#include <snakeoil/std/container/vector.hpp>

namespace so_gpu
{
    class vertex_buffer_layout
    {
        so_this_typedefs( vertex_buffer_layout ) ;
        
        typedef so_std::vector< so_gpu::vertex_attribute > layout_elements_t ;

    private:

        layout_elements_t _elements ;

    public:
        
        vertex_buffer_layout( void_t ) {}
        vertex_buffer_layout( this_rref_t rhv) :
            _elements(std::move(rhv._elements))
        {}
        
    public:

        this_ref_t add_layout_element( so_gpu::vertex_attribute vbl ) 
        {
            auto iter = std::find( _elements.begin(), _elements.end(), vbl ) ;
            if( iter != _elements.end() ) return *this ;

            _elements.push_back(vbl) ;
            return *this ;
        }

        size_t get_num_elements( void_t ) const { return _elements.size() ; }
        so_gpu::vertex_attribute operator[]( size_t index ) const { return _elements[index] ; }
    };
    so_typedef( vertex_buffer_layout ) ;
}

#endif

