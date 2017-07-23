//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_BUFFER_BUFFER_H_
#define _SNAKEOIL_GPU_BUFFER_BUFFER_H_

#include "../object.h"
#include "buffer_layout.h"

namespace so_gpu
{
    class SNAKEOIL_GPU_API buffer : public so_gpu::object
    {
        typedef so_gpu::object base_t ;

        so_this_typedefs( buffer ) ;

    public:

        typedef so_gpu::buffer_layout buffer_layout_t ;
        typedef buffer_layout_t const & buffer_layout_cref_t ;
        typedef buffer_layout_t::layout_element_t layout_element_t ;
        typedef layout_element_t const & layout_element_cref_t ;

    public:

        buffer( void_t ) ;
        buffer( this_rref_t rhv ) ;
        virtual ~buffer( void_t ) ;
        //this_ref_t operator = ( this_rref_t rhv ) {}

    public: // interface

        virtual size_t get_num_elements( void_t ) const = 0 ;
        virtual size_t get_sib( void_t ) const = 0 ;
        virtual void_cptr_t get_raw_pointer( void_t ) const = 0 ;
        virtual void_cptr_t get_raw_pointer( size_t start_index ) const = 0 ;
        
        virtual buffer_layout_cref_t get_buffer_layout( void_t ) const = 0 ;
        virtual layout_element_cref_t get_buffer_layout_element( size_t index ) const = 0 ;
        virtual void_t destroy( void_t ) = 0 ;
    };
    so_typedef( buffer ) ;
}

#endif

