//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_SHADE_SHADER_VERTEX_SHADER_H_
#define _SNAKEOIL_SHADE_SHADER_VERTEX_SHADER_H_

#include "shader.h"

#include "binding/geometry_binding.h"
#include "binding/object_binding.h"
#include "binding/camera_binding.h"

#include "../code/ascii_code.h"

#include <snakeoil/std/container/vector.hpp>

namespace so_shade
{
    namespace so_shader
    {
        class SNAKEOIL_SHADE_API vertex_shader : public shader
        {
            so_this_typedefs( vertex_shader ) ;
            
            so_typedefs( so_std::vector<so_shade::so_shader::texcoord_binding>,
                texcoord_bindings ) ;

            so_typedefs( so_std::vector<so_shade::so_shader::object_binding>,
                object_bindings ) ;

            so_typedefs( so_std::vector<so_shade::so_shader::camera_binding>,
                camera_bindings ) ;

        private:

            so_shade::ascii_code_t _code ;

            so_shade::so_shader::position_binding _pos_bind ;
            so_shade::so_shader::normal_binding _nrm_bind ;
            texcoord_bindings_t _texcoord_binds ;
            object_bindings_t _object_binds ;
            camera_bindings_t _cam_binds ;
            
        public:

            vertex_shader( void_t ) ;
            vertex_shader( so_shade::ascii_code_in_t ) ;
            vertex_shader( this_cref_t ) ;
            vertex_shader( this_rref_t ) ;
            ~vertex_shader( void_t ) ;

        public:

            this_ref_t operator = ( this_cref_t ) ;

        public:

            static this_ptr_t create( this_cref_t, so_memory::purpose_cref_t ) ;
            static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
            static void_t destroy( this_ptr_t ) ;

        public:

            void_t set_code( so_shade::ascii_code_in_t ) ;
            ascii_code_cref_t get_code( void_t ) ;

            void_t set( so_shade::so_shader::position_binding_in_t ) ;
            void_t set( so_shade::so_shader::normal_binding_in_t ) ;
            void_t add( so_shade::so_shader::texcoord_binding_in_t ) ;
            void_t add( so_shade::so_shader::object_binding_in_t ) ;
            void_t add( so_shade::so_shader::camera_binding_in_t ) ;
            
            bool_t get( so_shade::so_shader::position_binding_out_t ) const ;
            bool_t get( so_shade::so_shader::normal_binding_out_t ) const ;
            
            size_t get_num_texcood_bindings( void_t ) const ;
            bool_t get_texcoord_binding( size_t i, so_shade::so_shader::texcoord_binding_out_t ) const ;

            size_t get_num_object_bindings( void_t ) const ;
            bool_t get_object_binding( size_t, so_shade::so_shader::object_binding_out_t ) const ;

            size_t get_num_camera_bindings( void_t ) const ;
            bool_t get_camera_binding( size_t, so_shade::so_shader::camera_binding_out_t ) const ;

        public:

            virtual void_t destroy( void_t ) ;
        
        };
        so_typedef( vertex_shader ) ;
    }
}

#endif
