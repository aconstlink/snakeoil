//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "gl_33_api.h"

#include "../gl_any/log.h" // gl

#include "../gl_any/objects/vertex_shader.h"
#include "../gl_any/objects/geometry_shader.h"
#include "../gl_any/objects/pixel_shader.h"
#include "../gl_any/objects/program.h"

#include "../../../shader/vertex_shader.h"
#include "../../../shader/geometry_shader.h"
#include "../../../shader/pixel_shader.h"

#include <snakeoil/std/string/split.hpp>
#include <snakeoil/std/string/string.hpp>
#include <snakeoil/std/container/vector.hpp>

#include <snakeoil/memory/guards/malloc_guard.hpp>
#include <snakeoil/log/global.h>

using namespace so_gpu ;
using namespace so_gpu::so_gl ;

namespace drv = so_gpu::so_gl ;

using gl_log = so_gpu::so_gl::log ;

//*****************************************************************************************************
so_gpu::result gl_33_api::compile( GLuint shader_id, so_gpu::shader_ptr_t shd_ptr ) 
{
    if( so_log::global::warning( shd_ptr->has_no_code(), "[gl_33_api::compile] : shader has no code." ) ) 
        return so_gpu::invalid_argument ;

    // compile
    {
        GLchar const * source_string ;
        source_string = (GLchar const *)(shd_ptr->get_code().c_str()) ;

        so_gli::gl::glShaderSource( shader_id, 1, &source_string, 0 ) ;
        if( gl_log::error("[gl_33_api::compile] : glShaderSource failed.") ) 
            return so_gpu::failed_gl_api ;
    
        so_gli::gl::glCompileShader( shader_id ) ;
        if( gl_log::error("[gl_33_api::compile] : glCompileShader failed.") ) 
            return so_gpu::failed_gl_api ;
    }

    GLint ret ;
    so_gli::gl::glGetShaderiv( shader_id, GL_COMPILE_STATUS, &ret ) ;

    GLint length ;
    so_gli::gl::glGetShaderiv( shader_id, GL_INFO_LOG_LENGTH, &length ) ;

    if( ret == GL_TRUE && length <= 1 ) return so_gpu::ok ;

    if( so_log::global::error( length == 0, "shader compilation failed, but info log length is 0.") ) 
            return so_gpu::failed_gl_api ;

    // print first line for info
    // user can place the shader name or any info there.
    {
        size_t pos = shd_ptr->get_code().find_first_of('\n') ;
        so_log::global::error( "First Line: " + shd_ptr->get_code().substr(0, pos) ) ;
    }
    
    // get the error message it is and print it
    {
        so_memory::malloc_guard<char> info_log(length) ;

        so_gli::gl::glGetShaderInfoLog( shader_id, length, 0, info_log ) ;

        so_std::vector<so_std::string_t> tokens ;
        so_std::string_ops::split( so_std::string_t(info_log), '\n', tokens ) ;

        for( auto const & msg : tokens )
        {
            so_log::global::error( msg ) ;
        }
    }

    return so_gpu::failed_shader_compile ;    
}

//*****************************************************************************************************
so_gpu::result gl_33_api::compile( so_gpu::vertex_shader_ptr_t ptr ) 
{
    if( so_gpu::no_success( this_t::create_shader( ptr ) ) ) 
        return so_gpu::result::failed_shader_compile ;

    auto * api_obj = api_object_helper_t::get_cast_api_object<drv::vertex_shader>(ptr) ;

    return this_t::compile( api_obj->id, ptr ) ;
}

//*****************************************************************************************************
so_gpu::result gl_33_api::compile( so_gpu::geometry_shader_ptr_t ptr ) 
{
    if( so_gpu::no_success( this_t::create_shader( ptr ) ) )
        return so_gpu::result::failed_shader_compile ;

    auto * api_obj = api_object_helper_t::get_cast_api_object<drv::geometry_shader>(ptr) ;

    return this_t::compile( api_obj->id, ptr ) ;
}

//*****************************************************************************************************
so_gpu::result gl_33_api::compile( so_gpu::pixel_shader_ptr_t ptr ) 
{
    if( so_gpu::no_success( this_t::create_shader( ptr ) ) )
        return so_gpu::result::failed_shader_compile ;

    auto * api_obj = api_object_helper_t::get_cast_api_object<drv::pixel_shader>(ptr) ;

    return this_t::compile( api_obj->id, ptr ) ;
}

