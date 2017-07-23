//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_BUFFER_INDEX_BUFFER_H_
#define _SNAKEOIL_GPU_BUFFER_INDEX_BUFFER_H_

#include "iindex_buffer.h"
#include "../enums/type_traits.h"
#include "../converter/convert_core_gfx_type.h"
#include "buffer.hpp"

namespace so_gpu
{
    template< typename index_t >
    class index_buffer : public iindex_buffer
    {
        so_this_typedefs( index_buffer<index_t> ) ;

        typedef so_gpu::allocator<index_t> alloc_t ;
        typedef so_gpu::mem_buffer< index_t, alloc_t > elements_t ;

    private:

        elements_t _buffer ;

    public:

        index_buffer( void_t ) : _buffer( alloc_t("[so_gpu] : index_buffer") )
        {
            auto t = so_gpu::type_traits<index_t>::gfx_type ;
            auto ts = so_gpu::type_traits<index_t>::gfx_type_struct ;
            _buffer.add_layout_element(so_gpu::convert_from(t), so_gpu::convert_from(ts)) ;
        }

        index_buffer( this_rref_t rhv ) : iindex_buffer( std::move(rhv) ), 
            _buffer( std::move(rhv._buffer), alloc_t("[so_gpu] : index_buffer") )
        {
        }

        virtual ~index_buffer( void_t ) {}

    public: // create/destroy

        static this_ptr_t create( this_rref_t rhv, std::string const & purpose )
        {
            return so_gpu::memory::alloc( std::move(rhv), purpose ) ;
        }

        static this_ptr_t create( std::string const & purpose )
        {
            return so_gpu::memory::alloc( this_t(), purpose ) ;
        }

        static this_ptr_t create( void_t )
        {
            return this_t::create( this_t() ) ;
        }

        static this_ptr_t create( this_rref_t rhv )
        {
            return this_t::create( this_t(), "[so_gpu::index_buffer::create]" ) ;
        }       

        static void_t destroy( this_ptr_t ptr )
        {
            so_gpu::memory::dealloc( ptr ) ;
        }

    public:

        void_t resize( size_t num_elements )
        {
            _buffer.resize( num_elements ) ;
        }

        this_ref_t reserve( size_t num_elements )
        {
            _buffer.reserve( num_elements ) ;
            return *this ;
        }

        void_t set_index( size_t pos, index_t index )
        {
            _buffer[pos] = index ;
        }

        index_t get_index( size_t pos ) const
        {
            return _buffer[pos] ;
        }

        this_ref_t add_index( index_t index ) 
        {
            _buffer.add_element( index ) ;
            return *this ;
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

        virtual void_t destroy( void_t )
        {
            this_t::destroy( this ) ;
        }

    };
}

#endif

