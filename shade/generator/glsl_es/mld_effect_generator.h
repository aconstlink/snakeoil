//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_SHADE_GENERATOR_GLSL_ES_MLD_EFFECT_GENERATOR_H_
#define _SNAKEOIL_SHADE_GENERATOR_GLSL_ES_MLD_EFFECT_GENERATOR_H_

#include "effect_generator.h"

#include "../../variable.h"
#include "../../shader/effect.h"
#include "../../shader/binding/mld_bindings.h"

#include <snakeoil/math/vector/vector3.hpp>

namespace so_shade
{
    namespace so_generator
    {
        namespace so_glsl_es
        {
            class SNAKEOIL_SHADE_API mld_effect_generator : public effect_generator
            {
                so_this_typedefs( mld_effect_generator ) ;
                
            public:

                struct mld_generate_params
                {

                };
                so_typedef( mld_generate_params ) ;

            public:

                mld_effect_generator( mld_generate_params_cref_t, 
                    so_shade::so_generator::so_glsl_es::generator_properties_cref_t ) ;
                mld_effect_generator( this_rref_t ) ;
                virtual ~mld_effect_generator( void_t ) ;

            public:

                static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
                static void_t destroy( this_ptr_t ) ;

            public:

                so_shade::result generate_effect_from( 
                    so_shade::so_material::standard_material_ptr_t, 
                    so_shade::so_shader::effect_out_t eout ) ;

            private:

                so_shade::result generate_glsl_es1_mld_effect_from( 
                    so_shade::so_material::standard_material_ptr_t,
                    so_shade::so_shader::effect_out_t eout ) ;


                so_std::string_t generate_glsl_es1_mld_uniforms_from(
                    so_shade::so_material::standard_material_ptr_t,
                    so_shade::so_shader::effect_ref_t ) ;
                
                so_std::string_t generate_glsl_es1_mld_uniforms_ps_from( void_t ) ;

            private: // varyings

                so_std::string_t generate_glsl_es1_mld_varyings_from(
                    so_shade::so_material::standard_material_ptr_t ) ;

                so_std::string_t generate_glsl_es1_mld_varying_defines_ps_from(
                    so_shade::so_material::standard_material_ptr_t ) ;
                
                so_std::string_t generate_glsl_es1_mld_linear_z_info_write_through_from(
                    so_shade::so_material::standard_material_ptr_t ) ;
                
                

            private:

                so_std::string_t generate_glsl_es1_mld_z_comptue_ps_from( void_t ) ;

                so_std::string_t generate_glsl_es1_mld_mix_front_back_from( void_t ) ;

                so_std::string_t generate_glsl_es1_linear_z_output_ps_from( void_t ) ;


            private:

                so_std::string_t convert_vec3_to_glsl( so_math::vec3f_cref_t ) const ;
            };
        }
        
    }
}

#endif
