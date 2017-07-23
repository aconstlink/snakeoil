//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_SHADE_SHADER_PIXEL_SHADER_H_
#define _SNAKEOIL_SHADE_SHADER_PIXEL_SHADER_H_

#include "shader.h"

#include "../code/ascii_code.h"
#include "binding/texture_binding.h"

#include <snakeoil/std/container/vector.hpp>

namespace so_shade
{
    namespace so_shader
    {
        class SNAKEOIL_SHADE_API pixel_shader : public shader
        {
            so_this_typedefs( pixel_shader ) ;
            
            so_typedefs( so_std::vector<so_shade::so_shader::texture_binding>,
                texture_bindings ) ;

        private:

            so_shade::ascii_code_t _code ;
            texture_bindings_t _tex_binds ;
            
        public:

            pixel_shader( void_t ) ;
            pixel_shader( this_rref_t ) ;
            pixel_shader( this_cref_t ) ;
            pixel_shader( so_shade::ascii_code_in_t ) ;
            ~pixel_shader( void_t ) ;

        public:

            this_ref_t operator = ( this_cref_t ) ;

        public:

            static this_ptr_t create( this_cref_t, so_memory::purpose_cref_t ) ;
            static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
            static void_t destroy( this_ptr_t ) ;

        public:

            void_t set_code( so_shade::ascii_code_in_t ) ;
            ascii_code_cref_t get_code( void_t ) ;

            void_t add( so_shade::so_shader::texture_binding_in_t ) ;

        public:

            size_t get_num_texture_bindings( void_t ) const ;
            bool_t get_texture_binding( size_t, so_shade::so_shader::texture_binding_out_t ) const ;

        public:

            virtual void_t destroy( void_t ) ;
        };
        so_typedef( pixel_shader ) ;
    }
}

#endif
