//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "gl_33_api.h"

#include "../gl_any/objects/variable_set.h"
#include "../gl_any/objects/variable.h"

#include "../../../variable/variable_set.h"
#include "../../../variable/variable.h"

#include "../gl_any/log.h"

using namespace so_gpu ;
using namespace so_gpu::so_gl ;

namespace drv = so_gpu::so_gl ;

using gl_log = so_gpu::so_gl::log ;

//*****************************************************************************************************
so_gpu::result gl_33_api::create_variable( so_gpu::variable_set_ptr_t ptr ) 
{
    if( api_object_helper_t::has_driver_object( ptr ) ) return so_gpu::ok ;

    api_object_helper_t::set_driver_object( ptr, so_gpu::memory::alloc( 
        drv::variable_set(), "[gl_33_api::create_variable]" ) ) ;

    return so_gpu::ok ;
}

//*****************************************************************************************************
so_gpu::result gl_33_api::release_variable( so_gpu::variable_set_ptr_t ptr ) 
{
    if( api_object_helper_t::has_no_driver_object( ptr ) ) return so_gpu::ok ;
    auto * api_obj = api_object_helper_t::get_cast_api_object<drv::variable_set>(ptr) ;
    
    for( auto item : ptr->data_variables() )
    {
        if( so_gpu::no_success( this_t::release_variable( item.first ) ) )
        {
            so_log::log::error("[gl_33_api::release_variable] : data_variables") ;
            continue ;
        }
    }

    for( auto item : ptr->texture_variables() )
    {
        if( so_gpu::no_success( this_t::release_variable( item.first ) ) )
        {
            so_log::log::error("[gl_33_api::release_variable] : texture_variables") ;
            continue ;
        }
    }

    for( auto item : ptr->data_buffer_variables() )
    {
        if( so_gpu::no_success( this_t::release_variable( item.first ) ) )
        {
            so_log::log::error("[gl_33_api::release_variable] : data_buffer_variables") ;
            continue ;
        }
    }

    so_gpu::memory::dealloc( api_obj ) ;
    api_object_helper_t::set_driver_object( ptr, nullptr ) ;

    return so_gpu::ok ;
}

//*****************************************************************************************************
so_gpu::result gl_33_api::create_variable( so_gpu::variable_ptr_t ptr ) 
{
    if( api_object_helper_t::has_driver_object( ptr ) ) return so_gpu::ok ;

    drv::variable var ;
    var.data_ptr = this_t::alloc_variable_memory( ptr->get_type() ) ;
    
    if( var.data_ptr == nullptr )
    {
        so_log::log::error("[gl_33_api::create_variable] : Either no memory or undefined type.") ;
        return so_gpu::failed ;
    }

    var.do_copy = this_t::make_copy_funk( ptr->get_type() ) ;
    var.do_uniform = this_t::make_uniform_funk( ptr->get_type() ) ;
    api_object_helper_t::set_driver_object( ptr, so_gpu::memory::alloc( std::move(var) ) ) ;

    return so_gpu::ok ;
}

//*****************************************************************************************************
so_gpu::result gl_33_api::release_variable( so_gpu::variable_ptr_t ptr ) 
{
    if( api_object_helper_t::has_no_driver_object( ptr ) ) return so_gpu::ok ;
    auto * api_obj = api_object_helper_t::get_cast_api_object<drv::variable>(ptr) ;
    
    this_t::free_variable_memory( api_obj->data_ptr ) ;

    so_gpu::memory::dealloc( api_obj ) ;
    api_object_helper_t::set_driver_object( ptr, nullptr ) ;

    return so_gpu::ok ;
}

//*****************************************************************************************************
so_gpu::result gl_33_api::validate_variable( so_gpu::variable_set_ptr_t ptr ) 
{
    if( ptr == nullptr ) return so_gpu::invalid_argument ;

    if( !ptr->has_changed() ) return so_gpu::ok ;

    for( auto item : ptr->data_variables() )
    {
        if( api_object_helper_t::has_driver_object(item.first) ) continue ;

        if( so_gpu::no_success( this_t::create_variable( item.first ) ) ) 
        {
            so_log::log::error("[gl_33_api::validate_variable] : data") ;
            continue ;
        }        
    }

    for( auto item : ptr->texture_variables() )
    {
        if( api_object_helper_t::has_driver_object(item.first) ) continue ;

        if( so_gpu::no_success( this_t::create_variable( item.first ) ) ) 
        {
            so_log::log::error("[gl_33_api::validate_variable] : texture") ;
            continue ;
        }        
    }

    for( auto item : ptr->data_buffer_variables() )
    {
        if( api_object_helper_t::has_driver_object(item.first) ) continue ;

        if( so_gpu::no_success( this_t::create_variable( item.first ) ) ) 
        {
            so_log::log::error("[gl_33_api::validate_variable] : data_buffer") ;
            continue ;
        }        
    }

    ptr->set_changed( false ) ;

    return so_gpu::ok ;
}

//*****************************************************************************************************
so_gpu::result gl_33_api::load_variable( so_gpu::variable_set_ptr_t ptr ) 
{
    if( ptr == nullptr ) return so_gpu::invalid_argument ;

    this_t::validate_variable( ptr ) ;

    for( auto item : ptr->data_variables() )
    {
        if( api_object_helper_t::has_no_driver_object(item.first) )
        {
            so_log::log::error("[gl_33_api::load_variable] : no api object") ;
            continue ;
        }
        auto *api_obj = api_object_helper_t::get_cast_api_object<drv::variable>(item.first) ;
        api_obj->do_copy( api_obj->data_ptr, item.first ) ;
    }

    return so_gpu::ok ;
}

//*****************************************************************************************************
template< typename type_t >
so_gpu::so_gl::variable::copy_funk_t create_data_variable_funk( void_t )
{
    return [=]( void_ptr_t dst, variable_ptr_t src)
        { *reinterpret_cast<type_t*>(dst) = reinterpret_cast<data_variable<type_t>*>(src)->get(); } ;
}

//*****************************************************************************************************
so_gpu::so_gl::variable::copy_funk_t gl_33_api::make_copy_funk( shader_variable_type t ) 
{
    so_gpu::so_gl::variable::copy_funk_t empty_funk = 
        [=]( void_ptr_t /*dst*/, variable_ptr_t /*src*/) {} ;

    if( t == shader_variable_type::undefined ) return empty_funk ;

    switch( t )
    {
    case shader_variable_type::vec1f: return create_data_variable_funk<float_t>() ;
    case shader_variable_type::vec2f: return create_data_variable_funk<so_math::vec2f_t>() ;
    case shader_variable_type::vec3f: return create_data_variable_funk<so_math::vec3f_t>() ;
    case shader_variable_type::vec4f: return create_data_variable_funk<so_math::vec4f_t>() ;

    case shader_variable_type::vec1i: return create_data_variable_funk<int_t>() ;
    case shader_variable_type::vec2i: return create_data_variable_funk<so_math::vec2i_t>() ;
    case shader_variable_type::vec3i: return create_data_variable_funk<so_math::vec3i_t>() ;
    case shader_variable_type::vec4i: return create_data_variable_funk<so_math::vec4i_t>() ;

    case shader_variable_type::vec1ui: return create_data_variable_funk<uint_t>() ;
    case shader_variable_type::vec2ui: return create_data_variable_funk<so_math::vec2ui_t>() ;
    case shader_variable_type::vec3ui: return create_data_variable_funk<so_math::vec3ui_t>() ;
    case shader_variable_type::vec4ui: return create_data_variable_funk<so_math::vec4ui_t>() ;
    
    case shader_variable_type::mat2f: return create_data_variable_funk<so_math::mat2f_t>() ;
    case shader_variable_type::mat3f: return create_data_variable_funk<so_math::mat3f_t>() ;
    case shader_variable_type::mat4f: return create_data_variable_funk<so_math::mat4f_t>() ;

    case shader_variable_type::sampler_1d: return create_data_variable_funk<uint_t>() ;
    case shader_variable_type::sampler_2d: return create_data_variable_funk<uint_t>() ;
    case shader_variable_type::sampler_3d: return create_data_variable_funk<uint_t>() ;
    case shader_variable_type::sampler_1i: return create_data_variable_funk<uint_t>() ;
    case shader_variable_type::sampler_2i: return create_data_variable_funk<uint_t>() ;
    case shader_variable_type::sampler_3i: return create_data_variable_funk<uint_t>() ;
    case shader_variable_type::sampler_1ui: return create_data_variable_funk<uint_t>() ;
    case shader_variable_type::sampler_2ui: return create_data_variable_funk<uint_t>() ;
    case shader_variable_type::sampler_3ui: return create_data_variable_funk<uint_t>() ;
    case shader_variable_type::sampler_cube: return create_data_variable_funk<uint_t>() ;
    case shader_variable_type::sampler_1d_shadow: return create_data_variable_funk<uint_t>() ;
    case shader_variable_type::sampler_2d_shadow: return create_data_variable_funk<uint_t>() ;
    case shader_variable_type::sampler_buffer: return create_data_variable_funk<uint_t>() ;
    case shader_variable_type::sampler_buffer_i: return create_data_variable_funk<uint_t>() ;
    case shader_variable_type::sampler_buffer_ui: return create_data_variable_funk<uint_t>() ;
    case shader_variable_type::sampler_2d_array: return create_data_variable_funk<uint_t>() ;
    case shader_variable_type::sampler_2d_array_i: return create_data_variable_funk<uint_t>() ;
    case shader_variable_type::sampler_2d_array_ui: return create_data_variable_funk<uint_t>() ;
    
    default: break ;
    }
    return empty_funk ;

}

//*****************************************************************************************************
so_gpu::so_gl::variable::uniform_funk_t gl_33_api::make_uniform_funk( shader_variable_type t ) 
{
    so_gpu::so_gl::variable::uniform_funk_t empty_funk = 
        [=]( GLuint, GLuint, void_ptr_t ) {} ;

    if( t == shader_variable_type::undefined ) return empty_funk ;

    switch( t )
    {
    case shader_variable_type::vec1f: return [=]( GLuint loc, GLuint /*count*/, void_ptr_t p )
    { so_gli::gl::glUniform1f(loc, *(const GLfloat*)p); } ;
    case shader_variable_type::vec2f: return [=]( GLuint loc, GLuint count, void_ptr_t p )
    { so_gli::gl::glUniform2fv(loc, count, (const GLfloat*)p); } ;
    case shader_variable_type::vec3f: return [=]( GLuint loc, GLuint count, void_ptr_t p )
    { so_gli::gl::glUniform3fv(loc, count, (const GLfloat*)p); } ;
    case shader_variable_type::vec4f: return [=]( GLuint loc, GLuint count, void_ptr_t p )
    { so_gli::gl::glUniform4fv(loc, count, (const GLfloat*)p); } ;

    case shader_variable_type::vec1i: return [=]( GLuint loc, GLuint /*count*/, void_ptr_t p )
    { so_gli::gl::glUniform1i(loc, *(const GLint*)p); } ;
    case shader_variable_type::vec2i: return [=]( GLuint loc, GLuint count, void_ptr_t p )
    { so_gli::gl::glUniform2iv(loc, count, (const GLint*)p); } ;
    case shader_variable_type::vec3i: return [=]( GLuint loc, GLuint count, void_ptr_t p )
    { so_gli::gl::glUniform3iv(loc, count, (const GLint*)p); } ;
    case shader_variable_type::vec4i: return [=]( GLuint loc, GLuint count, void_ptr_t p )
    { so_gli::gl::glUniform4iv(loc, count, (const GLint*)p); } ;

    case shader_variable_type::vec1ui: return [=]( GLuint loc, GLuint /*count*/, void_ptr_t p )
    { so_gli::gl::glUniform1ui(loc, *(const GLuint*)p); } ;
    case shader_variable_type::vec2ui: return [=]( GLuint loc, GLuint count, void_ptr_t p )
    { so_gli::gl::glUniform2uiv(loc, count, (const GLuint*)p); } ;
    case shader_variable_type::vec3ui: return [=]( GLuint loc, GLuint count, void_ptr_t p )
    { so_gli::gl::glUniform3uiv(loc, count, (const GLuint*)p); } ;
    case shader_variable_type::vec4ui: return [=]( GLuint loc, GLuint count, void_ptr_t p )
    { so_gli::gl::glUniform4uiv(loc, count, (const GLuint*)p); } ;

    
    case shader_variable_type::mat2f: return [=]( GLuint loc, GLuint count, void_ptr_t p )
    { so_gli::gl::glUniformMatrix2fv(loc, count, GL_TRUE, (const GLfloat*)p); } ;
    case shader_variable_type::mat3f: return [=]( GLuint loc, GLuint count, void_ptr_t p )
    { so_gli::gl::glUniformMatrix3fv(loc, count, GL_TRUE, (const GLfloat*)p); } ;
    case shader_variable_type::mat4f: return [=]( GLuint loc, GLuint count, void_ptr_t p )
    { so_gli::gl::glUniformMatrix4fv(loc, count, GL_TRUE, (const GLfloat*)p); } ;

    case shader_variable_type::sampler_1d: 
    case shader_variable_type::sampler_2d: 
    case shader_variable_type::sampler_3d: 
    case shader_variable_type::sampler_1i: 
    case shader_variable_type::sampler_2i: 
    case shader_variable_type::sampler_3i: 
    case shader_variable_type::sampler_1ui: 
    case shader_variable_type::sampler_2ui: 
    case shader_variable_type::sampler_3ui: 
    case shader_variable_type::sampler_cube: 
    case shader_variable_type::sampler_1d_shadow: 
    case shader_variable_type::sampler_2d_shadow: 
    case shader_variable_type::sampler_buffer: 
    case shader_variable_type::sampler_buffer_i: 
    case shader_variable_type::sampler_buffer_ui: 
    case shader_variable_type::sampler_2d_array: 
    case shader_variable_type::sampler_2d_array_i: 
    case shader_variable_type::sampler_2d_array_ui: return [=]( GLuint loc, GLuint count, void_ptr_t p )
    { so_gli::gl::glUniform1iv(loc, count, (const GLint*)p); } ;
    
    default: break ;
    }
    return empty_funk ;
}

//*****************************************************************************************************
void_ptr_t gl_33_api::alloc_variable_memory( shader_variable_type t ) 
{
    if( t == shader_variable_type::undefined ) return nullptr ;

    switch( t )
    {
    case shader_variable_type::vec1f: return so_gpu::memory::alloc(sizeof(float_t),			"[gl_33_api] : alloc_variable_memory") ;
    case shader_variable_type::vec2f: return so_gpu::memory::alloc(sizeof(so_math::vec2f_t),	"[gl_33_api] : alloc_variable_memory") ;
    case shader_variable_type::vec3f: return so_gpu::memory::alloc(sizeof(so_math::vec3f_t),	"[gl_33_api] : alloc_variable_memory") ;
    case shader_variable_type::vec4f: return so_gpu::memory::alloc(sizeof(so_math::vec4f_t),	"[gl_33_api] : alloc_variable_memory") ;
    case shader_variable_type::vec1i: return so_gpu::memory::alloc(sizeof(int_t),				"[gl_33_api] : alloc_variable_memory") ;
    case shader_variable_type::vec2i: return so_gpu::memory::alloc(sizeof(so_math::vec2i_t),	"[gl_33_api] : alloc_variable_memory") ;
    case shader_variable_type::vec3i: return so_gpu::memory::alloc(sizeof(so_math::vec3i_t),	"[gl_33_api] : alloc_variable_memory") ;
    case shader_variable_type::vec4i: return so_gpu::memory::alloc(sizeof(so_math::vec4i_t),	"[gl_33_api] : alloc_variable_memory") ;
    case shader_variable_type::vec1ui: return so_gpu::memory::alloc(sizeof(uint_t),			"[gl_33_api] : alloc_variable_memory") ;
    case shader_variable_type::vec2ui: return so_gpu::memory::alloc(sizeof(so_math::vec2ui_t), "[gl_33_api] : alloc_variable_memory") ;
    case shader_variable_type::vec3ui: return so_gpu::memory::alloc(sizeof(so_math::vec3ui_t), "[gl_33_api] : alloc_variable_memory") ;
    case shader_variable_type::vec4ui: return so_gpu::memory::alloc(sizeof(so_math::vec4ui_t), "[gl_33_api] : alloc_variable_memory") ;
    case shader_variable_type::vec1b: return so_gpu::memory::alloc(sizeof(bool_t),				"[gl_33_api] : alloc_variable_memory") ;
    case shader_variable_type::vec2b: return so_gpu::memory::alloc(sizeof(so_math::vec2b_t),	"[gl_33_api] : alloc_variable_memory") ;
    case shader_variable_type::vec3b: return so_gpu::memory::alloc(sizeof(so_math::vec3b_t),	"[gl_33_api] : alloc_variable_memory") ;
    case shader_variable_type::vec4b: return so_gpu::memory::alloc(sizeof(so_math::vec4b_t),	"[gl_33_api] : alloc_variable_memory") ;
    case shader_variable_type::mat2f: return so_gpu::memory::alloc(sizeof(so_math::mat2f_t),	"[gl_33_api] : alloc_variable_memory") ;
    case shader_variable_type::mat3f: return so_gpu::memory::alloc(sizeof(so_math::mat3f_t),	"[gl_33_api] : alloc_variable_memory") ;
    case shader_variable_type::mat4f: return so_gpu::memory::alloc(sizeof(so_math::mat4f_t),	"[gl_33_api] : alloc_variable_memory") ;
    case shader_variable_type::sampler_1d: return so_gpu::memory::alloc(sizeof(uint_t),		"[gl_33_api] : alloc_variable_memory") ;
    case shader_variable_type::sampler_2d: return so_gpu::memory::alloc(sizeof(uint_t),		"[gl_33_api] : alloc_variable_memory") ;
    case shader_variable_type::sampler_3d: return so_gpu::memory::alloc(sizeof(uint_t),		"[gl_33_api] : alloc_variable_memory") ;
    case shader_variable_type::sampler_1i: return so_gpu::memory::alloc(sizeof(uint_t),		"[gl_33_api] : alloc_variable_memory") ;
    case shader_variable_type::sampler_2i: return so_gpu::memory::alloc(sizeof(uint_t),		"[gl_33_api] : alloc_variable_memory") ;
    case shader_variable_type::sampler_3i: return so_gpu::memory::alloc(sizeof(uint_t),		"[gl_33_api] : alloc_variable_memory") ;
    case shader_variable_type::sampler_1ui: return so_gpu::memory::alloc(sizeof(uint_t),		"[gl_33_api] : alloc_variable_memory") ;
    case shader_variable_type::sampler_2ui: return so_gpu::memory::alloc(sizeof(uint_t),		"[gl_33_api] : alloc_variable_memory") ;
    case shader_variable_type::sampler_3ui: return so_gpu::memory::alloc(sizeof(uint_t),		"[gl_33_api] : alloc_variable_memory") ;
    case shader_variable_type::sampler_cube: return so_gpu::memory::alloc(sizeof(uint_t),		"[gl_33_api] : alloc_variable_memory") ;
    case shader_variable_type::sampler_1d_shadow: return so_gpu::memory::alloc(sizeof(uint_t), "[gl_33_api] : alloc_variable_memory") ;
    case shader_variable_type::sampler_2d_shadow: return so_gpu::memory::alloc(sizeof(uint_t), "[gl_33_api] : alloc_variable_memory") ;
    case shader_variable_type::sampler_buffer: return so_gpu::memory::alloc(sizeof(uint_t),	"[gl_33_api] : alloc_variable_memory") ;
    case shader_variable_type::sampler_buffer_i: return so_gpu::memory::alloc(sizeof(uint_t),	"[gl_33_api] : alloc_variable_memory") ;
    case shader_variable_type::sampler_buffer_ui: return so_gpu::memory::alloc(sizeof(uint_t), "[gl_33_api] : alloc_variable_memory") ;
    case shader_variable_type::sampler_2d_array: return so_gpu::memory::alloc(sizeof(uint_t),	"[gl_33_api] : alloc_variable_memory") ;
    case shader_variable_type::sampler_2d_array_i: return so_gpu::memory::alloc(sizeof(uint_t), "[gl_33_api] : alloc_variable_memory") ;
    case shader_variable_type::sampler_2d_array_ui: return so_gpu::memory::alloc(sizeof(uint_t), "[gl_33_api] : alloc_variable_memory") ;
    default: break ;
    }
    return nullptr ;
}

//*****************************************************************************************************
void_t gl_33_api::free_variable_memory( void_ptr_t ptr ) 
{
    so_gpu::memory::dealloc( ptr ) ;
}

