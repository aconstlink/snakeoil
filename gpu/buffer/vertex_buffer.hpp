//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_BUFFER_VERTEX_BUFFER_H_
#define _SNAKEOIL_GPU_BUFFER_VERTEX_BUFFER_H_

#include "ivertex_buffer.h"

#include "ivertex_buffer.h"
#include "vertex_buffer_layout.h"
#include "../enums/type_traits.h"
#include "../converter/convert_core_gfx_type.h"
#include "buffer.hpp"

namespace so_gpu
{
    template< typename vertex_t >
    class vertex_buffer : public so_gpu::ivertex_buffer
    {
        typedef so_gpu::buffer base_t ;

        so_this_typedefs( vertex_buffer<vertex_t> ) ;

        typedef so_gpu::allocator<vertex_t> alloc_t ;
        typedef so_gpu::mem_buffer< vertex_t, alloc_t > elements_t ;

        typedef vertex_t const & vertex_cref_t ;

    private:

        vertex_buffer_layout _layout ;
        elements_t _buffer ;

    public:

        vertex_buffer( void_t ) : _buffer(alloc_t("[so_gpu] : vertex_buffer") )
        {}

        vertex_buffer( this_rref_t rhv ) : ivertex_buffer( std::move(rhv) ),
            _layout( std::move( rhv._layout ) ),
            _buffer( std::move(rhv._buffer), alloc_t("[so_gpu] : vertex_buffer" ) )
        {}

        virtual ~vertex_buffer( void_t ) {}
        
    public:

        static this_ptr_t create( void_t )
        {
            return this_t::create( this_t(), "[vertex_buffer::create]" ) ;
        }

        static this_ptr_t create( this_rref_t rhv, std::string const & purpose )
        {
            return so_gpu::memory::alloc( std::move(rhv), purpose ) ;
        }

        static this_ptr_t create( std::string const & purpose )
        {
            return this_t::create( this_t(), purpose ) ;
        }

        static void_t destroy( this_ptr_t ptr )
        {
            so_gpu::memory::dealloc( ptr ) ;
        }

    public: // interface

        virtual size_t get_num_elements( void_t ) const 
        { 
            return _buffer.size() ; 
        }

        virtual size_t get_sib( void_t ) const 
        { 
            return _buffer.get_sib() ; 
        } 

        virtual void_cptr_t get_raw_pointer( void_t ) const 
        { 
            return _buffer.get_raw_pointer() ; 
        }

        virtual void_cptr_t get_raw_pointer( size_t start_index ) const 
        { 
            return _buffer.get_raw_pointer(start_index) ; 
        }

        virtual buffer_layout_cref_t get_buffer_layout( void_t ) const 
        { 
            return _buffer.get_buffer_layout() ; 
        }

        virtual layout_element_cref_t get_buffer_layout_element( size_t index ) const
        {
            return _buffer.get_buffer_layout().get_element(index) ;
        }

    public:

        this_ref_t resize( size_t num_elements )
        {
            _buffer.resize( num_elements ) ; 
            return *this ;
        }

        this_ref_t reserve( size_t num_elements )
        {
            _buffer.reserve( num_elements ) ; 
            return *this ;
        }

        this_ref_t add_vertex( vertex_cref_t element )
        {
            _buffer.add_element( element ) ;
            return *this ;
        }

        this_ref_t set_vertex( size_t index, vertex_cref_t element )
        {
            _buffer[index] = element ;
            return *this ;
        }

        vertex_cref_t get_vertex( size_t index ) 
        {
            return _buffer[index] ;
        }

        template< typename type_t >
        this_ref_t add_layout_element( so_gpu::vertex_attribute va  )
        {
            return add_layout_element( va, 
                so_gpu::type_traits<type_t>::gfx_type, 
                so_gpu::type_traits<type_t>::gfx_type_struct ) ;
        }

        this_ref_t add_layout_element( so_gpu::vertex_attribute va, so_gpu::type t, so_gpu::type_struct ts  )
        {
            _buffer.add_layout_element(so_gpu::convert_from(t), so_gpu::convert_from(ts)) ;
           _layout.add_layout_element(va) ;
            return *this ;
        }

    public: // ivertex_buffer
        
        virtual size_t get_num_buffer_layout_elements( void_t ) const
        {
            return _layout.get_num_elements() ;
        }

        virtual vertex_buffer_layout const & get_vertex_buffer_layout( void_t ) const
        {
            return _layout ;
        }

        virtual vertex_attribute get_vertex_attribute( size_t index ) const
        {
            return _layout[index] ;
        }

        virtual void_t destroy( void_t ) 
        {
            this_t::destroy( this ) ;
        }

    };
}

#endif

