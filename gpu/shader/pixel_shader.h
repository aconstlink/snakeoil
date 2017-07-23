//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_SHADER_PIXEL_SHADER_H_
#define _SNAKEOIL_GPU_SHADER_PIXEL_SHADER_H_

#include "shader.h"

namespace so_gpu
{
    class SNAKEOIL_GPU_API pixel_shader : public so_gpu::shader
    {
        typedef so_gpu::shader base_t ;
        so_this_typedefs( pixel_shader ) ;

    public:

        static this_ptr_t create( so_memory::purpose_cref_t purpose ) 
        {
            return so_gpu::memory::alloc( this_t(), purpose ) ;
        }

        static this_ptr_t create( this_rref_t rhv, so_memory::purpose_cref_t purpose )
        {
            return so_gpu::memory::alloc( std::move(rhv), purpose ) ;
        }

        static void_t destroy( this_ptr_t ptr ) 
        {
            so_gpu::memory::dealloc( ptr ) ;
        }

    public:

        pixel_shader( void_t ) : base_t(shader_type::pixel_shader) {}
        pixel_shader( this_rref_t rhv ) : shader( std::move(rhv) ){}
        virtual ~pixel_shader( void_t ) {}

    private:

        virtual void_t destroy( void_t )
        {
            this_t::destroy( this ) ;
        }
    };
    so_typedef( pixel_shader ) ;
}

#endif

