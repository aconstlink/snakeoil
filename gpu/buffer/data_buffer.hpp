//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_BUFFER_DATA_BUFFER_H_
#define _SNAKEOIL_GPU_BUFFER_DATA_BUFFER_H_

#include "idata_buffer.h"
#include "buffer.hpp"

#include "../enums/type_traits.h"
#include "../converter/convert_core_gfx_type.h"

namespace so_gpu
{
    /// data buffer:
    /// only supports vec4 types!
    template< typename data_t >
    class data_buffer : public so_gpu::idata_buffer
    {
        typedef so_gpu::idata_buffer base_t ;
        so_this_typedefs( data_buffer<data_t> ) ;

        typedef so_gpu::allocator<data_t> alloc_t ;
        typedef so_gpu::mem_buffer< data_t, alloc_t > elements_t ;

    private:

        elements_t _buffer ;

    public:

        data_buffer( void_t ) : _buffer( alloc_t("[so_gpu] : data_buffer") )
        {
            base_t::set_data_buffer_format( so_gpu::type_traits<data_t>::gfx_data_buffer_format ) ;
            
            auto t = so_gpu::type_traits<data_t>::gfx_type ;
            auto ts = so_gpu::type_traits<data_t>::gfx_type_struct ;
            _buffer.add_layout_element(so_gpu::convert_from(t), so_gpu::convert_from(ts)) ;
        }

        data_buffer( size_t const num_elements ) : _buffer( alloc_t( "[so_gpu] : data_buffer" ) )
        {
            base_t::set_data_buffer_format( so_gpu::type_traits<data_t>::gfx_data_buffer_format ) ;

            auto t = so_gpu::type_traits<data_t>::gfx_type ;
            auto ts = so_gpu::type_traits<data_t>::gfx_type_struct ;
            _buffer.add_layout_element( so_gpu::convert_from( t ), so_gpu::convert_from( ts ) ) ;
            _buffer.resize( num_elements ) ;
        }

        data_buffer( data_buffer && rhv ) : idata_buffer( std::move(rhv) ),
            _buffer( std::move(rhv._buffer), alloc_t("[so_gpu] : data_buffer"))
        {
        }

        virtual ~data_buffer( void_t ) {}

    public:

        static this_ptr_t create( std::string const & purpose ) 
        {
            return this_t::create( this_t(), purpose ) ;
        }

        static this_ptr_t create( this_rref_t rhv, std::string const & purpose )
        {
            return so_gpu::memory::alloc( std::move(rhv), purpose ) ;
        }

        static void_t destroy( this_ptr_t ptr ) 
        {
            so_gpu::memory::dealloc( ptr ) ;
        }


    public:

        this_ref_t add_element( data_t const & data )
        {
            _buffer.add_element( data ) ;
            return *this ;
        }

        this_ref_t set_element( size_t i, data_t const & data )
        {
            _buffer.set_element( i, data ) ;
            return *this ;
        }

        data_t get_element( size_t i ) const
        {
            return _buffer[i] ;
        }

        this_ref_t resize( size_t const num_elements )
        {
            _buffer.resize( num_elements ) ;
            return *this ;
        }

        this_ref_t reserve( size_t const num_elements )
        {
            _buffer.reserve( num_elements ) ;
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
            return _buffer.get_raw_pointer() ;
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

        virtual void_t destroy( void_t )
        {
            this_t::destroy( this ) ;
        }
    };

    so_typedefs( data_buffer<so_math::vec4f_t>, data_buffer_vec4f ) ;
}

#endif

