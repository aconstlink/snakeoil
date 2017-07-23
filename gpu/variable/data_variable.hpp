//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_VARIABLE_DATA_VARIABLE_HPP_
#define _SNAKEOIL_GPU_VARIABLE_DATA_VARIABLE_HPP_

#include "variable.h"

#include "../enums/type_traits.h"

namespace so_gpu
{
    template< typename type_t >
    class data_variable : public so_gpu::variable
    {
        typedef so_gpu::variable base_t ;
        so_this_typedefs(data_variable<type_t>) ;

    private:

        type_t * _ptr ;

    public:

        static SNAKEOIL_GPU_API this_ptr_t create( void_t ) 
        {
            return this_t::create( this_t(), "[so_gpu::texture_variable::create]" ) ;
        }

        static SNAKEOIL_GPU_API this_ptr_t create( this_rref_t rhv, std::string const & purpose ) 
        {
            return so_gpu::memory::alloc(std::move(rhv), purpose ) ;
        }

        static SNAKEOIL_GPU_API void_t destroy( this_ptr_t ptr ) 
        {
            so_gpu::memory::dealloc( ptr ) ;
        }

    public:

        data_variable( void_t ) : 
            variable( so_gpu::type_traits<type_t>::gfx_shader_variable_type )
        {
            _ptr = nullptr ;
        }

        data_variable( type_t * ptr ) : 
            variable( so_gpu::type_traits<type_t>::gfx_shader_variable_type  )
        {
            _ptr = ptr ;
        }

        data_variable( data_variable<type_t> && rhv ) : variable( std::move( rhv ) )
        {
            _ptr = rhv._ptr ;
            rhv._ptr = nullptr ;
        }        

    public:

        void_t set( type_t const & value )
        {
            so_assert( _ptr != nullptr ) ;
            *_ptr = value ;
        }

        type_t const & get( void_t ) const 
        {
            so_assert( _ptr != nullptr ) ;
            return *_ptr ;
        }

    private:

        virtual void_t destroy( void_t )
        {
            this_t::destroy( this ) ;
        }
    };
    
    typedef data_variable< float_t > float_variable_t ;
    typedef data_variable< int_t > int_variable_t ;
    typedef data_variable< uint_t > uint_variable_t ;
    
}

#endif

