//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_VARIABLE_DATA_BUFFER_VARIABLE_HPP_
#define _SNAKEOIL_GPU_VARIABLE_DATA_BUFFER_VARIABLE_HPP_

#include "variable.h"

#include "../typedefs.h"
#include "../protos.h"

#include "../enums/type_traits.h"
#include "../texture/texture_2d.h"

namespace so_gpu
{        
    // just an interface for simplifying things.
    // used for all data buffer variables being able to connect
    // to shaders.
    class idata_buffer_variable : public so_gpu::variable
    {
    public:

        idata_buffer_variable( void_t ){}
        idata_buffer_variable( idata_buffer_variable && rhv ) : variable(std::move(rhv) ){}
        virtual ~idata_buffer_variable( void_t ) {}

    public:

        virtual so_gpu::idata_buffer_ptr_t get_buffer( void_t ) = 0 ;
        virtual so_gpu::idata_buffer_cptr_t get_buffer( void_t ) const = 0 ;
    } ;

    /// the image template parameter is required for deducing the run-time
    /// shader variable type up front. This is also coherent with
    /// the data_variable api.
    template< typename type_t >
    class data_buffer_variable : public idata_buffer_variable
    {
        typedef idata_buffer_variable base_t ;
        so_this_typedefs( data_buffer_variable< type_t > ) ;       

    private:

        idata_buffer_ptr_t _buf_ptr = nullptr ;
        bool_t _has_changed = true ;

    public:

        data_buffer_variable( void_t ) 
        {
            base_t::set_shader_variable_type(
                so_gpu::from_type_to_data_buffer_shadar_type(
                    so_gpu::type_traits<type_t>::gfx_type ) 
                    ) ;
        }
        
        data_buffer_variable( idata_buffer_ptr_t ptr ) 
        {
            _buf_ptr = ptr ;
            base_t::set_shader_variable_type(
                so_gpu::from_type_to_data_buffer_shadar_type(
                    so_gpu::type_traits<type_t>::gfx_type ) 
                    ) ;
        }
        
        data_buffer_variable( this_rref_t rhv ) : base_t( std::move( rhv) )
        {
            _buf_ptr = rhv._buf_ptr ;
            rhv._buf_ptr = nullptr ;
            _has_changed = rhv._has_changed ;
        }

        static this_ptr_t create( void_t )
        {
            return so_gpu::memory::alloc( this_t(), "[data_buffer_variable<T>::create]" ) ;
        }

        static void_t destroy( this_ptr_t ptr )
        {
            so_gpu::memory::dealloc( ptr ) ;
        }

    public:

        void_t set( idata_buffer_ptr_t ptr ) 
        {
            _buf_ptr = ptr ;
            _has_changed = true ;
        }
        idata_buffer_cptr_t get( void_t ) const 
        {
            return _buf_ptr ;
        }
        idata_buffer_ptr_t get( void_t ) 
        {
            return _buf_ptr ;
        }

    public:

        bool_t has_changed( void_t ) const 
        {
            return _has_changed ;
        }
        void_t set_changed( bool_t b) 
        {
            _has_changed = b ;
        }

    public: // interface

        virtual so_gpu::idata_buffer_ptr_t get_buffer( void_t )
        {
            return _buf_ptr ;
        }

        virtual so_gpu::idata_buffer_cptr_t get_buffer( void_t ) const
        {
            return _buf_ptr ;
        }

        virtual void_t destroy( void_t ) 
        {
            this_t::destroy( this ) ;
        }

    };
}

#endif

