//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "gl_33_api.h"

#include "../gl_any/objects/vertex_shader.h"
#include "../gl_any/objects/geometry_shader.h"
#include "../gl_any/objects/pixel_shader.h"
#include "../gl_any/objects/program.h"
#include "../gl_any/objects/shader_variable.h"

#include "../gl_any/converter/gl_convert_feedback_buffer_type.h"
#include "../gl_any/converter/gl_convert_feedback_primitve_type.h"
#include "../gl_any/converter/gl_convert_uniform_type.h"
#include "../gl_any/converter/gl_convert_geometry_shader_type.h"

#include "../../../shader/vertex_shader.h"
#include "../../../shader/pixel_shader.h"
#include "../../../program/program.h"
#include "../../../variable/shader_variable.h"

#include "../gl_any/log.h" // gl

#include <snakeoil/gli/gl/gl.h>
#include <snakeoil/std/string/split.hpp>
#include <snakeoil/memory/guards/malloc_guard.hpp>

using namespace so_gpu ;
using namespace so_gpu::so_gl ;

namespace drv = so_gpu::so_gl ;

using gl_log = so_gpu::so_gl::log ;

//**********************************************************************************************
so_gpu::result gl_33_api::detatach_shaders( GLuint program_id )
{
    GLsizei count = 0 ;
    GLuint shaders[ 10 ] ;
    
    so_gli::gl::glGetAttachedShaders( program_id, 10, &count, shaders ) ;
    if( gl_log::error( "[gl_33_api::detatach_shaders] : glGetAttachedShaders" ) )
        return so_gpu::failed_gl_api ;

    for( GLsizei i = 0; i < count; ++i )
    {
        so_gli::gl::glDetachShader( program_id, shaders[ i ] ) ;
    }
    
    return so_gpu::ok ;
}

//**********************************************************************************************
so_gpu::result gl_33_api::prepare_vertex_shader( GLuint program_id, so_gpu::program_ptr_t program_ptr ) 
{
    if( so_log::global::error( program_ptr->has_no_vertex_shader(), "[gl_33_api::prepare_vertex_shader] : vertex shader required." ) ) 
        return so_gpu::failed ;

    auto * shd_ptr = program_ptr->get_vertex_shader() ;
    if( so_log::global::error( api_object_helper_t::has_no_driver_object(shd_ptr), "[gl_33_api::prepare_vertex_shader] : no driver object in shader" ) ) 
        return so_gpu::failed ;

    so_gli::gl::glAttachShader( program_id, api_object_helper_t::get_cast_api_object<drv::vertex_shader>( shd_ptr )->id ) ;
    if( gl_log::error( "[gl_33_api::prepare_vertex_shader] : glAttachShader" ) )
        return so_gpu::failed_gl_api ;

    return so_gpu::ok ;
}

//**********************************************************************************************
so_gpu::result gl_33_api::prepare_geometry_shader( GLuint program_id, so_gpu::program_ptr_t program_ptr ) 
{
    if( program_ptr->has_no_geometry_shader() )
        return so_gpu::ok ;
    
    auto * shd_ptr = program_ptr->get_geometry_shader() ;
    if( so_log::global::error( api_object_helper_t::has_no_driver_object(shd_ptr), "[gl_33_api::prepare_geometry_shader] : no driver object in shader" ) ) 
        return so_gpu::failed ;

    // check max output vertices
    {
        GLint max_out = 0 ;
        so_gli::gl::glGetIntegerv( GL_MAX_GEOMETRY_OUTPUT_VERTICES, &max_out) ;
        if( gl_log::error("[gl_33_api::prepare_geometry_shader] : glGetIntegerv - GL_MAX_GEOMETRY_OUTPUT_VERTICES") ) 
            return so_gpu::failed_gl_api ;

        if( max_out < GLint(shd_ptr->get_num_output_vertices()) )
        {
            shd_ptr->set_num_output_vertices( size_t(max_out) ) ;
            so_log::global::warning( "[gl_33_api::prepare_geometry_shader] : Max output vertices invalid.") ;
        }
    }
    
    so_gli::gl::glProgramParameteri(program_id, GL_GEOMETRY_INPUT_TYPE, (GLint)so_gl::convert(shd_ptr->get_input_type()) ) ;
    if( gl_log::error("[gl_33_api::prepare_geometry_shader] : glProgramParameteri - GL_GEOMETRY_INPUT_TYPE_ARB") ) 
        return so_gpu::failed_gl_api ;
    
    so_gli::gl::glProgramParameteri(program_id, GL_GEOMETRY_OUTPUT_TYPE, (GLint)so_gl::convert(shd_ptr->get_output_type()) ) ;
    if( gl_log::error("[gl_33_api::prepare_geometry_shader] : glProgramParameteri - GL_GEOMETRY_OUTPUT_TYPE_ARB") ) 
        return so_gpu::failed_gl_api ;
    
    so_gli::gl::glProgramParameteri(program_id, GL_GEOMETRY_VERTICES_OUT, (GLint)shd_ptr->get_num_output_vertices()) ;
    if( gl_log::error("[gl_33_api::prepare_geometry_shader] : glProgramParameteri - GL_GEOMETRY_VERTICES_OUT_ARB") ) 
        return so_gpu::failed_gl_api ;
    
    so_gli::gl::glAttachShader( program_id, api_object_helper_t::get_cast_api_object<drv::geometry_shader>(shd_ptr)->id ) ;
    if( gl_log::error("[gl_33_api::prepare_geometry_shader] : glAttachShader") ) 
        return so_gpu::failed_gl_api ;
    
    return so_gpu::ok ;
}

//**********************************************************************************************
so_gpu::result gl_33_api::prepare_pixel_shader( GLuint program_id, so_gpu::program_ptr_t program_ptr ) 
{
    // in transform feedback, this is allowed.
    if( program_ptr->has_no_pixel_shader() )
        return so_gpu::ok ;

    auto * shd_ptr = program_ptr->get_pixel_shader() ;
    if( so_log::global::error( api_object_helper_t::has_no_driver_object(shd_ptr), "[gl_33_api::prepare_pixel_shader] : no driver object in shader" ) ) 
        return so_gpu::failed ;

    so_gli::gl::glAttachShader( program_id, api_object_helper_t::get_cast_api_object<drv::pixel_shader>(shd_ptr)->id ) ;
    if( gl_log::error("[gl_33_api::prepare_pixel_shader] : glAttachShader") ) 
        return so_gpu::failed_gl_api ;

    return so_gpu::ok ;
}

//**********************************************************************************************
so_gpu::result gl_33_api::pre_link_transform_feedback( GLuint program_id, so_gpu::program_ptr_t program_ptr ) 
{
    if( program_ptr->has_no_feedback_names() ) return so_gpu::ok ;

    size_t const num_names = program_ptr->get_num_feedback_names() ;
    so_memory::malloc_guard<const char*> names(num_names) ;
    
    for( size_t i=0; i<num_names; ++i )
    {
        names[i] = program_ptr->get_feedback_name(i).c_str() ;
    }

    so_gli::gl::glTransformFeedbackVaryings( program_id, GLsizei(num_names), names, 
        so_gpu::so_gl::convert(program_ptr->get_feedback_type()) ) ;
    
    if( gl_log::error("[gl_system::prepare_transform_feedback] glTransformFeedbackVaryings") ) 
        return so_gpu::failed_gl_api ;    

    return so_gpu::ok ;
}

//**********************************************************************************************
so_gpu::result gl_33_api::post_link_attributes( GLuint program_id, so_gpu::program_ptr_t program_ptr ) 
{
    GLint num_active_attributes = 0 ;
    GLint name_length = 0 ;

    so_gli::gl::glGetProgramiv( program_id, GL_ACTIVE_ATTRIBUTES, &num_active_attributes ) ;
    if( so_gpu::so_gl::log::error("glGetProgramiv failed: GL_ACTIVE_ATTRIBUTES") ) 
        return so_gpu::failed_gl_api ;    
    
    if( num_active_attributes == 0 ) return so_gpu::ok ;

    so_gli::gl::glGetProgramiv( program_id, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &name_length ) ;
    if( so_gpu::so_gl::log::error("glGetProgramiv failed: GL_ACTIVE_ATTRIBUTE_MAX_LENGTH") ) 
        return so_gpu::failed_gl_api ;    

    GLint size ;
    GLenum gl_attrib_type ;

    so_memory::malloc_guard<char> buffer(name_length) ;
    
    for( GLint i=0; i<num_active_attributes; ++i )
    {
        so_gli::gl::glGetActiveAttrib( program_id, i, name_length, 0, &size, &gl_attrib_type, buffer ) ;
        if( so_gpu::so_gl::log::error("glGetActiveAttrib failed. continue loop.") ) continue ;

        // build-ins are not needed in the shader variables container.
        if( name_length >=3 && buffer.equals(0, 'g') && buffer.equals(1, 'l') && buffer.equals(2, '_') ) continue ;

        GLuint location_id = so_gli::gl::glGetAttribLocation(program_id, buffer) ;
        if( so_gpu::so_gl::log::error("glGetAttribLocation failed. continue loop.") ) continue ;
        
        std::string variable_name = std::string((const char*)buffer) ;

        auto * var_ptr = this_t::create_shader_variable( location_id, gl_attrib_type, variable_name ) ;
        if( var_ptr == nullptr ) continue ;

        program_ptr->add_input_variable( var_ptr ) ;
    }
    return so_gpu::ok ;
}

//**********************************************************************************************
so_gpu::result gl_33_api::post_link_uniforms( GLuint program_id, so_gpu::program_ptr_t program_ptr ) 
{
    GLint num_active_uniforms = 0 ;
    GLint name_length = 0 ;

    so_gli::gl::glGetProgramiv( program_id, GL_ACTIVE_UNIFORMS, &num_active_uniforms ) ;
    if( gl_log::error("[glGetProgramiv] : GL_ACTIVE_UNIFORMS") ) return so_gpu::failed_gl_api ;    
    
    if( num_active_uniforms == 0 ) return so_gpu::ok ;

    so_gli::gl::glGetProgramiv( program_id, GL_ACTIVE_UNIFORM_MAX_LENGTH, &name_length ) ;
    if( gl_log::error("[glGetProgramiv] : GL_ACTIVE_UNIFORM_MAX_LENGTH") ) return so_gpu::failed_gl_api ;    
    
    GLint size ;
    GLenum gl_uniform_type ;

    so_memory::malloc_guard<char> buffer(name_length) ;
        
    for( GLint i=0; i<num_active_uniforms; ++i )
    {
        so_gli::gl::glGetActiveUniform( program_id, i, name_length, 0, &size, &gl_uniform_type, buffer ) ;
        if( gl_log::error("[gl_33_api] : glGetActiveUniform") ) continue ;

        // build-ins are not needed in the shader variables container.
        if( name_length >=3 && buffer.equals(0,'g') && buffer.equals(1,'l') && buffer.equals(2,'_') ) continue ;

        GLuint location_id = so_gli::gl::glGetUniformLocation( program_id, buffer ) ;
        if( gl_log::error("[glGetUniformLocation]") ) continue ;

        if( so_log::global::error( location_id == GLuint(-1), "invalid uniform location id." ) ) continue ;
        
        std::string variable_name = std::string(char_cptr_t(buffer)) ;

        auto * var_ptr = this_t::create_shader_variable( location_id, gl_uniform_type, variable_name ) ;
        if( var_ptr == nullptr ) continue ;

        program_ptr->add_shader_variable( var_ptr ) ;
    }
    
    return so_gpu::ok ;
}

//**********************************************************************************************
shader_variable_ptr_t gl_33_api::create_shader_variable( GLuint location, GLenum gltype,  std::string const & name ) 
{
    shader_variable_type type = so_gpu::so_gl::convert_uniform_type( gltype ) ;
    
    if( so_log::global::error( type == shader_variable_type::undefined, "unsupported shader variable type") )
        return nullptr ;

    auto * var_ptr = so_gpu::memory::alloc( so_gpu::shader_variable( name, type ),
        "[gl_33_api::create_shader_variable] : so_gpu::shader_variable" ) ;

    api_object_helper_t::set_driver_object( var_ptr, 
        so_gpu::memory::alloc( drv::shader_variable(location),
        "[gl_33_api::create_shader_variable] : gl::shader_variable") ) ;

    return var_ptr ;
}

//**********************************************************************************************
void_t gl_33_api::delete_all_shader_variables( so_gpu::program_ptr_t prog_ptr ) 
{
    {
        auto variables = prog_ptr->move_out_input_variables() ;
        for( auto var : variables )
        {
            this_t::delete_shader_variable( var ) ;
        }
    }
    {
        auto variables = prog_ptr->move_out_shader_variables() ;
        for( auto var : variables )
        {
            this_t::delete_shader_variable( var ) ;
        }
    }
}

//**********************************************************************************************
void_t gl_33_api::delete_shader_variable( so_gpu::shader_variable_ptr_t var_ptr ) 
{
    if( var_ptr == nullptr ) return ;
    
    auto * api_obj = api_object_helper_t::get_cast_api_object<drv::shader_variable>(var_ptr) ;
    // a gl shader variable can be deleted right away. It does not contain
    // manually created gl resources.
    so_gpu::memory::dealloc( api_obj ) ;
    api_object_helper_t::set_driver_object( var_ptr, nullptr ) ;

    so_gpu::memory::dealloc( var_ptr ) ;
}

//**********************************************************************************************
so_gpu::result gl_33_api::link( so_gpu::program_ptr_t ptr ) 
{
    if( so_gpu::no_success( this_t::create_program( ptr ) ) )
        return so_gpu::result::failed_shader_link ;

    auto * api_obj = api_object_helper_t::get_cast_api_object<drv::program>(ptr) ;
    
    GLuint program_id = api_obj->id ; 

    this_t::detatach_shaders( program_id ) ;

    this_t::prepare_vertex_shader( program_id, ptr ) ;
    this_t::prepare_geometry_shader( program_id, ptr ) ;
    this_t::prepare_pixel_shader( program_id, ptr ) ;
    
    this_t::pre_link_transform_feedback( program_id, ptr ) ;

    so_gli::gl::glLinkProgram(program_id) ;
    if( gl_log::error("[gl_33_api::link] : glLinkProgram") ) 
        return so_gpu::failed_gl_api ;

    if( so_gpu::no_success( this_t::check_link(program_id) ) ) 
        return so_gpu::failed_gl_api ;

    post_link_attributes( program_id, ptr ) ;
    post_link_uniforms( program_id, ptr ) ;

    /// this is required because this program could
    /// have been set to a config before the link is done.
    ptr->validate_connected_configs() ;

    return so_gpu::ok ;
}

//**********************************************************************************************
so_gpu::result gl_33_api::check_link( GLuint program_id ) 
{
    GLint ret ;
    so_gli::gl::glGetProgramiv( program_id, GL_LINK_STATUS, &ret ) ;

    GLint length ;
    so_gli::gl::glGetProgramiv( program_id, GL_INFO_LOG_LENGTH, &length ) ;

    if( ret == GL_TRUE && length <= 1 ) return so_gpu::ok ;
    if( so_log::global::error( length == 0, "[gl_33_api::check_link] : unknown") ) 
        return so_gpu::failed_gl_api ;
    
    so_memory::malloc_guard<char> info_log(length) ;
        
    so_gli::gl::glGetProgramInfoLog( program_id, length, 0, info_log ) ;
    if( so_gpu::so_gl::log::error("[gl_33_api::check_link] : glGetProgramInfoLog") ) 
        return so_gpu::failed_gl_api ;
    
    std::string info_log_string = std::string((const char*)info_log) ;

    {
        so_std::vector<so_std::string_t> tokens ;
        so_std::string_ops::split( so_std::string_t( info_log ), '\n', tokens ) ;

        for( auto token : tokens )
        {
            so_log::global::error( token ) ;
        }
    }

    return so_gpu::failed_shader_link ;
}

