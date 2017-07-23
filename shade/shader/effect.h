//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_SHADE_SHADER_EFFECT_H_
#define _SNAKEOIL_SHADE_SHADER_EFFECT_H_

#include "ieffect.h"

#include "vertex_shader.h"
#include "pixel_shader.h"

#include <snakeoil/std/container/vector.hpp>

namespace so_shade
{
    namespace so_shader
    {
        class SNAKEOIL_SHADE_API effect : public ieffect
        {
            so_this_typedefs( effect ) ;

            struct texture_binding_data
            {
                so_std::string_t binding_point ;
                so_std::string_t user_data ;
            };
            so_typedef( texture_binding_data ) ;
            so_typedefs( so_std::vector< texture_binding_data_t >, texture_bindings ) ;

        private: // vertex shader

            so_shade::so_shader::vertex_shader_ptr_t _vs_ptr = nullptr ;
            // vertex shader texture bindings

        private: // pixel shader

            so_shade::so_shader::pixel_shader_ptr_t _ps_ptr = nullptr ;
            texture_bindings_t _pixel_shader_texture_bindings ;

        private:



            // render states

        public:

            effect( void_t ) ;
            effect( this_rref_t ) ;
            effect( this_cref_t ) ;
            ~effect( void_t ) ;

        public:

            this_ref_t operator = ( this_cref_t ) ;
            this_ref_t operator = ( this_rref_t ) ;

        public:

            static this_ptr_t create( so_memory::purpose_cref_t ) ;
            static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
            static void_t destroy( this_ptr_t ) ;

        public:
                        
            bool_t set_vertex_shader( so_shade::so_shader::vertex_shader_ptr_t ) ;
            bool_t set_pixel_shader( so_shade::so_shader::pixel_shader_ptr_t ) ;

            so_shade::so_shader::vertex_shader_ptr_t get_vertex_shader( void_t ) ;
            so_shade::so_shader::pixel_shader_ptr_t get_pixel_shader( void_t ) ;

            so_shade::so_shader::vertex_shader_cptr_t get_vertex_shader( void_t ) const ;
            so_shade::so_shader::pixel_shader_cptr_t get_pixel_shader( void_t ) const ;

            bool_t add_pixel_shader_binding( so_shade::texture_variable_cref_t, so_std::string_cref_t ) ;
            bool_t get_pixel_shader_binding( so_shade::texture_variable_cref_t, so_std::string_out_t ) const ;

        public:

            virtual void_t destroy( void_t ) ;
        };
        so_typedef( effect ) ;
    }
}

#endif
