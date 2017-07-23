//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_SHADER_VERTEX_SHADER_H_
#define _SNAKEOIL_GPU_SHADER_VERTEX_SHADER_H_

#include "shader.h"

#include "../enums/primitive_types.h"

namespace so_gpu
{
    class SNAKEOIL_GPU_API vertex_shader final : public so_gpu::shader
    {
        typedef so_gpu::shader base_t ;
        so_this_typedefs( vertex_shader ) ;

    private:

        /// the output type for transform feedback
        primitive_type _pt = primitive_type::undefined ;

    public:

        static this_ptr_t create( so_memory::purpose_cref_t p ) 
        {
            return so_gpu::memory::alloc( this_t(), p ) ;
        }

        static this_ptr_t create( this_rref_t rhv, so_memory::purpose_cref_t p )
        {
            return so_gpu::memory::alloc( std::move(rhv), p ) ;
        }

        static void_t destroy( this_ptr_t ptr ) 
        {
            so_gpu::memory::dealloc( ptr ) ;
        }

    public:

        vertex_shader( void_t ) : base_t(shader_type::vertex_shader) {}
        vertex_shader( vertex_shader && rhv ) : shader( std::move(rhv) ){}
        virtual ~vertex_shader( void_t ) {}
        
    public:

        primitive_type get_output_type( void_t ) const { return _pt ; }
        void_t set_output_type( primitive_type t ) { _pt = t ; }

    public:

        virtual void_t destroy( void_t ) final 
        {
            this_t::destroy( this ) ;
        }

    };
    so_typedef( vertex_shader ) ;
}

#endif

