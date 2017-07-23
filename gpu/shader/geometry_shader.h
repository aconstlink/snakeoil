//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_SHADER_GEOMETRY_SHADER_H_
#define _SNAKEOIL_GPU_SHADER_GEOMETRY_SHADER_H_

#include "shader.h"
#include "../enums/geometry_shader_input_types.h"
#include "../enums/geometry_shader_output_types.h"

namespace so_gpu
{
    class SNAKEOIL_GPU_API geometry_shader : public so_gpu::shader
    {
        typedef so_gpu::shader base_t ;
        so_this_typedefs( geometry_shader ) ;

    public:

        geometry_shader_input_type _input_type = geometry_shader_input_type::undefined ;
        geometry_shader_output_type _output_type = geometry_shader_output_type::undefined ;
        size_t _num_output_vertices = 0 ;

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

        geometry_shader( void_t ) : base_t(shader_type::geometry_shader) {}
        geometry_shader( this_rref_t rhv ) : shader( std::move(rhv) )
        {
            _input_type = rhv._input_type ;
            _output_type = rhv._output_type ;
            _num_output_vertices = rhv._num_output_vertices ;
        }

        virtual ~geometry_shader( void_t ) {}
        
    public:

        size_t get_num_output_vertices( void_t ) const { return _num_output_vertices ; }
        void_t set_num_output_vertices( size_t n ) { _num_output_vertices = n ; }

        geometry_shader_input_type get_input_type( void_t ) const { return _input_type ; }
        geometry_shader_output_type get_output_type( void_t ) const { return _output_type ; }

        void_t set_input_type( geometry_shader_input_type t ) { _input_type = t ; }
        void_t set_output_type( geometry_shader_output_type t ) { _output_type = t ; }

    private:

        virtual void_t destroy( void_t )
        {
            this_t::destroy( this ) ;
        }

    };
    so_typedef( geometry_shader ) ;
}

#endif

