//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "effect_generator.h"
#include "../../shader/effect.h"
#include "../../material/standard_material.h"
#include "../../variable.h"

using namespace so_shade ;
using namespace so_shade::so_generator ;
using namespace so_shade::so_generator::so_glsl_es ;

//*************************************************************************************
effect_generator::effect_generator( so_shade::so_generator::so_glsl_es::generator_properties_cref_t props )
{
    _gen_props = props ;
}

//*************************************************************************************
effect_generator::effect_generator( this_rref_t rhv )
{
    _gen_props = std::move(rhv._gen_props) ;
}

//*************************************************************************************
effect_generator::~effect_generator( void_t )
{}

//*************************************************************************************
effect_generator::this_ptr_t effect_generator::create( this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_shade::memory::alloc( std::move(rhv), p ) ;
}

//*************************************************************************************
void_t effect_generator::destroy( this_ptr_t p )
{
    so_shade::memory::dealloc( p ) ;
}

//*************************************************************************************
so_shade::result effect_generator::generate_effect_from(
    so_shade::so_material::standard_material_ptr_t mat_ptr,
    so_shade::so_shader::effect_out_t eff_ptr ) 
{
    if( so_core::is_nullptr(mat_ptr) )
        return so_shade::invalid_argument ;

    return this_t::generate_glsl_es1_effect_from( mat_ptr, eff_ptr ) ;
}

//*************************************************************************************
so_shade::result effect_generator::generate_glsl_es1_effect_from(
    so_shade::so_material::standard_material_ptr_t mat_ptr,
    so_shade::so_shader::effect_out_t eff_out ) 
{
    so_shade::so_shader::effect_t eff ;

    {
        eff.set_vertex_shader( so_shade::so_shader::vertex_shader_t::create(
            so_shade::so_shader::vertex_shader_t(),
            "[so_shade::effect_generator::generate_glsl_es1_effect_from] : vertex shader" ) ) ;

        so_std::string_t code ;
        code += "#version 100\n" ;
        code += "//\n" ;
        code += "// [GEN] : snakeoil gles glsl code generator\n" ;
        code += "//\n" ;
        code += "precision highp float ; \n" ;
        //code += "precision highp int ; \n" ;
        //code += "precision highp sampler2D ; \n" ;
        code += "\n" ;
        code += "// [GEN] : Vertex Attributes \n" ;
        code += this_t::generate_glsl_es1_ins_from( mat_ptr, eff ) ;
        code += "\n" ;
        code += "// [GEN] : Uniforms \n" ;
        code += this_t::generate_glsl_es1_uniforms_from( mat_ptr, eff ) ;
        code += "\n" ;
        code += "// [GEN] : Defines \n" ;
        code += this_t::generate_glsl_es1_defines_from( mat_ptr ) ;
        
        code += "\n" ;
        code += "// [GEN] : Varying Data \n" ;
        //code += "out vertex_data{\n" ;
        code += this_t::generate_glsl_es1_normal_varyings_from( mat_ptr ) ;
        code += this_t::generate_glsl_es1_light_varyings_from( mat_ptr ) ;
        code += this_t::generate_glsl_es1_texcoord_varyings_from( mat_ptr ) ;
        code += this_t::generate_glsl_es1_linear_z_varyings_from( mat_ptr ) ;
        //code += "}vso ;\n" ;
        code += "\n" ;
        code += "void main()\n" ;
        code += "{\n" ;
        
        code += this_t::generate_glsl_es1_obj_to_view_pos_transform_from( mat_ptr ) ;
        code += "\n" ;
        code += this_t::generate_glsl_es1_linear_z_write_through_from( mat_ptr ) ;
        code += "\n" ;
        code += this_t::generate_glsl_es1_normal_write_through_from(mat_ptr) ;
        code += "\n" ;
        code += this_t::generate_glsl_es1_light_write_through_from(mat_ptr) ;
        code += "\n" ;
        code += this_t::generate_glsl_es1_texcoord_write_through_from(mat_ptr) ;
        code += "\n" ;
        code += this_t::generate_glsl_es1_view_to_proj_pos_transform_from( mat_ptr ) ;

        code += "}\n" ;

        eff.get_vertex_shader()->set_code( code ) ;
    }

    {
        eff.set_pixel_shader( so_shade::so_shader::pixel_shader_t::create(
            so_shade::so_shader::pixel_shader_t(),
            "[so_shade::effect_generator::generate_glsl_es1_effect_from] : pixel_shader" ) ) ;

        so_std::string_t code ;
        code += "#version 100\n" ;
        code += "precision highp float ;\n" ;
        code += "//\n" ;
        code += "// [GEN] : snakeoil gles glsl code generator\n" ;
        code += "//\n" ;
        code += this_t::generate_glsl_es1_samplers_from( mat_ptr, eff ) ;
        code += "\n" ;
        //code += "in vertex_data{\n" ;
        code += this_t::generate_glsl_es1_normal_varyings_from( mat_ptr ) ;
        code += this_t::generate_glsl_es1_light_varyings_from( mat_ptr ) ;
        code += this_t::generate_glsl_es1_texcoord_varyings_from( mat_ptr ) ;
        code += this_t::generate_glsl_es1_linear_z_varyings_from( mat_ptr ) ;
        
        //code += "}psi ;\n" ;
        code += "\n" ;

        //
        // SECTION : Functions
        code += "vec4 albedo_color_funk( void ) \n" ;
        code += "{\n" ;
        code += this_t::generate_glsl_es1_texture_access_from( mat_ptr ) ;
        code += this_t::generate_glsl_es1_albedo_output_from( mat_ptr ) ;
        code += "} \n\n " ;


        //
        // SECTION : main
        code += "void main()\n" ;
        code += "{\n" ;
        //code += "// [GEN] : do texture color composition\n" ;
        code += generate_glsl_es1_pixel_shader_call_albedo_from() ;
        code += "gl_FragColor = albedo_color ; \n" ;
        //code += "out_ = vec4(var_txc_0.xy,0,0) ; " ; // test
        code += "\n" ;
        code += this_t::generate_glsl_es1_linear_z_output_from( mat_ptr ) ;
        code += "}\n" ;

        eff.get_pixel_shader()->set_code( code ) ;        
    }

    eff_out = std::move( eff ) ;
    return so_shade::ok ;
}

//*************************************************************************************
so_std::string_t effect_generator::generate_glsl_es1_ins_from(
    so_shade::so_material::standard_material_ptr_t mat_ptr,
    so_shade::so_shader::effect_ref_t ein ) 
{
    so_std::string_t expr ;

    expr += "attribute vec3 "+_gen_props.position_name+" ; \n" ;   
    {
        so_shade::so_shader::position_binding_t pos_bind ;
        pos_bind.var.name = _gen_props.position_name ;
        pos_bind.var.of_type = so_shade::type(
            so_shade::type_base::tfloat, so_shade::type_struct::vector3 ) ;
        ein.get_vertex_shader()->set( pos_bind ) ;
    }
    
    if( mat_ptr->has_normal_layer() )
    {
        expr += "attribute vec3 "+_gen_props.normal_name+" ; \n" ;
        {
            so_shade::so_shader::normal_binding_t bind ;
            bind.var.name = _gen_props.normal_name ;
            bind.var.of_type = so_shade::type(
                so_shade::type_base::tfloat, so_shade::type_struct::vector3 ) ;
            ein.get_vertex_shader()->set( bind ) ;
        }
    }

    for( size_t i=0; i<mat_ptr->get_num_texture_layers(); ++i )
    {
        so_std::string_t const i_s = std::to_string(i) ;
        so_std::string_t const tc_name = _gen_props.texcoord_pre_name + (i>0 ? i_s : "") ;
        expr += "attribute vec2 " + tc_name + " ;\n" ;
        {
            so_shade::so_shader::texcoord_binding_t bind ;
            bind.var.name = tc_name ;
            bind.var.of_type = so_shade::type(
                so_shade::type_base::tfloat, so_shade::type_struct::vector2 ) ;
            ein.get_vertex_shader()->add( bind ) ;
        }
    }

    return std::move( expr ) ;
}

//*************************************************************************************
so_std::string_t effect_generator::generate_glsl_es1_uniforms_from(
    so_shade::so_material::standard_material_ptr_t,
    so_shade::so_shader::effect_ref_t ein )
{
    so_std::string_t expr ;

    if( _gen_props.enable_object_matrix )
    {
        expr += "uniform mat4 " + _gen_props.object_matrix_name + " ;\n" ;
        {
            so_shade::so_shader::object_binding_t bind ;
            bind.binding_point = so_shade::so_shader::object_binding_point::object_matrix ;
            bind.var.name = _gen_props.object_matrix_name ;
            bind.var.of_type = so_shade::type_t(
                so_shade::type_base::tfloat, so_shade::type_struct::matrix4 ) ;

            ein.get_vertex_shader()->add( bind ) ;
        }
    }

    expr += "uniform mat4 " + _gen_props.world_matrix_name + " ;\n" ;
    {
        so_shade::so_shader::object_binding_t bind ;
        bind.binding_point = so_shade::so_shader::object_binding_point::world_matrix ;
        bind.var.name = _gen_props.world_matrix_name ;
        bind.var.of_type = so_shade::type_t(
            so_shade::type_base::tfloat, so_shade::type_struct::matrix4 ) ;

        ein.get_vertex_shader()->add( bind ) ;
    }

    expr += "uniform mat4 " + _gen_props.view_matrix_name + " ;\n" ;
    {
        so_shade::so_shader::camera_binding_t bind ;
        bind.binding_point = so_shade::so_shader::camera_binding_point::view_matrix ;
        bind.var.name = _gen_props.view_matrix_name ;
        bind.var.of_type = so_shade::type_t(
            so_shade::type_base::tfloat, so_shade::type_struct::matrix4 ) ;

        ein.get_vertex_shader()->add( bind ) ;
    }

    expr += "uniform mat4 " + _gen_props.projection_matrix_name + " ;\n" ;
    {
        so_shade::so_shader::camera_binding_t bind ;
        bind.binding_point = so_shade::so_shader::camera_binding_point::proj_matrix ;
        bind.var.name = _gen_props.projection_matrix_name ;
        bind.var.of_type = so_shade::type_t(
            so_shade::type_base::tfloat, so_shade::type_struct::matrix4 ) ;

        ein.get_vertex_shader()->add( bind ) ;
    }

    if( _gen_props.enable_mvp_matrix )
    {
        expr += "uniform mat4 " + _gen_props.mvp_matrix_name + " ;\n" ;
        {
            so_shade::so_shader::camera_binding_t bind ;
            bind.binding_point = so_shade::so_shader::camera_binding_point::mvp_matrix ;
            bind.var.name = _gen_props.mvp_matrix_name ;
            bind.var.of_type = so_shade::type_t(
                so_shade::type_base::tfloat, so_shade::type_struct::matrix4 ) ;

            ein.get_vertex_shader()->add( bind ) ;
        }
    }

    if( _gen_props.enable_linear_depth )
    {
        expr += "uniform vec4 "+ _gen_props.projection_param_0_name +" ;\n" ;
        {
            so_shade::so_shader::camera_binding_t bind ;
            bind.binding_point = so_shade::so_shader::camera_binding_point::proj_info_0 ;
            bind.var.name = _gen_props.projection_param_0_name ;
            bind.var.of_type = so_shade::type_t(
                so_shade::type_base::tfloat, so_shade::type_struct::vector4 ) ;

            ein.get_vertex_shader()->add( bind ) ;
        }
    }
    

    return std::move(expr) ;
}

//*************************************************************************************
so_std::string_t effect_generator::generate_glsl_es1_samplers_from(
    so_shade::so_material::standard_material_ptr_t mat_ptr,
    so_shade::so_shader::effect_ref_t ein )
{
    if( mat_ptr->get_num_texture_layers() == 0 )
    {
        return so_std::string_t("// [GEN] : no texture layers - no samplers generated\n") ;
    }

    so_std::string_t expr ;

    for( size_t l = 0; l < mat_ptr->get_num_texture_layers(); ++l )
    {
        so_std::string_t const l_s = std::to_string(l) ;
        
        for( size_t i=0; i<mat_ptr->get_num_textures(l); ++i )
        {            
            so_shade::texture_variable_t user_var ;
            auto res = mat_ptr->get_texture( l, i, user_var ) ;
            if( so_core::is_not(res) ){}
            
            so_std::string_t const i_s = std::to_string(i) ;
            so_std::string_t const name = "u_smp_" + i_s + "_on_layer_" + l_s ;

            // @todo sampler type is to be determined by 
            // the input texture variable form the material
            expr += "uniform sampler2D " + name + " ;\n" ;
            {
                so_shade::so_shader::texture_binding bind ;
                bind.var.name = name ;
                bind.var.of_type.base = so_shade::type_base::tfloat ;
                bind.var.of_type.dim = so_shade::texture_dimension::dim_2d ;
                bind.var.of_type.encoding = so_shade::texture_encoding::color ;
                ein.get_pixel_shader()->add( bind ) ;

                ein.add_pixel_shader_binding( bind.var, user_var.name ) ;
            }
        }
    }

    return std::move( expr ) ;
}

//*************************************************************************************
so_std::string_t effect_generator::generate_glsl_es1_defines_from( 
    so_shade::so_material::standard_material_ptr_t ) 
{
    so_std::string_t expr ;

    if( _gen_props.enable_linear_depth )
    {
        expr += "#define __near "+ _gen_props.projection_param_0_name +".x \n" ;
        expr += "#define __far "+ _gen_props.projection_param_0_name +".y \n" ;
    }
    

    return std::move(expr) ;
}

//*************************************************************************************
so_std::string_t effect_generator::generate_glsl_es1_texcoord_varyings_from(
    so_shade::so_material::standard_material_ptr_t mat_ptr ) 
{
    if( mat_ptr->get_num_texture_layers() == 0 )
    {
        return so_std::string_t("// [GEN] : no texture layers - no texcoords \n") ;
    }

    so_std::string_t expr ;

    for( size_t i=0; i<mat_ptr->get_num_texture_layers(); ++i )
    {
        so_std::string_t const i_s = std::to_string(i) ;
        expr += "varying vec2 var_txc_" + i_s + " ;\n" ;
    }

    return std::move(expr) ;
}

//*************************************************************************************
so_std::string_t effect_generator::generate_glsl_es1_normal_varyings_from(
    so_shade::so_material::standard_material_ptr_t mat_ptr ) 
{
    if( so_core::is_not( mat_ptr->has_normal_layer() ) )
    {
        return so_std::string_t("// [GEN] : no normals. \n") ;
    }

    so_std::string_t expr ;

    expr += "varying vec3 var_nrm ;\n" ;

    return std::move(expr) ;
}

//*************************************************************************************
so_std::string_t effect_generator::generate_glsl_es1_light_varyings_from(
    so_shade::so_material::standard_material_ptr_t mat_ptr ) 
{
    // @todo check lights
    if( so_core::is_not( mat_ptr->has_normal_layer() ) )
    {
        return so_std::string_t( "// [GEN] : no normals - no lights \n" ) ;
    }

    // foreach light
    so_std::string_t expr ;
    expr += "varying vec3 var_light_dir ; \n" ;
    return expr ;
}

//*************************************************************************************
so_std::string_t effect_generator::generate_glsl_es1_linear_z_varyings_from(
    so_shade::so_material::standard_material_ptr_t ) 
{
    so_std::string_t expr ;

    if( _gen_props.enable_linear_depth )
    {
        expr += "varying float var_linear_z ; \n" ;
    }
        

    return std::move( expr ) ;
}

//*************************************************************************************
so_std::string_t effect_generator::generate_glsl_es1_texcoord_write_through_from(
    so_shade::so_material::standard_material_ptr_t mat_ptr ) 
{
    if( mat_ptr->get_num_texture_layers() == 0 )
    {
        return so_std::string_t("// [GEN] : no texcoords. \n") ;
    }

    so_std::string_t expr ;

    expr += "// [GEN] : texcoord write through \n" ;
    for( size_t i = 0; i < mat_ptr->get_num_texture_layers(); ++i )
    {
        so_math::vec2f_t uv_scale ;
        so_math::vec2f_t uv_offset ;

        if( so_core::is_not( mat_ptr->get_texture_layer_coord_info(i, uv_scale, uv_offset) ) ) continue ;

        so_std::string_t const i_s = std::to_string( i ) ;
        expr += "var_txc_" + i_s + " = ("+ _gen_props.texcoord_pre_name + (i>0 ? i_s : "") + " " ;
        expr += " + vec2( " + std::to_string(uv_offset.x())+", " + std::to_string(uv_offset.y()) + "))"  ;
        expr += " * vec2( " + std::to_string(uv_scale.x())+", " + std::to_string(uv_scale.y()) + ")"  ;
        
        expr += " ; \n" ;
    }
    
    return std::move( expr ) ;
}

//*************************************************************************************
so_std::string_t effect_generator::generate_glsl_es1_normal_write_through_from(
    so_shade::so_material::standard_material_ptr_t mat_ptr ) 
{
    if( so_core::is_not(mat_ptr->has_normal_layer()) )
    {
        return so_std::string_t("// [GEN] : no normal layer. \n") ;
    }

    so_std::string_t expr ;

    {
        so_std::string_t const obj_name = _gen_props.object_matrix_name ;
        so_std::string_t const wrl_name = _gen_props.world_matrix_name ;
        so_std::string_t const view_name = _gen_props.view_matrix_name ;
        so_std::string_t const nrm_name = _gen_props.normal_name ;
                

        expr += "// [GEN] : view space normal \n" ;
        expr += "var_nrm = vec4( " + view_name + " * " + wrl_name ;
        
        if( _gen_props.enable_object_matrix )
        {
            expr += " * " + obj_name ;
        }
        
        expr += " * vec4( " + nrm_name + ", 0.0 ) ).xyz ; \n" ;
    }
    

    return std::move( expr ) ;
}

//*************************************************************************************
so_std::string_t effect_generator::generate_glsl_es1_light_write_through_from(
    so_shade::so_material::standard_material_ptr_t mat_ptr ) 
{
    if( so_core::is_not( mat_ptr->has_normal_layer() ) )
    {
        return so_std::string_t( "// [GEN] : no normal layer - no light \n" ) ;
    }

    so_std::string_t expr ;
    expr += "// [GEN] : dummy light \n" ;

    // @todo for each light dir
    {
        so_std::string_t const obj_name = _gen_props.object_matrix_name ;
        so_std::string_t const wrl_name = _gen_props.world_matrix_name ;
        so_std::string_t const view_name = _gen_props.view_matrix_name ;

        so_std::string_t const pos_name = _gen_props.position_name ;

        expr += "vec3 light_wrl_pos = vec3(0,1000,0) ;\n" ;
        expr += "vec3 to_light = light_wrl_pos - vec4( " + wrl_name ;
        
        
        if( _gen_props.enable_object_matrix )
        {
            expr += " * " + obj_name ;
        }

        expr += "* vec4( " + pos_name + ", 1 ) ).xyz ; \n" ;

        expr += "var_light_dir = vec4( "+view_name+" * vec4( normalize( to_light ), 0.0 ) ).xyz ;\n" ;
    }

    return std::move( expr ) ;
}

//*************************************************************************************
so_std::string_t effect_generator::generate_glsl_es1_linear_z_write_through_from(
    so_shade::so_material::standard_material_ptr_t ) 
{
    so_std::string_t expr ;

    if( _gen_props.enable_linear_depth )
    {
        expr += "// [GEN] : calculate linear z \n" ;
        expr += "var_linear_z = (view_pos.z - __near) / (__far - __near) ; \n " ;
    }
    

    return std::move( expr ) ;
}

//*************************************************************************************
so_std::string_t effect_generator::generate_glsl_es1_texture_access_from(
    so_shade::so_material::standard_material_ptr_t mat_ptr ) 
{
    so_std::string_t expr ;

    for( size_t l=0; l<mat_ptr->get_num_texture_layers(); ++l )
    {
        so_std::string_t const l_s = std::to_string(l) ;
        for( size_t i=0; i<mat_ptr->get_num_textures(l); ++i )
        {
            so_std::string_t const i_s = std::to_string(i) ;
            so_std::string_t const smp_name = "u_smp_" + i_s + "_on_layer_" + l_s ;
            so_std::string_t const var_name = "color_" + i_s + "_on_" + l_s ;

            expr += "vec4 "+ var_name + " = texture2D( " + smp_name + ", var_txc_"+l_s+" ) ;\n" ;
        }        
    }

    return std::move( expr ) ;
}

//*************************************************************************************
so_std::string_t effect_generator::generate_glsl_es1_albedo_output_from(
    so_shade::so_material::standard_material_ptr_t mat_ptr ) 
{
    size_t div_parts = 0 ;

    so_std::string_t expr ;
    so_std::string_t tex_expr ;
    
    bool_t const has_texture_layers = mat_ptr->get_num_texture_layers() > 0 ;
    bool_t const has_color_layers = mat_ptr->get_num_color_layers() > 0 ;

    // textures
    if( has_texture_layers ) 
    {        
        tex_expr += "vec4 acc_tex_color = " ;

        float_t const mix_val_layer = 1.0f / float_t( mat_ptr->get_num_texture_layers() ) ;
        so_std::string_t const mix_layer_s = std::to_string( mix_val_layer ) ;
        for( size_t l = 0; l < mat_ptr->get_num_texture_layers(); ++l )
        {
            if( mat_ptr->get_num_textures( l ) == 0 ) continue ;

            so_std::string_t inner_expr ;

            float_t const mix_val = 1.0f / float_t( mat_ptr->get_num_textures( l ) ) ;
            so_std::string_t const mix_s = std::to_string( mix_val ) ;
            so_std::string_t const l_s = std::to_string( l ) ;

            for( size_t i = 0; i < mat_ptr->get_num_textures( l ); ++i )
            {
                so_std::string_t const i_s = std::to_string( i ) ;

                inner_expr += "vec4(" + mix_s + ") * color_" + i_s + "_on_" + l_s + " + " ;
            }
            inner_expr += "vec4(0)";

            tex_expr += "( " + inner_expr + " ) * vec4(" + mix_layer_s + ") + " ;
        }
        tex_expr += "vec4(0) ; \n\n" ;

        div_parts += 1 ;
    }
    expr += tex_expr ;   

    // lights
    // @todo light code needs to be done based on affecting lights
    // lights are not part of materials
    /*{
        expr += "// [GEN] : do light color composition\n" ;
        if( mat_ptr->has_normal_layer() )
        {
            // if has light
            {
                expr += "vec4 sky_color = vec4(0.3,0.3,1.0,1.0) ; \n" ;
                expr += "vec4 bottom_color = vec4(0.6,0.3,0.3,1.0) ; \n" ;
                expr += "float ndl = dot(var_nrm, normalize(var_light_dir)) ; \n" ;
                expr += "vec4 light_color = mix( bottom_color, sky_color, ndl ) ; \n\n" ;
                // Test code
                //expr += "out_color = vec4(ndl) ;\n" ;
                //expr += "out_color = vec4(normalize(var_light_dir),1.0) ;\n" ;
                //expr += "out_color = vec4(var_nrm,1.0) ;\n" ;
            }
            final_division += 1 ;
        }
    }*/

    if( has_color_layers ) 
    {
        so_std::string_t inner_expr = "vec4 const_color = ( " ;

        float_t layer_div = 0.0f ;
        for( size_t i = 0; i < mat_ptr->get_num_color_layers(); ++i )
        {
            so_math::vec3f_t color ;
            if( so_core::is_not( mat_ptr->get_color_layer(i, color) ) ) continue ;

            inner_expr += "vec4(" ;
            inner_expr += this_t::convert_vec3_to_glsl( color ) ;
            inner_expr += ", 0.0) + " ;

            layer_div += 1.0f ;
        }
        inner_expr += "vec4(0.0)" ;
        inner_expr += ") * vec4(" + std::to_string(1.0f/layer_div) + ") ;\n\n " ;
        
        expr += "// [GEN] : do constant color composition\n" ;
        expr += inner_expr ;

        div_parts += 1 ;
    }

    {
        expr += "// [GEN] : do final color composition\n" ;
        div_parts = div_parts == 0 ? 1 : div_parts ;
        float_t final_division = 1.0f / float_t(div_parts) ;
        expr += "return ( " ;
        if( has_texture_layers ) 
            expr += "acc_tex_color +" ;
        else 
            expr += "vec4(0) +" ;

        if( has_color_layers )
            expr += "const_color +" ;
        else
            expr += "vec4(0) +" ;
            
        expr += " vec4(0) ) " ;
        expr += "* vec4(" + std::to_string( final_division ) + ") ; \n\n" ;
        //expr = "return vec4(var_txc_0,0.0,1.0) ;" ;
    }

    return std::move( expr ) ;
}

//*************************************************************************************
so_std::string_t effect_generator::generate_glsl_es1_linear_z_output_from( 
    so_shade::so_material::standard_material_ptr_t mat_ptr ) 
{
    so_std::string_t expr ;
    if( _gen_props.enable_linear_depth )
    {
        expr += "gl_FragDepth = var_linear_z ; \n" ;
    }
    
    return std::move( expr ) ;
}

//*************************************************************************************
so_std::string_t effect_generator::generate_glsl_es1_pixel_shader_call_albedo_from( void_t ) 
{
    so_std::string_t expr ;

    expr = "vec4 albedo_color = albedo_color_funk() ; \n" ;
    return std::move( expr ) ;
}

//*************************************************************************************
so_std::string_t effect_generator::generate_glsl_es1_obj_to_view_pos_transform_from( 
    so_shade::so_material::standard_material_ptr_t ) 
{
    so_std::string_t expr ;

    {
        so_std::string_t const obj_name = _gen_props.object_matrix_name ;
        so_std::string_t const wrl_name = _gen_props.world_matrix_name ;
        so_std::string_t const view_name = _gen_props.view_matrix_name ;
        so_std::string_t const pos_name = _gen_props.position_name ;
        
        expr += "// [GEN] : object space to view space\n" ;
        expr += "vec3 view_pos = vec4( " + view_name + " * " + wrl_name ;
            
        if( _gen_props.enable_object_matrix )
        {
            expr += " * " + obj_name ;
        }
        
        expr += " * vec4( " + pos_name + ", 1.0 ) ).xyz ; \n" ;
    }

    return std::move( expr ) ;
}

//*************************************************************************************
so_std::string_t effect_generator::generate_glsl_es1_view_to_proj_pos_transform_from(
    so_shade::so_material::standard_material_ptr_t ) 
{
    so_std::string_t expr ;

    if( _gen_props.enable_mvp_matrix )
    {
        so_std::string_t const mvp_name = _gen_props.mvp_matrix_name ;
        so_std::string_t const pos_name = _gen_props.position_name  ;
        expr += "// [GEN] : object space to projection space\n" ;
        expr += "gl_Position = "+mvp_name+" * vec4( vec3("+pos_name+") , 1.0 ) ; \n" ;
    }
    else
    {
        so_std::string_t const proj_name = _gen_props.projection_matrix_name ;
        expr += "// [GEN] : view space to projection space\n" ;
        expr += "gl_Position = "+proj_name+" * vec4( view_pos, 1.0 ) ; \n" ;
    }
    

    return std::move(expr) ;
}

//*************************************************************************************
so_std::string_t effect_generator::variable_to_string_glsl33( so_shade::variable_cref_t var ) 
{
    so_std::string_t expr ;

    so_shade::type_base const tb = var.of_type.get_type_base() ;
    so_shade::type_struct const ts = var.of_type.get_type_struct() ;

    switch( tb )
    {
    case type_base::invalid:
        expr += "invalid_type";
        break ;
    case type_base::tbyte:
        expr += "byte_is_unsupported";
        break ;
    case type_base::tint:
        switch( ts )
        {
        case type_struct::invalid: expr += "invalid_type_struct" ; break ;
        case type_struct::vector1: expr += "int" ; break ;
        case type_struct::vector2: expr += "ivec2" ; break ;
        case type_struct::vector3: expr += "ivec3" ; break ;
        case type_struct::vector4: expr += "ivec4" ; break ;
        case type_struct::matrix2: expr += "invalid_type_struct" ; break ;
        case type_struct::matrix3: expr += "invalid_type_struct" ; break ;
        case type_struct::matrix4: expr += "invalid_type_struct" ; break ;
        default: expr += "unhandled type_struct" ;
        }
        break ;
    case type_base::tuint:
        switch( ts )
        {
        case type_struct::invalid: expr += "invalid_type_struct" ; break ;
        case type_struct::vector1: expr += "uint" ; break ;
        case type_struct::vector2: expr += "uvec2" ; break ;
        case type_struct::vector3: expr += "uvec3" ; break ;
        case type_struct::vector4: expr += "uvec4" ; break ;
        case type_struct::matrix2: expr += "invalid_type_struct" ; break ;
        case type_struct::matrix3: expr += "invalid_type_struct" ; break ;
        case type_struct::matrix4: expr += "invalid_type_struct" ; break ;
        default: expr += "unhandled type_struct" ;
        }
        break ;
    case type_base::tfloat:
        switch( ts )
        {
        case type_struct::invalid: expr += "invalid_type_struct" ; break ;
        case type_struct::vector1: expr += "float" ; break ;
        case type_struct::vector2: expr += "vec2" ; break ;
        case type_struct::vector3: expr += "vec3" ; break ;
        case type_struct::vector4: expr += "vec4" ; break ;
        case type_struct::matrix2: expr += "mat2" ; break ;
        case type_struct::matrix3: expr += "mat3" ; break ;
        case type_struct::matrix4: expr += "mat4" ; break ;
        default: expr += "unhandled type_struct" ;
        }
        break ;
    case type_base::tdouble:
        switch( ts )
        {
        case type_struct::invalid: expr += "invalid_type_struct" ; break ;
        case type_struct::vector1: expr += "double_unsupported" ; break ;
        case type_struct::vector2: expr += "double_unsupported" ; break ;
        case type_struct::vector3: expr += "double_unsupported" ; break ;
        case type_struct::vector4: expr += "double_unsupported" ; break ;
        case type_struct::matrix2: expr += "double_unsupported" ; break ;
        case type_struct::matrix3: expr += "double_unsupported" ; break ;
        case type_struct::matrix4: expr += "double_unsupported" ; break ;
        default: expr += "unhandled_type_struct" ;
        }
        break ;
    
    }

    return std::move( expr ) ;
}

//*************************************************************************************
so_std::string_t effect_generator::texture_variable_to_string_glsl33( 
    so_shade::texture_variable_cref_t var ) 
{
    so_std::string_t expr ;

    so_shade::type_base const tb = var.of_type.get_type_base() ;
    so_shade::texture_dimension const td = var.of_type.get_dimension() ;
    so_shade::texture_encoding const te = var.of_type.get_encoding() ;

    switch( te )
    {
        //
        // Color
        //
        case so_shade::texture_encoding::color: 
        switch(td)
        {
        case so_shade::texture_dimension::dim_1d: 
        switch( tb )
        {
        case so_shade::type_base::tint: expr += "isampler1D_unsuppored"; break ;
        case so_shade::type_base::tuint: expr += "uisampler1D_unsuppored"; break ;
        case so_shade::type_base::tfloat: expr += "sampler1D_unsuppored"; break ;
        default: expr += "unsuppored_texture_type"; break ;
        }
        break ;

        case so_shade::texture_dimension::dim_2d: 
        switch( tb )
        {
        case so_shade::type_base::tint: expr += "isampler2D"; break ;
        case so_shade::type_base::tuint: expr += "uisampler2D"; break ;
        case so_shade::type_base::tfloat: expr += "sampler2D"; break ;
        default: expr += "unsuppored_texture_type"; break ;
        }
        break ;

        case so_shade::texture_dimension::dim_3d: 
        switch( tb )
        {
        case so_shade::type_base::tint: expr += "isampler3D"; break ;
        case so_shade::type_base::tuint: expr += "uisampler3D"; break ;
        case so_shade::type_base::tfloat: expr += "sampler3D"; break ;
        default: expr += "unsuppored_texture_type"; break ;
        }
        break ;

        case so_shade::texture_dimension::dim_cube: 
        switch( tb )
        {
        case so_shade::type_base::tint: expr += "isamplerCube"; break ;
        case so_shade::type_base::tuint: expr += "uisamplerCube"; break ;
        case so_shade::type_base::tfloat: expr += "samplerCube"; break ;
        default: expr += "unsuppored_texture_type"; break ;
        }
                                                     break ;
        
        default: expr += "unhandled_texture_dimension"; break ;
        }
        break ;

        //
        // Depth
        //
        case so_shade::texture_encoding::depth:
        switch( td )
        {
        case so_shade::texture_dimension::dim_1d:
        switch( tb )
        {
        case so_shade::type_base::tint: expr += "isampler1DShadow_unsuppored"; break ;
        case so_shade::type_base::tuint: expr += "uisampler1DShadow_unsuppored"; break ;
        case so_shade::type_base::tfloat: expr += "sampler1DShadow_unsuppored"; break ;
        default: expr += "unsuppored_texture_type"; break ;
        }
        break ;

        case so_shade::texture_dimension::dim_2d:
        switch( tb )
        {
        case so_shade::type_base::tint: expr += "isampler2DShadow_unsuppored"; break ;
        case so_shade::type_base::tuint: expr += "uisampler2DShadow_unsuppored"; break ;
        case so_shade::type_base::tfloat: expr += "sampler2DShadow"; break ;
        default: expr += "unsuppored_texture_type"; break ;
        }
        break ;

        case so_shade::texture_dimension::dim_3d:
        switch( tb )
        {
        case so_shade::type_base::tint: expr += "isampler3DShadow_unsuppored"; break ;
        case so_shade::type_base::tuint: expr += "uisampler3DShadow_unsuppored"; break ;
        case so_shade::type_base::tfloat: expr += "sampler3DShadow_unsuppored"; break ;
        default: expr += "unsuppored_texture_type"; break ;
        }
        break ;

        case so_shade::texture_dimension::dim_cube:
        switch( tb )
        {
        case so_shade::type_base::tint: expr += "isamplerCubeShadow_unsuppored"; break ;
        case so_shade::type_base::tuint: expr += "uisamplerCubeShadow_unsuppored"; break ;
        case so_shade::type_base::tfloat: expr += "samplerCubeShadow"; break ;
        default: expr += "unsuppored_texture_type"; break ;
        }
        break ;

        default: expr += "unhandled_texture_dimension"; break ;
        }
        break ;
    }

    return std::move( expr ) ;
}

//*************************************************************************************
so_std::string_t effect_generator::convert_vec3_to_glsl( so_math::vec3f_cref_t vin ) const 
{
    so_std::string_t expr ;

    expr += "vec3( " ;
    expr += std::to_string( vin.x() ) + "," ;
    expr += std::to_string( vin.y() ) + "," ;
    expr += std::to_string( vin.z() ) + ")" ;

    return std::move( expr ) ;
}
