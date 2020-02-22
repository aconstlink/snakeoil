//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "gl_33_api.h"

#include "../gl_any/converter/gl_convert_image_format.h"
#include "../gl_any/converter/gl_convert_data_buffer_format.h"

#include "../gl_any/objects/data_buffer.h"
#include "../gl_any/objects/vertex_buffer.h"
#include "../gl_any/objects/index_buffer.h"
#include "../gl_any/objects/image.h"

#include "../../../buffer/idata_buffer.h"
#include "../../../buffer/ivertex_buffer.h"
#include "../../../buffer/iindex_buffer.h"

#include "../../../texture/texture_2d.h"
#include "../../../texture/depth_texture_2d.h"

#include "../gl_any/log.h" // gl

using namespace so_gpu ;
using namespace so_gpu::so_gl ;

namespace drv = so_gpu::so_gl ;

using gl_log = so_gpu::so_gl::log ;

//*****************************************************************************************************
so_gpu::result gl_33_api::alloc_image_memory( so_gpu::iimage_2d_ptr_t ptr, image_alloc_info_cref_t info )
{
    if( api_object_helper_t::has_no_driver_object(ptr) ) 
    {
        so_log::global::warning("[gl_33_api::alloc_image_memory] : image2d - no valid driver object") ;
        return so_gpu::invalid_argument ;
    }

    auto * api_obj = api_object_helper_t::get_cast_api_object<drv::image>(ptr) ;

    this_t::use_image( api_obj->id, GL_TEXTURE_2D ) ;

    void_cptr_t buffer_ptr = info.do_copy ? ptr->get_raw_pointer() : nullptr ;

    if( ptr->get_type() == so_gpu::image_type::color_2d )
    {
        api_obj->image_target = GL_TEXTURE_2D ;
        api_obj->width = GLsizei(ptr->get_width()) ;
        api_obj->height = GLsizei(ptr->get_height()) ;

        so_gli::gl::glTexImage2D(
        GL_TEXTURE_2D,
        0,
        so_gl::convert_to_gl_format( ptr->get_format(), ptr->get_pixel_type() ),
        GLsizei(ptr->get_width()),
        GLsizei(ptr->get_height()),
        0,
        so_gl::convert_to_gl_pixel_format( ptr->get_format() ),
        so_gl::convert_to_gl_pixel_type( ptr->get_pixel_type() ),
        buffer_ptr
        ) ;
        if( gl_log::error("[gl_33_api::alloc_image_memory] : glTexImage2D - Color") ) 
            return so_gpu::failed_gl_api ;
    }
    else if( ptr->get_type() == image_type::depth )
    {
        api_obj->image_target = GL_TEXTURE_2D ;
        api_obj->width = GLsizei( ptr->get_width() ) ;
        api_obj->height = GLsizei( ptr->get_height() ) ;

        so_gli::gl::glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_DEPTH_COMPONENT,
        GLsizei(ptr->get_width()),
        GLsizei(ptr->get_height()),
        0,
        GL_DEPTH_COMPONENT,
        so_gl::convert_to_gl_pixel_type( ptr->get_pixel_type() ),
        buffer_ptr
        ) ;
        if( gl_log::error("[gl_33_api::alloc_image_memory] : glTexImage2D") ) 
            return so_gpu::failed_gl_api ;
    }
    else if( ptr->get_type() == image_type::stencil )
    {
    }
    else
    {
        so_log::global::error( "[gl_33_api::alloc_image_memory] : undefined image type" ) ;
        return so_gpu::invalid_argument ;
    }

    return so_gpu::ok ;
}

//*****************************************************************************************************
so_gpu::result gl_33_api::load_image_memory( so_gpu::iimage_2d_ptr_t ptr, image_load_info_cref_t )
{
    if( api_object_helper_t::has_no_driver_object( ptr ) )
    {
        so_log::global::warning( "[gl_33_api::load_image_memory] : image2d - no valid driver object" ) ;
        return so_gpu::invalid_argument ;
    }

    auto * api_obj = api_object_helper_t::get_cast_api_object<drv::image>( ptr ) ;

    this_t::use_image( api_obj->id, GL_TEXTURE_2D ) ;

    void_cptr_t buffer_ptr = ptr->get_raw_pointer() ;

    if( ptr->get_type() == so_gpu::image_type::color_2d )
    {
        api_obj->image_target = GL_TEXTURE_2D ;
        api_obj->width = GLsizei( ptr->get_width() ) ;
        api_obj->height = GLsizei( ptr->get_height() ) ;

        so_gli::gl::glTexSubImage2D(
            GL_TEXTURE_2D,
            0,
            0,0,
            GLsizei( ptr->get_width() ),
            GLsizei( ptr->get_height() ),
            so_gl::convert_to_gl_pixel_format( ptr->get_format() ),
            so_gl::convert_to_gl_pixel_type( ptr->get_pixel_type() ),
            buffer_ptr
        ) ;
        if( gl_log::error( "[gl_33_api::load_image_memory] : glTexImage2D - Color" ) )
            return so_gpu::failed_gl_api ;
    }
    else if( ptr->get_type() == image_type::depth )
    {
        api_obj->image_target = GL_TEXTURE_2D ;
        api_obj->width = GLsizei( ptr->get_width() ) ;
        api_obj->height = GLsizei( ptr->get_height() ) ;

        so_gli::gl::glTexSubImage2D(
            GL_TEXTURE_2D,
            0, 0, 0, 
            GLsizei( ptr->get_width() ),
            GLsizei( ptr->get_height() ),
            GL_DEPTH_COMPONENT,
            so_gl::convert_to_gl_pixel_type( ptr->get_pixel_type() ),
            buffer_ptr
        ) ;
        if( gl_log::error( "[gl_33_api::load_image_memory] : glTexImage2D" ) )
            return so_gpu::failed_gl_api ;
    }
    else if( ptr->get_type() == image_type::stencil )
    {
        so_log::global::warning("[gl_33_api::load_image_memory] : stencil not supported") ;
        return so_gpu::invalid_argument ;
    }
    else
    {
        so_log::global::error( "[gl_33_api::load_image_memory] : undefined image type" ) ;
        return so_gpu::invalid_argument ;
    }

    return so_gpu::ok ;
}

//*****************************************************************************************************
so_gpu::result gl_33_api::save_image_memory( 
    so_gpu::iimage_2d_ptr_t ptr, image_save_info_cref_t ) 
{
    if( api_object_helper_t::has_no_driver_object( ptr ) )
    {
        so_log::global::warning( "[gl_33_api::save_image_memory] : "
            "image2d - no valid driver object" ) ;
        return so_gpu::invalid_argument ;
    }

    auto * api_obj = api_object_helper_t::get_cast_api_object<drv::image>( ptr ) ;
    
    // check if image was even allocated and
    // if the image size has changed without reallocating
    if( GLsizei(ptr->get_width()) != api_obj->width ||
        GLsizei(ptr->get_height()) != api_obj->height )
    {
        so_log::global::warning( "[gl_33_api::save_image_memory] : "
            "width or height invalid" ) ;
        return so_gpu::invalid_argument ;
    }

    this_t::use_image( api_obj->id, GL_TEXTURE_2D ) ;

    void_ptr_t buffer_ptr = ptr->get_raw_pointer() ;

    if( ptr->get_type() == so_gpu::image_type::color_2d )
    {
        so_gli::gl::glGetTexImage(
            GL_TEXTURE_2D,
            0,
            so_gl::convert_to_gl_pixel_format( ptr->get_format() ),
            so_gl::convert_to_gl_pixel_type( ptr->get_pixel_type() ),
            buffer_ptr ) ;

        if( gl_log::error( "[gl_33_api::save_image_memory] : glGetTexImage - Color" ) )
            return so_gpu::failed_gl_api ;
    }
    else if( ptr->get_type() == image_type::depth )
    {
        so_gli::gl::glGetTexImage(
            GL_TEXTURE_2D,
            0,
            GL_DEPTH_COMPONENT,
            so_gl::convert_to_gl_pixel_type( ptr->get_pixel_type() ),
            buffer_ptr ) ;

        if( gl_log::error( "[gl_33_api::save_image_memory] : glGetTexImage - Depth" ) )
            return so_gpu::failed_gl_api ;
    }
    else if( ptr->get_type() == image_type::stencil )
    {
    }
    else
    {
        so_log::global::error( "[gl_33_api::alloc_image_memory] : undefined image type" ) ;
        return so_gpu::invalid_argument ;
    }

    return so_gpu::ok ;
}

//*****************************************************************************************************
so_gpu::result gl_33_api::alloc_buffer_memory( so_gpu::idata_buffer_ptr_t ptr, memory_alloc_info_cref_t info )
{
    if( so_gpu::no_success( this_t::create_buffer(ptr) ) )
    {
        so_log::global::warning("[gl_33_api::alloc_buffer_memory] : data_buffer - no valid driver object") ;
        return so_gpu::invalid_argument ;
    }

    this_t::null_vao() ;

    auto * api_obj = api_object_helper_t::get_cast_api_object<drv::data_buffer>(ptr) ;

    size_t const sib = std::min( info.sib, ptr->get_sib() ) ;
    void_cptr_t buffer = info.do_copy == true ? ptr->get_raw_pointer() : nullptr ;

    GLuint const buffer_id = api_obj->bid ;
    GLuint const texture_id = api_obj->tid ;

    {
        so_gli::gl::glBindBuffer( GL_TEXTURE_BUFFER, buffer_id ) ;
        if( gl_log::error("[gl_33_api::alloc_buffer_memory] : glBindBuffer GL_TEXTURE_BUFFER") ) 
            return so_gpu::failed_gl_api ;

        so_gli::gl::glBufferData( GL_TEXTURE_BUFFER, sib, buffer, GL_STATIC_DRAW ) ;
        if( gl_log::error("[gl_33_api::alloc_buffer_memory] : glBufferData GL_TEXTURE_BUFFER") ) 
            return so_gpu::failed_gl_api ;
    }

    // glTexBuffer is required to be called after driver memory is aquired.
    {
        so_gli::gl::glBindTexture( GL_TEXTURE_BUFFER, texture_id ) ;
        if( gl_log::error("[gl_33_api::alloc_buffer_memory] : glBindTexture GL_TEXTURE_BUFFER") ) 
            return so_gpu::failed_gl_api ;

        so_gli::gl::glTexBuffer( GL_TEXTURE_BUFFER, so_gl::convert(ptr->get_data_buffer_format()), buffer_id ) ;
        if( gl_log::error("[gl_33_api::alloc_buffer_memory] : glTexBuffer GL_TEXTURE_BUFFER") ) 
            return so_gpu::failed_gl_api ;
    }

    return so_gpu::ok ;
}

//*****************************************************************************************************
so_gpu::result gl_33_api::alloc_buffer_memory( so_gpu::ivertex_buffer_ptr_t ptr, memory_alloc_info_cref_t info )
{
    if( so_gpu::no_success( this_t::create_buffer(ptr) ) )
    {
        so_log::global::warning("[gl_33_api::alloc_buffer_memory] : vertex_buffer - no valid driver object") ;
        return so_gpu::invalid_argument ;
    }

    this_t::null_vao() ;

    auto * api_obj = api_object_helper_t::get_cast_api_object<drv::vertex_buffer>(ptr) ;

    size_t const sib = std::min( info.sib, ptr->get_sib() ) ;
    void_cptr_t buffer = info.do_copy == true ? ptr->get_raw_pointer() : nullptr ;

    so_gli::gl::glBindBuffer( GL_ARRAY_BUFFER, api_obj->id ) ;
    if( gl_log::error("[gl_33_api::alloc_vertex_buffer_memory] : glBindBuffer") ) 
        return so_gpu::failed_gl_api ;

    so_gli::gl::glBufferData( GL_ARRAY_BUFFER, sib, buffer, GL_STATIC_DRAW ) ;
    if( gl_log::error("[gl_33_api::alloc_vertex_buffer_memory] : glBufferData") ) 
        return so_gpu::failed_gl_api ;

    api_obj->sib = sib ;

    return so_gpu::ok ;
}

//*****************************************************************************************************
so_gpu::result gl_33_api::alloc_buffer_memory( so_gpu::iindex_buffer_ptr_t ptr, memory_alloc_info_cref_t info )
{
    if( so_gpu::no_success( this_t::create_buffer(ptr) ) )
    {
        so_log::global::warning("[gl_33_api::alloc_buffer_memory] : index_buffer - no valid driver object") ;
        return so_gpu::invalid_argument ;
    }

    this_t::null_vao() ;

    auto * api_obj = api_object_helper_t::get_cast_api_object<drv::index_buffer>(ptr) ;

    size_t const sib = std::min( info.sib, ptr->get_sib() ) ;
    void_cptr_t buffer = info.do_copy == true ? ptr->get_raw_pointer() : nullptr ;

    so_gli::gl::glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, api_obj->id ) ;
    if( gl_log::error("[gl_33_api::alloc_vertex_buffer_memory] : glBindBuffer") ) 
        return so_gpu::failed_gl_api ;

    so_gli::gl::glBufferData( GL_ELEMENT_ARRAY_BUFFER, sib, buffer, GL_STATIC_DRAW ) ;
    if( gl_log::error("[gl_33_api::alloc_vertex_buffer_memory] : glBufferData") ) 
        return so_gpu::failed_gl_api ;

    return so_gpu::ok ;
}

//*****************************************************************************************************
void_ptr_t gl_33_api::map_buffer_memory( so_gpu::ivertex_buffer_ptr_t ptr, memory_mapping_info_cref_t info ) 
{
    if( so_gpu::no_success( this_t::create_buffer(ptr) ) )
    {
        so_log::global::warning("[gl_33_api::map_buffer_memory] : vertex_buffer - no valid driver object") ;
        return nullptr ;
    }

    this_t::null_vao() ;

    auto * api_obj = api_object_helper_t::get_cast_api_object<drv::vertex_buffer>(ptr) ;

    size_t const sib = std::min( info.sib, ptr->get_sib() ) ;

    so_gli::gl::glBindBuffer( GL_ARRAY_BUFFER, api_obj->id ) ;
    if( gl_log::error("[gl_33_api::map_vertex_buffer_memory] : glBindBuffer") ) 
        return nullptr ;
    
    void_ptr_t ret_ptr = so_gli::gl::glMapBufferRange( GL_ARRAY_BUFFER, 0, sib, GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT ) ;
    if( gl_log::error("[gl_33_api::map_vertex_buffer_memory] : glMapBufferRange") ) 
        return nullptr ;

    return ret_ptr ;
}

//*****************************************************************************************************
so_gpu::result gl_33_api::unmap_buffer_memory( so_gpu::ivertex_buffer_ptr_t ptr )
{
    if( so_gpu::no_success( this_t::create_buffer(ptr) ) )
    {
        so_log::global::warning("[gl_33_api::unmap_buffer_memory] : vertex_buffer - no valid driver object") ;
        return so_gpu::invalid_argument ;
    }

    this_t::null_vao() ;

    auto * api_obj = api_object_helper_t::get_cast_api_object<drv::vertex_buffer>(ptr) ;

    so_gli::gl::glBindBuffer( GL_ARRAY_BUFFER, api_obj->id ) ;
    if( gl_log::error("[gl_33_api::unmap_vertex_buffer_memory] : glBindBuffer") ) 
        return so_gpu::failed_gl_api ;

    so_gli::gl::glUnmapBuffer( GL_ARRAY_BUFFER ) ;
    if( gl_log::error("[gl_33_api::unmap_vertex_buffer_memory] : glUnmapBuffer") ) 
        return so_gpu::failed_gl_api ;

    return so_gpu::ok ;
}

//*****************************************************************************************************            
void_ptr_t gl_33_api::map_buffer_memory( so_gpu::iindex_buffer_ptr_t ptr, memory_mapping_info_cref_t info ) 
{
    if( so_gpu::no_success( this_t::create_buffer(ptr) ) )
    {
        so_log::global::warning("[gl_33_api::map_buffer_memory] : index_buffer - no valid driver object") ;
        return nullptr ;
    }

    auto * api_obj = api_object_helper_t::get_cast_api_object<drv::index_buffer>(ptr) ;

    size_t const sib = std::min( info.sib, ptr->get_sib() ) ;

    so_gli::gl::glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, api_obj->id ) ;
    if( gl_log::error("[gl_33_api::map_index_buffer_memory] : glBindBuffer") ) 
        return nullptr ;
    
    void_ptr_t ret_ptr = so_gli::gl::glMapBufferRange( GL_ELEMENT_ARRAY_BUFFER, 0, sib, GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT ) ;
    if( gl_log::error("[gl_33_api::map_index_buffer_memory] : glMapBufferRange") ) 
        return nullptr ;

    return ret_ptr ;
}

//*****************************************************************************************************
so_gpu::result gl_33_api::unmap_buffer_memory( so_gpu::iindex_buffer_ptr_t ptr ) 
{
    if( so_gpu::no_success( this_t::create_buffer(ptr) ) )
    {
        so_log::global::warning("[gl_33_api::unmap_buffer_memory] : index_buffer - no valid driver object") ;
        return so_gpu::invalid_argument ;
    }

    this_t::null_vao() ;

    auto * api_obj = api_object_helper_t::get_cast_api_object<drv::index_buffer>(ptr) ;

    so_gli::gl::glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, api_obj->id ) ;
    if( gl_log::error("[gl_33_api::unmap_index_buffer_memory] : glBindBuffer") ) 
        return so_gpu::failed_gl_api ;

    so_gli::gl::glUnmapBuffer( GL_ELEMENT_ARRAY_BUFFER ) ;
    if( gl_log::error("[gl_33_api::unmap_index_buffer_memory] : glUnmapBuffer") ) 
        return so_gpu::failed_gl_api ;

    return so_gpu::ok ;
}

//*****************************************************************************************************
so_gpu::result gl_33_api::load_buffer_memory( so_gpu::ivertex_buffer_ptr_t ptr, memory_load_info_cref_t info ) 
{
    if( so_gpu::no_success( this_t::create_buffer(ptr) ) )
    {
        so_log::global::warning("[gl_33_api::load_buffer_memory] : vertex_buffer - no valid driver object") ;
        return so_gpu::invalid_argument ;
    }

    this_t::null_vao() ;

    auto * api_obj = api_object_helper_t::get_cast_api_object<drv::vertex_buffer>(ptr) ;

    size_t const off = std::min( info.offset, ptr->get_sib() ) ;
    size_t const sib = std::min( info.sib, ptr->get_sib() ) ;
    
    size_t const required_size = off + sib ;

    if( required_size > api_obj->sib )
    {
        // @todo what if the required size is bigger than the allocated size
        // do automatic alloc?
    }
    
    void_cptr_t buffer = ptr->get_raw_pointer() ;

    so_gli::gl::glBindBuffer( GL_ARRAY_BUFFER, api_obj->id ) ;
    if( gl_log::error("[gl_33_api::load_buffer_memory] : glBindBuffer GL_ARRAY_BUFFER") ) 
        return so_gpu::failed_gl_api ;

    so_gli::gl::glBufferSubData( GL_ARRAY_BUFFER, off, sib, buffer ) ;
    if( gl_log::error("[gl_33_api::load_buffer_memory] : glBufferSubData GL_ARRAY_BUFFER") ) 
        return so_gpu::failed_gl_api ;

    return so_gpu::ok ;
}

//*****************************************************************************************************
so_gpu::result gl_33_api::load_buffer_memory( so_gpu::iindex_buffer_ptr_t ptr, memory_load_info_cref_t info ) 
{
    if( so_gpu::no_success( this_t::create_buffer(ptr) ) )
    {
        so_log::global::warning("[gl_33_api::load_buffer_memory] : index_buffer - no valid driver object") ;
        return so_gpu::invalid_argument ;
    }

    this_t::null_vao() ;

    auto * api_obj = api_object_helper_t::get_cast_api_object<drv::index_buffer>(ptr) ;

    size_t const off = std::min( info.offset, ptr->get_sib() ) ;
    size_t const sib = std::min( info.sib, ptr->get_sib() ) ;
    void_cptr_t buffer = ptr->get_raw_pointer() ;

    so_gli::gl::glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, api_obj->id ) ;
    if( gl_log::error("[gl_33_api::load_buffer_memory] : glBindBuffer GL_ELEMENT_ARRAY_BUFFER") ) 
        return so_gpu::failed_gl_api ;

    so_gli::gl::glBufferSubData( GL_ELEMENT_ARRAY_BUFFER, off, sib, buffer ) ;
    if( gl_log::error("[gl_33_api::load_buffer_memory] : glBufferSubData GL_ELEMENT_ARRAY_BUFFER") ) 
        return so_gpu::failed_gl_api ;

    return so_gpu::ok ;
}

//*****************************************************************************************************
so_gpu::result gl_33_api::load_buffer_memory( so_gpu::idata_buffer_ptr_t ptr, so_gpu::memory_load_info_cref_t info ) 
{
    if( so_gpu::no_success( this_t::create_buffer(ptr) ) )
    {
        so_log::global::warning("[gl_33_api::load_buffer_memory] : data_buffer - no valid driver object") ;
        return so_gpu::invalid_argument ;
    }

    this_t::null_vao() ;

    auto * api_obj = api_object_helper_t::get_cast_api_object<drv::data_buffer>(ptr) ;

    size_t const off = std::min( info.offset, ptr->get_sib() ) ;
    size_t const sib = std::min( info.sib, ptr->get_sib() ) ;
    void_cptr_t buffer = ptr->get_raw_pointer() ;

    so_gli::gl::glBindBuffer( GL_TEXTURE_BUFFER, api_obj->bid ) ;
    if( gl_log::error("[gl_33_api::load_buffer_memory] : glBindBuffer GL_TEXTURE_BUFFER") ) 
        return so_gpu::failed_gl_api ;

    so_gli::gl::glBufferSubData( GL_TEXTURE_BUFFER, off, sib, buffer ) ;
    if( gl_log::error("[gl_33_api::load_buffer_memory] : glBufferSubData GL_TEXTURE_BUFFER") ) 
        return so_gpu::failed_gl_api ;

    return so_gpu::ok ;
}

//*****************************************************************************************************
so_gpu::result gl_33_api::check_buffer_size( bool_t check_index_buffer ) 
{
    // need to check if the vertex buffer has GPU memory.
    {
        GLint buffer_size = 0 ;
        so_gli::gl::glGetBufferParameteriv( GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &buffer_size ) ;
        if(gl_log::error( "[gl_33_api::execute] : glGetBufferParameteriv" ) ||
            buffer_size == 0)
        {
            so_log::global::error( "[so_gpu::gl_33_api::use_vao] : no vertex buffer memory" ) ;
            return so_gpu::invalid ;
        }
    }

    if( check_index_buffer )
    {
        GLint buffer_size = 0 ;
        so_gli::gl::glGetBufferParameteriv( GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &buffer_size ) ;
        if(gl_log::error( "[gl_33_api::execute] : glGetBufferParameteriv" ) ||
            buffer_size == 0)
        {
            so_log::global::error( "[so_gpu::gl_33_api::use_vao] : no index buffer memory" ) ;
            return so_gpu::invalid ;
        }
    }

    return so_gpu::ok ;
}
