//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "mld_effect_generator.h"
#include "../../shader/effect.h"
#include "../../material/standard_material.h"
#include "../../variable.h"

#include <snakeoil/math/vector/vector4.hpp>

using namespace so_shade ;
using namespace so_shade::so_generator ;
using namespace so_shade::so_generator::so_glsl ;

//*************************************************************************************
mld_effect_generator::mld_effect_generator( mld_generate_params_cref_t, 
    so_shade::so_generator::so_glsl::generator_properties_cref_t gp ) : effect_generator(gp)
{
    this_t::gen_props().enable_linear_depth = true ;
}

//*************************************************************************************
mld_effect_generator::mld_effect_generator( this_rref_t rhv ) : effect_generator( std::move(rhv) )
{

}

//*************************************************************************************
mld_effect_generator::~mld_effect_generator( void_t )
{

}

//*************************************************************************************
mld_effect_generator::this_ptr_t mld_effect_generator::create( this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_shade::memory::alloc( std::move(rhv), p ) ;
}

//*************************************************************************************
void_t mld_effect_generator::destroy( this_ptr_t p )
{
    so_shade::memory::dealloc( p ) ;
}

//*************************************************************************************
so_shade::result mld_effect_generator::generate_effect_from(
    so_shade::so_material::standard_material_ptr_t mat_ptr,
    so_shade::so_shader::effect_out_t eout ) 
{
    if( so_core::is_nullptr(mat_ptr) )
        return so_shade::invalid_argument ;
    
    return this_t::generate_glsl33_mld_effect_from( mat_ptr, eout ) ;
}

//*************************************************************************************
so_shade::result mld_effect_generator::generate_glsl33_mld_effect_from(
    so_shade::so_material::standard_material_ptr_t mat_ptr,
    so_shade::so_shader::effect_out_t eout ) 
{
    so_shade::so_shader::effect_t eff ;

    {
        eff.set_vertex_shader( so_shade::so_shader::vertex_shader_t::create(
            so_shade::so_shader::vertex_shader_t(),
            "[so_shade::mld_effect_generator::generate_glsl33_effect_from] : vertex shader" ) ) ;

        so_std::string_t code ;
        code += "// [GEN] : snakeoil glsl 3.30 code generator : MLD\n" ;
        code += "#version 330 core\n" ;
        code += "\n" ;
        code += "// [GEN] : Vertex Attributes \n" ;
        code += this_t::generate_glsl33_ins_from( mat_ptr, eff ) ;
        code += "\n" ;
        code += "// [GEN] : Uniforms \n" ;
        code += this_t::generate_glsl33_uniforms_from( mat_ptr, eff ) ;
        code += "// [GEN] : MLD Uniforms \n" ;
        code +=  this_t::generate_glsl33_mld_uniforms_from( mat_ptr, eff ) ;
        code += "\n" ;
        code += "// [GEN] : Defines \n" ;
        code += this_t::generate_glsl33_defines_from( mat_ptr ) ;

        code += "\n" ;
        code += "// [GEN] : Varying Data \n" ;
        code += "out vertex_data{\n" ;
        code += this_t::generate_glsl33_normal_varyings_from( mat_ptr ) ;
        code += this_t::generate_glsl33_light_varyings_from( mat_ptr ) ;
        code += this_t::generate_glsl33_texcoord_varyings_from( mat_ptr ) ;
        
        // linear z is encoded otherwise in the MLD shader
        //code += this_t::generate_glsl33_linear_z_varyings_from( mat_ptr ) ;
        code += this_t::generate_glsl33_mld_varyings_from( mat_ptr ) ;
        code += "}vso ;\n" ;
        code += "\n" ;
        code += "void main()\n" ;
        code += "{\n" ;

        code += this_t::generate_glsl33_obj_to_view_pos_transform_from( mat_ptr ) ;
        code += "\n" ;
        //code += this_t::generate_glsl33_linear_z_write_through_from( mat_ptr ) ;
        code += this_t::generate_glsl33_mld_linear_z_info_write_through_from( mat_ptr ) ;
        code += "\n" ;
        code += this_t::generate_glsl33_normal_write_through_from( mat_ptr ) ;
        code += "\n" ;
        code += this_t::generate_glsl33_light_write_through_from( mat_ptr ) ;
        code += "\n" ;
        code += this_t::generate_glsl33_texcoord_write_through_from( mat_ptr ) ;
        code += "\n" ;
        code += this_t::generate_glsl33_view_to_proj_pos_transform_from( mat_ptr ) ;

        code += "}\n" ;

        eff.get_vertex_shader()->set_code( code ) ;
    }

    {
        eff.set_pixel_shader( so_shade::so_shader::pixel_shader_t::create(
            so_shade::so_shader::pixel_shader_t(),
            "[so_shade::mld_effect_generator::generate_glsl33_effect_from] : pixel_shader" ) ) ;

        so_std::string_t code ;
        code += "// [GEN] : snakeoil glsl 3.30 code generator : MLD \n" ;
        code += "#version 330 core\n" ;
        code += "\n" ;
        code += "layout( location = 0, index = 0 ) out vec4 out_ ;\n" ;
        code += "\n" ;
        code += "// [GEN] : Uniforms \n" ;
        code += this_t::generate_glsl33_mld_uniforms_ps_from() ;
        code += this_t::generate_glsl33_samplers_from( mat_ptr, eff ) ;
        code += "\n" ;
        code += "// [GEN] : Varying Data \n" ;
        code += "in vertex_data{\n" ;
        code += this_t::generate_glsl33_normal_varyings_from( mat_ptr ) ;
        code += this_t::generate_glsl33_light_varyings_from( mat_ptr ) ;
        code += this_t::generate_glsl33_texcoord_varyings_from( mat_ptr ) ;
        // linear z is encoded otherwise in the MLD shader
        //code += this_t::generate_glsl33_linear_z_varyings_from( mat_ptr ) ;
        code += this_t::generate_glsl33_mld_varyings_from( mat_ptr ) ;

        code += "}psi ;\n" ;
        code += "\n" ;
        code += "// [GEN] : varying defines \n" ;
        code += this_t::generate_glsl33_mld_varying_defines_ps_from( mat_ptr ) ;
        code += "\n\n" ;
        
        //
        // SECTION : Functions
        code += "vec4 albedo_color_funk( void ) \n" ;
        code += "{\n" ;
        code += this_t::generate_glsl33_texture_access_from( mat_ptr ) ;
        code += this_t::generate_glsl33_albedo_output_from( mat_ptr ) ;
        code += "} \n\n" ;


        //
        // SECTION : main
        code += "void main()\n" ;
        code += "{\n" ;
        code += "// [GEN] : Compute color\n" ;
        code += this_t::generate_glsl33_pixel_shader_call_albedo_from() ;
        code += "\n" ;
        code += "// [GEN] : mld specifc\n" ;
        code += this_t::generate_glsl33_mld_z_comptue_ps_from() ;
        code += this_t::generate_glsl33_mld_mix_front_back_from() ;
        code += "\n" ;
        code += "// [GEN] : shader output \n" ;
        code += "out_ = final_color ; \n" ;        
        code += this_t::generate_glsl33_linear_z_output_ps_from() ;
        code += "}\n" ;

        eff.get_pixel_shader()->set_code( code ) ;
    }

    eout = std::move( eff ) ;

    return so_shade::ok ;
}

//*************************************************************************************
so_std::string_t mld_effect_generator::generate_glsl33_mld_uniforms_from(
    so_shade::so_material::standard_material_ptr_t,
    so_shade::so_shader::effect_ref_t ein )
{
    so_shade::so_shader::mld_variable_bindings bindings ;

    so_std::string_t expr ;

    expr += "// x: X_front : relative to center \n" ;
    expr += "// y: X_back : relative to center \n" ;
    expr += "// z: X_center : view space z pos \n" ;
    expr += "// w: unused \n" ;
    expr += "uniform vec4 u_mld_info_0 ; \n" ;
    {
        so_shade::so_shader::mld_variable_binding_t bind ;
        bind.binding_point = so_shade::so_shader::mld_binding_point::mld_info_0 ;
        bind.var.name = "u_mld_info_0" ;
        bind.var.of_type = so_shade::type_t(
            so_shade::type_base::tfloat, so_shade::type_struct::vector4 ) ;
        
        bind.var.create_default_value( so_math::vec4f_t( -10.0f, 10.0f, 1000.0f, 0.0f ) ) ;

        bindings.bindings.push_back( bind ) ;
    }

    expr += "// x: X_offset coeff \n" ;
    expr += "// y: unused \n" ;
    expr += "// z: unused \n" ;
    expr += "// w: unused \n" ;
    expr += "uniform vec4 u_mld_info_1 ; \n" ;
    {
        so_shade::so_shader::mld_variable_binding_t bind ;
        bind.binding_point = so_shade::so_shader::mld_binding_point::mld_info_1 ;
        bind.var.name = "u_mld_info_1" ;
        bind.var.of_type = so_shade::type_t(
            so_shade::type_base::tfloat, so_shade::type_struct::vector4 ) ;

        bind.var.create_default_value( so_math::vec4f_t( 1.0f, 0.0f, 0.0f, 0.0f ) ) ;

        bindings.bindings.push_back( bind ) ;
    }

    expr += "// x: front layer select: 1.0->true, 0.0->false \n" ;
    expr += "// y: unused \n" ;
    expr += "// z: unused \n" ;
    expr += "// w: unused \n" ;
    expr += "uniform vec4 u_mld_info_2 ; \n" ;
    {
        so_shade::so_shader::mld_variable_binding_t bind ;
        bind.binding_point = so_shade::so_shader::mld_binding_point::mld_info_2 ;
        bind.var.name = "u_mld_info_2" ;
        bind.var.of_type = so_shade::type_t(
            so_shade::type_base::tfloat, so_shade::type_struct::vector4 ) ;

        bind.var.create_default_value( so_math::vec4f_t(1.0f,0.0f,0.0f,0.0f) ) ;

        bindings.bindings.push_back( bind ) ;
    }

    ein.get_vertex_shader()->add_custom_binding( 
        so_shade::so_shader::mld_variable_bindings::create( std::move( bindings ), 
        "[mld_effect_generator::generate_glsl33_mld_uniforms_from] : mld_variable_bindings" ) ) ;

    return std::move(expr) ;
}

//*************************************************************************************
so_std::string_t mld_effect_generator::generate_glsl33_mld_uniforms_ps_from( void_t ) 
{
    so_std::string_t expr ;
    expr += "uniform vec4 u_mld_info_0 ; \n" ;
    expr += "uniform vec4 u_mld_info_1 ; \n" ;
    expr += "uniform vec4 u_mld_info_2 ; \n" ;
    return std::move(expr) ;
}

//*************************************************************************************
so_std::string_t mld_effect_generator::generate_glsl33_mld_varyings_from(
    so_shade::so_material::standard_material_ptr_t mat_ptr ) 
{
    so_std::string_t expr ;
    expr += "// [GEN] : vec3( mld_linear_near, mld_linear_far, linear_z)\n" ;
    expr += "vec3 linear_z_info ; \n" ;
    return std::move(expr) ;
}

//*************************************************************************************
so_std::string_t mld_effect_generator::generate_glsl33_mld_varying_defines_ps_from(
    so_shade::so_material::standard_material_ptr_t ) 
{
    so_std::string_t expr ;

    expr += "#define __mld_near psi.linear_z_info.x \n" ;
    expr += "#define __mld_far psi.linear_z_info.y \n" ;
    expr += "#define __linear_z psi.linear_z_info.z \n" ;

    return std::move( expr ) ;
}

//*************************************************************************************
so_std::string_t mld_effect_generator::generate_glsl33_mld_linear_z_info_write_through_from(
    so_shade::so_material::standard_material_ptr_t mat_ptr ) 
{
    so_std::string_t expr ;
 
    expr += "// [GEN] : MLD and linear z info \n" ;
    expr += "vec2 mld_front_back = vec2(u_mld_info_0.xy) + vec2(u_mld_info_0.z) ;\n" ;
    expr += "vso.linear_z_info = ( vec3(mld_front_back,view_pos.z) - vec3(__near) ) /\
            \r(vec3( __far ) - vec3( __near )) ; \n" ;
    return std::move( expr ) ;
}

//*************************************************************************************
so_std::string_t mld_effect_generator::generate_glsl33_mld_z_comptue_ps_from( void_t ) 
{
    so_std::string_t expr ;

    expr += "float mld_z = ( __linear_z - __mld_near) / (__mld_far - __mld_near ) ; \n" ;
    expr += "float mld_t_front = clamp( mld_z, 0.0, 1.0 ) ;\n" ;
    expr += "float mld_t_back = 1.0 - smoothstep( 0.0, u_mld_info_1.x, mld_t_front ) ;\n" ;

    return std::move( expr ) ;
}

//*************************************************************************************
so_std::string_t mld_effect_generator::generate_glsl33_mld_mix_front_back_from( void_t ) 
{
    so_std::string_t expr ;

    expr += "vec4 front_color = mix( albedo_color, vec4(0,0,0,1), mld_t_front ) ; \n" ;
    expr += "vec4 back_color = mix( albedo_color, vec4(0,0,0,1), mld_t_back ) ; \n" ;

    expr += "vec4 final_color = mix( back_color, front_color, u_mld_info_2.x ) ; \n" ;
    
    return std::move( expr ) ;
}

//*************************************************************************************
so_std::string_t mld_effect_generator::generate_glsl33_linear_z_output_ps_from( void_t ) 
{
    so_std::string_t expr ;    
    expr += "gl_FragDepth = __linear_z ; \n" ;    
    
    return std::move( expr ) ;
}

