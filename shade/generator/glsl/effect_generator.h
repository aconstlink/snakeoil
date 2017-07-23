//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_SHADE_GENERATOR_GLSL_EFFECT_GENERATOR_H_
#define _SNAKEOIL_SHADE_GENERATOR_GLSL_EFFECT_GENERATOR_H_

#include "../ieffect_generator.h"
#include "../../variable.h"

#include "../../shader/effect.h"

#include "generator_properties.h"

#include <snakeoil/math/vector/vector3.hpp>

namespace so_shade
{
    namespace so_generator
    {
        namespace so_glsl
        {
            class SNAKEOIL_SHADE_API effect_generator : public ieffect_generator
            {
                so_this_typedefs( effect_generator ) ;

            private:

                so_shade::so_generator::so_glsl::generator_properties_t _gen_props ;

            public:

                effect_generator( so_shade::so_generator::so_glsl::generator_properties_cref_t ) ;
                effect_generator( this_rref_t ) ;
                virtual ~effect_generator( void_t ) ;

            public:

                static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
                static void_t destroy( this_ptr_t ) ;

            public:

                so_shade::result generate_effect_from( 
                    so_shade::so_material::standard_material_ptr_t,
                    so_shade::so_shader::effect_out_t ) ;

            private:

                so_shade::result generate_glsl33_effect_from( 
                    so_shade::so_material::standard_material_ptr_t,
                    so_shade::so_shader::effect_out_t ) ;

            protected:

                so_shade::so_generator::so_glsl::generator_properties_cref_t gen_props(void_t) const 
                { return _gen_props ; }

                so_shade::so_generator::so_glsl::generator_properties_ref_t gen_props( void_t )
                { return _gen_props ; }

            protected: // ins/uniforms

                so_std::string_t generate_glsl33_ins_from(
                    so_shade::so_material::standard_material_ptr_t,
                    so_shade::so_shader::effect_ref_t ) ;

                so_std::string_t generate_glsl33_uniforms_from(
                    so_shade::so_material::standard_material_ptr_t,
                    so_shade::so_shader::effect_ref_t ) ;

                so_std::string_t generate_glsl33_samplers_from(
                    so_shade::so_material::standard_material_ptr_t,
                    so_shade::so_shader::effect_ref_t ) ;

            protected: // defines

                so_std::string_t generate_glsl33_defines_from(
                    so_shade::so_material::standard_material_ptr_t ) ;

            protected: // varyings

                so_std::string_t generate_glsl33_texcoord_varyings_from(
                    so_shade::so_material::standard_material_ptr_t ) ;
                so_std::string_t generate_glsl33_normal_varyings_from(
                    so_shade::so_material::standard_material_ptr_t ) ;
                so_std::string_t generate_glsl33_light_varyings_from(
                    so_shade::so_material::standard_material_ptr_t ) ;
                so_std::string_t generate_glsl33_linear_z_varyings_from(
                    so_shade::so_material::standard_material_ptr_t ) ;

            protected: // write to varyings

                so_std::string_t generate_glsl33_texcoord_write_through_from(
                    so_shade::so_material::standard_material_ptr_t ) ;
                so_std::string_t generate_glsl33_normal_write_through_from(
                    so_shade::so_material::standard_material_ptr_t ) ;
                so_std::string_t generate_glsl33_light_write_through_from(
                    so_shade::so_material::standard_material_ptr_t ) ;

                so_std::string_t generate_glsl33_linear_z_write_through_from(
                    so_shade::so_material::standard_material_ptr_t ) ;

            protected: // inside code
                
                so_std::string_t generate_glsl33_texture_access_from(
                    so_shade::so_material::standard_material_ptr_t ) ;
                so_std::string_t generate_glsl33_albedo_output_from(
                    so_shade::so_material::standard_material_ptr_t ) ;

                so_std::string_t generate_glsl33_linear_z_output_from(
                    so_shade::so_material::standard_material_ptr_t ) ;

                so_std::string_t generate_glsl33_pixel_shader_call_albedo_from( void_t ) ;

                so_std::string_t generate_glsl33_obj_to_view_pos_transform_from(
                    so_shade::so_material::standard_material_ptr_t ) ;

                so_std::string_t generate_glsl33_view_to_proj_pos_transform_from(
                    so_shade::so_material::standard_material_ptr_t ) ;

                so_std::string_t variable_to_string_glsl33( so_shade::variable_cref_t ) ;
                so_std::string_t texture_variable_to_string_glsl33( so_shade::texture_variable_cref_t ) ;

            protected:

                so_std::string_t convert_vec3_to_glsl( so_math::vec3f_cref_t ) const ;
            };
        }
        
    }
}

#endif
