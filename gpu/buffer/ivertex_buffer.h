//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_BUFFER_IVERTEX_BUFFER_H_
#define _SNAKEOIL_GPU_BUFFER_IVERTEX_BUFFER_H_

#include "buffer.h"
#include "vertex_buffer_layout.h"

namespace so_gpu
{
    /// since the concrete vertex buffer is a template function,
    /// a run-time version is required too for easier handling 
    /// and type safety.
    class SNAKEOIL_GPU_API ivertex_buffer : public buffer
    {
    protected:

        ivertex_buffer( void_t ) {}
        ivertex_buffer( ivertex_buffer && rhv ) : buffer( std::move(rhv) ) {}

    public:

        virtual ~ivertex_buffer( void_t ) {}

    public:

        virtual size_t get_num_buffer_layout_elements( void_t ) const = 0 ;
        virtual vertex_buffer_layout const & get_vertex_buffer_layout( void_t ) const = 0 ;
        virtual vertex_attribute get_vertex_attribute( size_t index ) const = 0 ;
        virtual void_t destroy( void_t ) = 0 ;
    };
    so_typedef( ivertex_buffer ) ;
}

#endif

