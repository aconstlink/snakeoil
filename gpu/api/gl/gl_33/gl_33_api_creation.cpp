//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "gl_33_api.h"

#include "../../../object.h"
#include "../../api_object.h"

// api objects 

#include "../gl_any/objects/feedback_buffer.h"
#include "../gl_any/objects/vertex_shader.h"
#include "../gl_any/objects/geometry_shader.h"
#include "../gl_any/objects/pixel_shader.h"
#include "../gl_any/objects/program.h"
#include "../gl_any/objects/vertex_buffer.h"
#include "../gl_any/objects/index_buffer.h"
#include "../gl_any/objects/data_buffer.h"
#include "../gl_any/objects/config.h"
#include "../gl_any/objects/image.h"
#include "../gl_any/objects/sampler.h"
#include "../gl_any/objects/sync.h"

// engine objects 

#include "../../../program/program.h"
#include "../../../program/config.h"
#include "../../../shader/vertex_shader.h"
#include "../../../shader/pixel_shader.h"

#include "../../../buffer/ivertex_buffer.h"
#include "../../../buffer/iindex_buffer.h"
#include "../../../buffer/idata_buffer.h"
#include "../../../buffer/feedback_buffer.h"

#include "../../../image/iimage_2d.h"
#include "../../../texture/texture_2d.h"
#include "../../../texture/depth_texture_2d.h"
#include "../../../framebuffer/framebuffer_2d.h"

#include "../../../sync/sync.h"

#include "../gl_any/log.h"

#include <snakeoil/memory/memory.h>

using namespace so_gpu ;
using namespace so_gpu::so_gl ;

namespace drv = so_gpu::so_gl ;
using gl_log = so_gpu::so_gl::log ;


//**********************************************************************************************
so_gpu::result gl_33_api::create_shader( so_gpu::vertex_shader_ptr_t ptr )
{
    if( api_object_helper_t::has_driver_object( ptr ) ) return so_gpu::ok ;
    
    GLint id = so_gli::gl::glCreateShader( GL_VERTEX_SHADER ) ;
    if( gl_log::error("[gl_33_api::create_shader] : vertex") ) 
        return so_gpu::failed_gl_api ;
        
    api_object_helper_t::set_driver_object( ptr, so_gpu::memory::alloc( drv::vertex_shader(id),
        "[gl_33_api] : create_shader - vertex" ) ) ;
        
    return so_gpu::ok ;
}

//**********************************************************************************************
so_gpu::result gl_33_api::release_shader( so_gpu::vertex_shader_ptr_t ptr )
{
    if( api_object_helper_t::has_no_driver_object( ptr ) ) return so_gpu::ok ;

    auto * api_obj = api_object_helper_t::get_cast_api_object<drv::vertex_shader>(ptr) ;

    so_gli::gl::glDeleteShader(api_obj->id) ;
    gl_log::error("[gl_33_api::release_shader] : glDeleteShader") ;

    so_gpu::memory::dealloc( api_obj ) ;
    api_object_helper_t::set_driver_object( ptr, nullptr ) ;

    return so_gpu::ok ;
}

//**********************************************************************************************
so_gpu::result gl_33_api::create_shader( so_gpu::geometry_shader_ptr_t ptr )
{
    if( api_object_helper_t::has_driver_object( ptr ) ) return so_gpu::ok ;
    
    GLint id = so_gli::gl::glCreateShader( GL_GEOMETRY_SHADER ) ;
    if( gl_log::error("[gl_33_api::create_shader] : geometry") ) 
        return so_gpu::failed_gl_api ;
        
    api_object_helper_t::set_driver_object( ptr, so_gpu::memory::alloc( drv::geometry_shader(id), 
        "[gl_33_api] : create_shader - geometry" ) ) ;
        
    return so_gpu::ok ;
}

//**********************************************************************************************
so_gpu::result gl_33_api::release_shader( so_gpu::geometry_shader_ptr_t ptr )
{
    if( api_object_helper_t::has_no_driver_object( ptr ) ) return so_gpu::ok ;

    auto * api_obj = api_object_helper_t::get_cast_api_object<drv::geometry_shader>(ptr) ;

    so_gli::gl::glDeleteShader(api_obj->id) ;
    gl_log::error("[gl_33_api::release_shader] : glDeleteShader") ;

    so_gpu::memory::dealloc( api_obj ) ;
    api_object_helper_t::set_driver_object( ptr, nullptr ) ;

    return so_gpu::ok ;
}

//**********************************************************************************************
so_gpu::result gl_33_api::create_shader( so_gpu::pixel_shader_ptr_t ptr ) 
{
    if( api_object_helper_t::has_driver_object( ptr ) ) return so_gpu::ok ;
    
    GLint id = so_gli::gl::glCreateShader( GL_FRAGMENT_SHADER ) ;
    if( gl_log::error("[gl_33_api::create_shader] : pixel") ) return so_gpu::failed_gl_api ;

    api_object_helper_t::set_driver_object( ptr, so_gpu::memory::alloc( drv::pixel_shader(id), 
        "[gl_33_api] : create_shader - pixel" ) ) ;

    return so_gpu::ok ;
}

//**********************************************************************************************
so_gpu::result gl_33_api::release_shader( so_gpu::pixel_shader_ptr_t ptr )
{
    if( api_object_helper_t::has_no_driver_object( ptr ) ) return so_gpu::ok ;

    auto * api_obj = api_object_helper_t::get_cast_api_object<drv::pixel_shader>(ptr) ;

    so_gli::gl::glDeleteShader(api_obj->id) ;
    gl_log::error("[gl_33_api::release_shader] : pixel") ;

    so_gpu::memory::dealloc( api_obj ) ;
    api_object_helper_t::set_driver_object( ptr, nullptr ) ;

    return so_gpu::ok ;
}

//**********************************************************************************************
so_gpu::result gl_33_api::create_program( so_gpu::program_ptr_t ptr )
{
    if( api_object_helper_t::has_driver_object( ptr ) ) return so_gpu::ok ;

    GLuint id = so_gli::gl::glCreateProgram() ;
    if( gl_log::error("[gl_33_api::create_program] : api") ) 
        return so_gpu::failed_gl_api ;

    api_object_helper_t::set_driver_object( ptr, so_gpu::memory::alloc( drv::program(id), 
        "[gl_33_api] : create_program" ) ) ;

    return so_gpu::ok ;
}

//**********************************************************************************************
so_gpu::result gl_33_api::release_program( so_gpu::program_ptr_t  ptr )
{
    if( api_object_helper_t::has_no_driver_object( ptr ) ) return so_gpu::ok ;

    // it is ok to delete all shader variables here, because
    // shader variables only exist if a gl program exists.
    // this is accomplished through program link operation.
    // @see link_program
    this_t::delete_all_shader_variables( ptr ) ;

    auto * api_obj = api_object_helper_t::get_cast_api_object<drv::program>(ptr) ;

    if( api_obj->id == _last_po ) _last_po = GLuint(-1) ;

    so_gli::gl::glDeleteProgram(api_obj->id) ;
    gl_log::error("[gl_33_api::release_program] : api") ;

    so_gpu::memory::dealloc( api_obj ) ;
    api_object_helper_t::set_driver_object( ptr, nullptr ) ;

    return so_gpu::ok ;
}

//**********************************************************************************************
so_gpu::result gl_33_api::create_buffer( so_gpu::ivertex_buffer_ptr_t ptr )
{
    if( api_object_helper_t::has_driver_object( ptr ) ) return so_gpu::ok ;

    GLuint id = GLuint(-1) ;
    so_gli::gl::glGenBuffers( 1, &id ) ;
    if( gl_log::error("[gl_33_api::create_vertex_buffer] : api") ) 
        return so_gpu::failed_gl_api ;

    api_object_helper_t::set_driver_object( ptr, so_gpu::memory::alloc( drv::vertex_buffer(id), 
        "[gl_33_api] : create_buffer - vertex" ) ) ;

    return so_gpu::ok ;
}

//**********************************************************************************************
so_gpu::result gl_33_api::release_buffer( so_gpu::ivertex_buffer_ptr_t ptr )
{
    if( api_object_helper_t::has_no_driver_object( ptr ) ) return so_gpu::ok ;

    auto * api_obj = api_object_helper_t::get_cast_api_object<drv::vertex_buffer>(ptr) ;

    {
        GLuint id = api_obj->id ;
        so_gli::gl::glDeleteBuffers( 1, &id ) ;
        gl_log::error("[gl_33_api::release_vertex_buffer] : api") ;
    }

    so_gpu::memory::dealloc( api_obj ) ;
    api_object_helper_t::set_driver_object( ptr, nullptr ) ;

    return so_gpu::ok ;
}

//**********************************************************************************************
so_gpu::result gl_33_api::create_buffer( so_gpu::iindex_buffer_ptr_t ptr ) 
{
    if( api_object_helper_t::has_driver_object( ptr ) ) return so_gpu::ok ;

    GLuint id = GLuint(-1) ;
    so_gli::gl::glGenBuffers( 1, &id ) ;
    if( gl_log::error("[gl_33_api::create_index_buffer] : api") ) 
        return so_gpu::failed_gl_api ;

    api_object_helper_t::set_driver_object( ptr, so_gpu::memory::alloc( drv::index_buffer(id), 
        "[gl_33_api] : create_buffer - index" ) ) ;

    return so_gpu::ok ;
}

//**********************************************************************************************
so_gpu::result gl_33_api::release_buffer( so_gpu::iindex_buffer_ptr_t ptr )
{
    if( api_object_helper_t::has_no_driver_object( ptr ) ) return so_gpu::ok ;

    auto * api_obj = api_object_helper_t::get_cast_api_object<drv::index_buffer>(ptr) ;

    {
        GLuint id = api_obj->id ;
        so_gli::gl::glDeleteBuffers( 1, &id ) ;
        gl_log::error("[gl_33_api::release_index_buffer] : api") ;
    }

    so_gpu::memory::dealloc( api_obj ) ;
    api_object_helper_t::set_driver_object( ptr, nullptr ) ;

    return so_gpu::ok ;
}

//**********************************************************************************************
so_gpu::result gl_33_api::create_buffer( so_gpu::idata_buffer_ptr_t ptr ) 
{
    if( api_object_helper_t::has_driver_object( ptr ) ) return so_gpu::ok ;

    GLuint buffer_id = GLuint(-1) ;
    GLuint texture_id = GLuint(-1) ;
    {
        so_gli::gl::glGenBuffers( 1, &buffer_id ) ;
        if( gl_log::error("[gl_33_api::create_buffer] : glGenBuffers") ) 
            return so_gpu::failed_gl_api ;
    }
    {
        so_gli::gl::glGenTextures( 1, &texture_id ) ;
        if( gl_log::error("[gl_33_api::create_buffer] : glGenTextures") ) 
            return so_gpu::failed_gl_api ;
    }

    api_object_helper_t::set_driver_object( ptr, so_gpu::memory::alloc( drv::data_buffer(buffer_id, texture_id), 
        "[gl_33_api] : create_buffer - data" ) ) ;

    return so_gpu::ok ;
}

//**********************************************************************************************
so_gpu::result gl_33_api::release_buffer( so_gpu::idata_buffer_ptr_t  ptr )
{
    if( api_object_helper_t::has_no_driver_object( ptr ) ) return so_gpu::ok ;

    auto * api_obj = api_object_helper_t::get_cast_api_object<drv::data_buffer>(ptr) ;

    so_gli::gl::glDeleteBuffers( 1, &api_obj->bid ) ;
    gl_log::error("[gl_33_api::release_buffer] : glDeleteBuffers") ;

    so_gli::gl::glDeleteTextures( 1, &api_obj->tid ) ;
    gl_log::error("[gl_33_api::release_buffer] : glDeleteTextures") ;

    so_gpu::memory::dealloc( api_obj ) ;
    api_object_helper_t::set_driver_object( ptr, nullptr ) ;

    return so_gpu::ok ;
}

//**********************************************************************************************
so_gpu::result gl_33_api::create_buffer( so_gpu::feedback_buffer_ptr_t ptr ) 
{
    if( api_object_helper_t::has_driver_object( ptr ) ) return so_gpu::ok ;

    GLuint buffer_id = GLuint(-1) ;
    GLuint query_id = GLuint(-1) ;

    {
        so_gli::gl::glGenTransformFeedbacks( 1, &buffer_id ) ;
        if( gl_log::error("[gl_33_api::create_buffer] : glGenTransformFeedbacks") ) 
            return so_gpu::failed_gl_api ;
    }
    {
        so_gli::gl::glGenQueries( 1, &query_id ) ;
        if( gl_log::error("[gl_33_api::create_buffer] : glGenQueries") ) 
            return so_gpu::failed_gl_api ;
    }

    // in order to omit a GL error in glGetQueryObjectuiv in gl_33_api::query_primitives_written
    // the query object is initialized here. So the query object will return 0 the first time
    // it is queried if no prior transform feedback has been performed.
    {        
        so_gli::gl::glBeginQuery( GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, query_id ) ;
        gl_log::error( "[gl_33_api::create_buffer] : glBeginQuery - GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN" ) ;
        
        so_gli::gl::glEndQuery( GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN ) ;
        gl_log::error( "[gl_33_api::use_feedback_buffer] : glEndQuery - GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN" ) ;
    }

    api_object_helper_t::set_driver_object( ptr, so_gpu::memory::alloc( drv::feedback_buffer(buffer_id, query_id),
        "[gl_33_api] : create_buffer - feedback" ) ) ;

    return so_gpu::ok ;
}

//**********************************************************************************************
so_gpu::result gl_33_api::release_buffer( so_gpu::feedback_buffer_ptr_t  ptr ) 
{
    if( api_object_helper_t::has_no_driver_object( ptr ) ) return so_gpu::ok ;

    auto * api_obj = api_object_helper_t::get_cast_api_object<drv::feedback_buffer>(ptr) ;

    if( _last_feedback_buffer == api_obj && _last_feedback_buffer->id == api_obj->id ) 
        _last_feedback_buffer = nullptr ;

    so_gli::gl::glDeleteTransformFeedbacks( 1, &api_obj->id ) ;
    gl_log::error("[gl_33_api::release_buffer] : glDeleteTransformFeedbacks") ;

    so_gli::gl::glDeleteQueries( 1, &api_obj->qid ) ;
    gl_log::error("[gl_33_api::release_buffer] : glDeleteQueries") ;

    so_gpu::memory::dealloc( api_obj ) ;
    api_object_helper_t::set_driver_object( ptr, nullptr ) ;

    return so_gpu::ok ;
}

//**********************************************************************************************
so_gpu::result gl_33_api::create_config( so_gpu::config_ptr_t ptr )
{
    if( api_object_helper_t::has_driver_object( ptr ) ) return so_gpu::ok ;

    GLuint id = GLuint(-1) ;
    so_gli::gl::glGenVertexArrays(1, &id) ;
    if( gl_log::error("[gl_33_api::create_geometry] : api") ) return so_gpu::failed_gl_api ;

    api_object_helper_t::set_driver_object( ptr, so_gpu::memory::alloc( drv::config(id),
        "[gl_33_api] : create_config" ) ) ;

    return so_gpu::ok ;
}

//**********************************************************************************************
so_gpu::result gl_33_api::release_config( so_gpu::config_ptr_t ptr )
{
    if( api_object_helper_t::has_no_driver_object( ptr ) ) return so_gpu::ok ;

    auto * api_obj = api_object_helper_t::get_cast_api_object<drv::config>(ptr) ;

    if( api_obj->id == _last_vao ) _last_vao = GLuint(-1) ;

    {
        GLuint id = api_obj->id ;
        so_gli::gl::glDeleteVertexArrays(1, &id) ;
        gl_log::error("[gl_33_api::release_geometry] : api") ;
    }

    so_gpu::memory::dealloc( api_obj ) ;
    api_object_helper_t::set_driver_object( ptr, nullptr ) ;

    return so_gpu::ok ;
}

//**********************************************************************************************
so_gpu::result gl_33_api::create_image( so_gpu::image_ptr_t ptr ) 
{
    if( api_object_helper_t::has_driver_object( ptr ) ) return so_gpu::ok ;

    GLuint id = GLuint(-1) ;
    so_gli::gl::glGenTextures(1, &id) ;
    if( gl_log::error("[gl_33_api::create_image] : glGenTextures") ) return so_gpu::failed_gl_api ;

    api_object_helper_t::set_driver_object( ptr, so_gpu::memory::alloc( drv::image(id),
        "[gl_33_api] : create_image" ) ) ;
    
    return so_gpu::ok ;
}

//**********************************************************************************************
so_gpu::result gl_33_api::release_image( so_gpu::image_ptr_t ptr ) 
{
    if( api_object_helper_t::has_no_driver_object( ptr ) ) return so_gpu::ok ;

    auto * api_obj = api_object_helper_t::get_cast_api_object<drv::image>(ptr) ;

    //if( api_obj->id == _last_image ) _last_image = GLuint(-1) ;

    {
        GLuint id = api_obj->id ;
        so_gli::gl::glDeleteTextures(1, &id) ;
        gl_log::error("[gl_33_api::release_image] : glDeleteTextures") ;
    }

    so_gpu::memory::dealloc( api_obj ) ;
    api_object_helper_t::set_driver_object( ptr, nullptr ) ;

    return so_gpu::ok ;
}

//**********************************************************************************************
so_gpu::result gl_33_api::create_texture( so_gpu::texture_ptr_t ptr )
{
    if( api_object_helper_t::has_driver_object( ptr ) ) return so_gpu::ok ;

    GLuint id = GLuint(-1) ;
    so_gli::gl::glGenSamplers(1, &id) ;
    if( gl_log::error("[gl_33_api::create_texture] : glGenSamplers") ) return so_gpu::failed_gl_api ;

    api_object_helper_t::set_driver_object( ptr, so_gpu::memory::alloc( drv::sampler(id),
        "[gl_33_api] : create_texture" ) ) ;
    
    return so_gpu::ok ;
}

//**********************************************************************************************
so_gpu::result gl_33_api::release_texture( so_gpu::texture_ptr_t ptr )
{
    if( api_object_helper_t::has_no_driver_object( ptr ) ) return so_gpu::ok ;

    auto * api_obj = api_object_helper_t::get_cast_api_object<drv::sampler>(ptr) ;

    {
        GLuint id = api_obj->id ;
        so_gli::gl::glDeleteSamplers(1, &id) ;
        gl_log::error("[gl_33_api::release_texture] : glDeleteSamplers") ;
    }

    so_gpu::memory::dealloc( api_obj ) ;
    api_object_helper_t::set_driver_object( ptr, nullptr ) ;

    return so_gpu::ok ;
}

//**********************************************************************************************
so_gpu::result gl_33_api::create_sync( so_gpu::sync_ptr_t ptr )  
{
    if( api_object_helper_t::has_driver_object( ptr ) ) return so_gpu::ok ;

    // which objects?
    // @todo which object

    api_object_helper_t::set_driver_object( ptr, so_gpu::memory::alloc( drv::sync(),
        "[gl_33_api] : create_sync" ) ) ;
    
    return so_gpu::ok ;
}

//**********************************************************************************************
so_gpu::result gl_33_api::release_sync( so_gpu::sync_ptr_t ptr )  
{
    if( api_object_helper_t::has_no_driver_object( ptr ) ) return so_gpu::ok ;

    auto * api_obj = api_object_helper_t::get_cast_api_object<drv::sync>(ptr) ;

   // @todo delete sync objects

    so_gpu::memory::dealloc( api_obj ) ;
    api_object_helper_t::set_driver_object( ptr, nullptr ) ;

    return so_gpu::ok ;
}

