//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "gl_33_api.h"

// api objects 

#include "../gl_any/objects/config.h"
#include "../gl_any/objects/program.h"
#include "../gl_any/objects/shader_variable.h"
#include "../gl_any/objects/vertex_buffer.h"
#include "../gl_any/objects/index_buffer.h"
#include "../gl_any/objects/sampler.h"
#include "../gl_any/objects/image.h"
#include "../gl_any/objects/sync.h"
#include "../gl_any/objects/data_buffer.h"
#include "../gl_any/objects/feedback_buffer.h"

#include "../gl_any/converter/gl_convert_clear_mode.h"
#include "../gl_any/converter/gl_convert_type.h"
#include "../gl_any/converter/gl_convert_primitive_type.h"
#include "../gl_any/converter/gl_convert_texture_filter.h"
#include "../gl_any/converter/gl_convert_texture_wrap.h"
#include "../gl_any/converter/gl_convert_feedback_primitve_type.h"

#include "../../../converter/convert_core_gfx_type.h"

// engine objects 

#include "../../../program/config.h"
#include "../../../program/program.h"
#include "../../../buffer/buffer.h"
#include "../../../buffer/ivertex_buffer.h"
#include "../../../buffer/idata_buffer.h"
#include "../../../buffer/feedback_buffer.h"

#include "../../../variable/shader_variable.h"
#include "../../../variable/variable_set.h"
#include "../../../variable/texture_variable.hpp"
#include "../../../variable/data_buffer_variable.hpp"

#include "../../../texture/texture.h"
#include "../../../texture/texture_2d.h"
#include "../../../image/iimage_2d.h"

#include "../../../sync/sync.h"

#include "../gl_any/log.h" // gl
#include <snakeoil/log/log.h>

using namespace so_gpu ;
using namespace so_gpu::so_gl ;

namespace drv = so_gpu::so_gl ;

using gl_log = so_gpu::so_gl::log ;

//*****************************************************************************************************
so_gpu::result gl_33_api::set_viewport( so_gpu::viewport_2d_cref_t viewport )
{
    _the_state.viewport_s.vp = viewport ;
    so_gli::gl::glViewport( GLsizei( viewport.get_x( ) ), GLsizei( viewport.get_y( ) ), GLsizei( viewport.get_width( ) ), GLsizei( viewport.get_height( ) ) ) ;
    if( gl_log::error( "[gl_33_api::set_viewport] : glViewport" ) ) return so_gpu::failed_gl_api ;
    
    return so_gpu::ok ;
}

//*****************************************************************************************************
so_gpu::result gl_33_api::set_clear_color( so_math::vec4f_t const & color )
{
    _the_state.color_s.clear_color = color ;
    so_gli::gl::glClearColor( color.x( ), color.y( ), color.z( ), color.w( ) ) ;
    if( gl_log::error( "[gl_33_api::set_clear_color] : glClearColor" ) ) return so_gpu::failed_gl_api ;

    return so_gpu::ok ;
}

//*****************************************************************************************************
so_gpu::result gl_33_api::clear( bool_t color, bool_t depth, bool_t stencil )
{
    so_gli::gl::glClear( so_gl::convert_to_clear_bit( color, depth, stencil ) ) ;
    if( gl_log::error( "[gl_33_api::clear] : glClear" ) ) return so_gpu::failed_gl_api ;
    
    return so_gpu::ok ;
}

//*****************************************************************************************************
so_gpu::result gl_33_api::execute( render_config_info_cref_t info ) 
{
    if( api_object_helper_t::has_no_driver_object(info.config_ptr) ) 
        return so_gpu::invalid_argument ;

    so_gpu::primitive_type const pt = info.config_ptr->get_primitive_type() ;

    if( pt == so_gpu::primitive_type::undefined )
    {
        return so_gpu::invalid_argument ;
    }

    so_gl::config * config_api = api_object_helper_t::get_cast_api_object<so_gl::config>(info.config_ptr) ;

    if( so_gpu::no_success( this_t::prepare_config( config_api->id, info.config_ptr, info.variable_set_index ) ) )
    {
        so_log::log::error("[gl_33_api::execute] : prepare_config") ;
        return so_gpu::failed ;
    }

    // begin transform feedback, if one is active, it will capture
    // all written primitives.
    begin_transform_feedback( pt, info.config_ptr->get_program() ) ;

    // render section
    {
        so_gpu::buffer_ptr_t vb = info.config_ptr->get_vertex_buffer() ;
        so_gpu::buffer_ptr_t ib = info.config_ptr->get_index_buffer() ;
        
        if( ib != nullptr )
        {
            size_t const max_elems = ib->get_num_elements() ;
            size_t ne = std::min( info.num_elements, max_elems ) ;

            /// convert element's core type to a gpu type
            auto const gpu_type = so_gpu::convert_from( ib->get_buffer_layout_element(0).get_type() ) ;
        
            so_gli::gl::glDrawElements( so_gl::convert( pt ), GLsizei(ne), so_gl::convert( gpu_type ), 0 ) ;
        
            if( gl_log::error( "[gl_33_api::execute] : glDrawElements" ) ) 
                return so_gpu::failed_gl_api ;
        }
        else
        {
            size_t const max_elems = vb->get_num_elements() ;
            size_t ne = std::min( info.num_elements, max_elems ) ;

            so_gli::gl::glDrawArrays( so_gl::convert( pt ), 0, GLsizei(ne) ) ;

            if( gl_log::error( "[gl_33_api::execute] : glDrawArrays" ) ) 
                return so_gpu::failed_gl_api ;
        }
    }

    // end transform feedback    
    end_transform_feedback() ;

    return so_gpu::ok ;
}

//*****************************************************************************************************
so_gpu::result gl_33_api::execute( so_gpu::feedback_buffer_ptr_t ptr, render_config_info_cref_t info ) 
{
    if( api_object_helper_t::has_no_driver_object(info.config_ptr) ) 
        return so_gpu::invalid_argument ;

    if( api_object_helper_t::has_no_driver_object(ptr) ) 
        return so_gpu::invalid_argument ;

    so_gpu::primitive_type const pt = info.config_ptr->get_primitive_type() ;

    if( pt == so_gpu::primitive_type::undefined )
    {
        return so_gpu::invalid_argument ;
    }

    auto * feedback_api = api_object_helper_t::get_cast_api_object<drv::feedback_buffer>(ptr) ;
    auto * config_api = api_object_helper_t::get_cast_api_object<drv::config>(info.config_ptr) ;

    if( so_gpu::no_success( this_t::prepare_config( config_api->id, info.config_ptr, info.variable_set_index ) ) )
    {
        so_log::log::error("[gl_33_api::execute] : prepare_config") ;
        return so_gpu::failed ;
    }

    if( _last_feedback_buffer == feedback_api )
    {
        this_t::use_feedback_buffer( nullptr ) ;
    }

    // we can use one transform feedback while rendering into another one.
    begin_transform_feedback( pt, info.config_ptr->get_program() ) ;

    so_gli::gl::glDrawTransformFeedback( so_gl::convert(pt), feedback_api->id ) ;
    if( gl_log::error( "[gl_33_api::execute] : glDrawTransformFeedback" ) ) 
                return so_gpu::failed_gl_api ;

    end_transform_feedback() ;    

    return so_gpu::ok ;
}

//*****************************************************************************************************
so_gpu::result gl_33_api::use_feedback_buffer( so_gpu::feedback_buffer_ptr_t ptr ) 
{
    if( ptr == nullptr )
    {
        so_gli::gl::glBindTransformFeedback( GL_TRANSFORM_FEEDBACK, 0 ) ;
        if( gl_log::error( "[gl_33_api::use_feedback_buffer] : glBindTransformFeedback" ) ) 
                return so_gpu::failed_gl_api ;

        _last_feedback_buffer = nullptr ;
        return so_gpu::ok ;
    }

    if( api_object_helper_t::has_no_driver_object(ptr) ) 
        return so_gpu::invalid_argument ;

    auto * api_obj = api_object_helper_t::get_cast_api_object<so_gl::feedback_buffer>(ptr) ;

    if( _last_feedback_buffer == nullptr || _last_feedback_buffer->id != api_obj->id )
    {
        so_gli::gl::glBindTransformFeedback( GL_TRANSFORM_FEEDBACK, api_obj->id ) ;
        if( gl_log::error( "[gl_33_api::bind_feedback] : glBindTransformFeedback" ) ) 
                return so_gpu::failed_gl_api ;

        _last_feedback_buffer = api_obj ;        
    }

    if( ptr->has_changed() )
    {
        size_t i = 0 ;
        for( auto buffer : ptr->get_buffers() )
        {
            if( api_object_helper_t::has_no_driver_object(ptr) )
            {
                so_log::log::warning("[gl_33_api::bind_feedback] : buffer in feedback buffer has no driver object.") ;
                continue ;
            }
            
            GLuint bid = GLuint(-1) ;
            GLsizei sib = GLsizei( buffer->get_sib() ) ;

            if( api_object_helper_t::is_of_api_object_type<so_gl::vertex_buffer>(buffer) )
            {
                bid = api_object_helper_t::get_cast_api_object<so_gl::vertex_buffer>(buffer)->id ;
            }
            else if( api_object_helper_t::is_of_api_object_type<so_gl::index_buffer>(buffer) )
            {
                bid = api_object_helper_t::get_cast_api_object<so_gl::index_buffer>(buffer)->id ;
            }
            else if( api_object_helper_t::is_of_api_object_type<so_gl::data_buffer>(buffer) )
            {
                bid = api_object_helper_t::get_cast_api_object<so_gl::data_buffer>(buffer)->bid ;
            }
            else
            {
                so_log::log::warning("[gl_33_api::bind_feedback] : unsupported buffer type.") ;
            }
            
            if( bid != GLuint(-1) )
            {
                so_gli::gl::glBindBufferRange( GL_TRANSFORM_FEEDBACK_BUFFER, GLuint(i), bid, 0, sib ) ;
                gl_log::error( "[gl_33_api::bind_feedback] : glBindTransformFeedback" ) ;
            }
            ++i ;
        }

        ptr->set_changed( false ) ;
    }   

    return so_gpu::ok ;
}

//*****************************************************************************************************
so_gpu::result gl_33_api::begin_transform_feedback( so_gpu::primitive_type pt, program_ptr_t program_ptr ) 
{
    if( _last_feedback_buffer == nullptr ) return so_gpu::ok ;

    // at first, the input type will determine the output type.
    so_gpu::primitive_type tf_pt = so_gl::deduce_transform_feedback(pt) ;
        
    // if the program has a vertex shader, and the output type is not undefined,
    // that output type is used for the transform feedback output type.
    if( program_ptr->get_vertex_shader()->get_output_type() != primitive_type::undefined )
    {
        tf_pt = program_ptr->get_vertex_shader()->get_output_type() ;
    }

    // if a geometry shader is present, the geometry shader's
    // output type will determine the output type.
    if( program_ptr->has_geometry_shader() )
    {
        tf_pt = so_gl::deduce_transform_feedback( 
            program_ptr->get_geometry_shader()->get_output_type() ) ;
    }

    {
        so_gli::gl::glBeginTransformFeedback( so_gl::convert(tf_pt) ) ;
        gl_log::error( "[gl_33_api::use_feedback_buffer] : glBeginTransformFeedback" ) ;
    }

    {
        so_gli::gl::glBeginQuery( GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, _last_feedback_buffer->qid ) ;
        gl_log::error( "[gl_33_api::use_feedback_buffer] : glBeginQuery - GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN" ) ;
    }

    return so_gpu::ok ;
}

//*****************************************************************************************************
so_gpu::result gl_33_api::end_transform_feedback( void_t ) 
{        
    if( _last_feedback_buffer == nullptr ) return so_gpu::ok ;

    so_gli::gl::glEndTransformFeedback() ;
    gl_log::error( "[gl_33_api::use_feedback_buffer] : glEndTransformFeedback" ) ;

    so_gli::gl::glEndQuery( GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN ) ;
    gl_log::error( "[gl_33_api::use_feedback_buffer] : glEndQuery - GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN" ) ;

    return so_gpu::ok ;
}

//*****************************************************************************************************
so_gpu::result gl_33_api::query_primitives_written( so_gpu::feedback_buffer_ptr_t ptr, bool_t wait_for_result ) 
{
    if( api_object_helper_t::has_no_driver_object(ptr) ) return so_gpu::invalid_argument ;

    auto * api_obj = api_object_helper_t::get_cast_api_object<drv::feedback_buffer>(ptr) ;

    ptr->set_num_primitives_written( size_t(0) ) ;

    GLuint is_available = wait_for_result ? GL_TRUE : GL_FALSE ;
    GLuint num_written = 0 ;

    if( !wait_for_result )
    {
        so_gli::gl::glGetQueryObjectuiv( api_obj->qid, GL_QUERY_RESULT_AVAILABLE, &is_available ) ;
        gl_log::error( "[gl_33_api::use_feedback_buffer] : glGetQueryObjectuiv - GL_QUERY_RESULT_AVAILABLE" ) ;
    }
    
    if( is_available == GL_TRUE )
    {
        so_gli::gl::glGetQueryObjectuiv( api_obj->qid, GL_QUERY_RESULT, &num_written ) ;
        gl_log::error( "[gl_33_api::use_feedback_buffer] : glGetQueryObjectuiv - GL_QUERY_RESULT" ) ;
        ptr->set_num_primitives_written( size_t(num_written) ) ;
    }
            
    return so_gpu::ok ;
}

//*****************************************************************************************************
so_gpu::result gl_33_api::flush( void_t )
{
    so_gli::gl::glFlush() ;
    if( gl_log::error("[gl_33_api::flush] : glFlush") ) return so_gpu::failed_gl_api ;
    return so_gpu::ok ;
}

//*****************************************************************************************************
so_gpu::result gl_33_api::finish( void_t )
{
    so_gli::gl::glFinish() ;
    if( gl_log::error("[gl_33_api::finish] : glFinish") ) return so_gpu::failed_gl_api ;
    return so_gpu::ok ;
}

//*****************************************************************************************************
so_gpu::result gl_33_api::sync_fence( sync_ptr_t ptr ) 
{
    if( api_object_helper_t::has_no_driver_object(ptr) ) return so_gpu::invalid_argument ;

    ptr->set_signaled(true) ;

    return so_gpu::ok ;
}

//*****************************************************************************************************
so_gpu::result gl_33_api::use_program( GLuint id, program_ptr_t ptr ) 
{
    if( _last_po == id ) return so_gpu::ok ;
    
    so_gli::gl::glUseProgram( id ) ;
    if( gl_log::error("[gl_33_api::use_program] : glUseProgram") ) 
        return so_gpu::failed_gl_api ;
    
    _last_po = id ;

    if( ptr->has_no_pixel_shader() && !_raster_discarded )
    {
        so_gli::gl::glEnable( GL_RASTERIZER_DISCARD ) ;
        if( gl_log::error("[gl_33_api::use_program] : glEnable GL_RASTERIZER_DISCARD") ) 
            return so_gpu::failed_gl_api ;

        _raster_discarded = true ;
    }
    else if( _raster_discarded )
    {
        so_gli::gl::glDisable( GL_RASTERIZER_DISCARD ) ;
        if( gl_log::error("[gl_33_api::use_program] : glDisable GL_RASTERIZER_DISCARD") ) 
            return so_gpu::failed_gl_api ;

        _raster_discarded = false ;
    }

    return so_gpu::ok ;
}

//*****************************************************************************************************
so_gpu::result gl_33_api::bind_image( GLuint id, GLuint tex_unit, GLenum tex_target ) 
{
    // do no check, because the image need to be bound!
    //if( _last_image == id ) return so_gpu::ok ;

    so_gli::gl::glActiveTexture( GLenum(GL_TEXTURE0 + tex_unit) ) ;
    if( gl_log::error("[gl_33_api::commit_textures] : glActiveTexture") ) 
        return so_gpu::failed_gl_api ;

    return use_image( id, tex_target ) ;
}

//*****************************************************************************************************
so_gpu::result gl_33_api::use_image( GLuint id, GLenum target ) 
{
    so_gli::gl::glBindTexture( target, id ) ;
    if( gl_log::error("[gl_33_api::use_image] : glBindTexture") ) 
        return so_gpu::failed_gl_api ;

    //_last_image = id ;

    // @todo set texture parameters if changed
    {
        //glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S , GL_REPEAT ) ;
        //glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T , GL_REPEAT ) ;		
        //glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER , GL_LINEAR ) ;		
        //glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER , GL_LINEAR ) ;
    }

    return so_gpu::ok ;
}

//*****************************************************************************************************
so_gpu::result gl_33_api::use_texture( GLuint tex_unit, GLuint sampler_id, texture_ptr_t ptr ) 
{
    so_gli::gl::glBindSampler( tex_unit, sampler_id ) ;
    if( gl_log::error("[gl_33_api::commit_textures] : glBindTexture") ) return so_gpu::failed_gl_api ;
        
    if( ptr->has_state_changed() )
    {
        if( so_log::log::error( ptr->get_image() == nullptr, "[gl_33_api::use_texture] : invalid image" ) ) 
            return so_gpu::failed ;

        if( ptr->get_image()->get_type() == so_gpu::image_type::color_2d )
        {
            texture_2d_ptr_t tex2d_ptr = reinterpret_cast<texture_2d_ptr_t>(ptr) ;

            auto const wraps = tex2d_ptr->get_texture_wrap() ;
            auto const filters = tex2d_ptr->get_texture_filter() ;

            so_gli::gl::glSamplerParameteri( sampler_id, GL_TEXTURE_WRAP_S, so_gl::convert(wraps.first) ) ;
            if( gl_log::error("[gl_33_api::commit_textures] : glSamplerParameteri") ) 
                return so_gpu::failed_gl_api ;

            so_gli::gl::glSamplerParameteri( sampler_id, GL_TEXTURE_WRAP_T , so_gl::convert(wraps.second) ) ;		
            if( gl_log::error("[gl_33_api::commit_textures] : glSamplerParameteri") ) 
                return so_gpu::failed_gl_api ;

            so_gli::gl::glSamplerParameteri( sampler_id, GL_TEXTURE_MIN_FILTER , so_gl::convert(filters.first) ) ;	
            if( gl_log::error("[gl_33_api::commit_textures] : glSamplerParameteri") ) 
                return so_gpu::failed_gl_api ;

            so_gli::gl::glSamplerParameteri( sampler_id, GL_TEXTURE_MAG_FILTER , so_gl::convert(filters.second) ) ;
            if( gl_log::error("[gl_33_api::commit_textures] : glSamplerParameteri") ) 
                return so_gpu::failed_gl_api ;
        }        

        ptr->set_state_changed( false ) ;
    }

    return so_gpu::ok ;    
}

//*****************************************************************************************************
so_gpu::result gl_33_api::use_vao( GLuint id, config_ptr_t config_ptr ) 
{
    if( _last_vao == id ) return so_gpu::ok ;
    
    so_gli::gl::glBindVertexArray(id) ;
    if( gl_log::error("[gl_33_api::use_vao] : glBindVertexArray") ) 
        return so_gpu::failed_gl_api ;

    _last_vao = id ;
    
    return so_gpu::ok ;
}

//*****************************************************************************************************
so_gpu::result gl_33_api::null_vao( void_t ) 
{
    if( _last_vao == 0 ) return so_gpu::ok ;

    so_gli::gl::glBindVertexArray( 0 ) ;
    if( gl_log::error( "[gl_33_api::end_vao] : glBindVertexArray" ) )
        return so_gpu::failed_gl_api ;

    _last_vao = 0 ;

    return so_gpu::ok ;
}

//*****************************************************************************************************
so_gpu::result gl_33_api::prepare_config( GLuint vao_id, config_ptr_t config_ptr, size_t var_set_id ) 
{
    so_gpu::program * prog_ptr = config_ptr->get_program() ;
    so_gl::program * program_api = api_object_helper_t::get_cast_api_object<so_gl::program>( prog_ptr ) ;
    
    if( so_gpu::no_success( this_t::use_vao( vao_id, config_ptr ) ) )
    {
        so_log::log::error("[gl_33_api::execute] : use_vao") ;  
        return so_gpu::failed ;
    }

    if( so_gpu::no_success( this_t::use_program( program_api->id, prog_ptr ) ) )
    {
        so_log::log::error("[gl_33_api::execute] : use_program") ;
    }
    
    if(config_ptr->has_config_changed())
    {
        this_t::bind_attributes( vao_id, config_ptr ) ;
        config_ptr->set_config_changed( false ) ;
    }

    /// @todo can this be evaluated outside of this function
    if( config_ptr->has_var_set_changed(var_set_id) )
    {
        this_t::bind_uniforms( config_ptr, var_set_id ) ;
        this_t::bind_textures( config_ptr, var_set_id ) ;

        config_ptr->set_var_set_changed( var_set_id, false ) ;
    }
    
    // if the config does not have its buffers loaded, 
    // do not proceed!
    if( so_gpu::no_success( this_t::check_buffer_size(
        config_ptr->get_index_buffer() != nullptr) ) )
    {
        return so_gpu::failed ;
    }

    GLuint tex_unit = 0 ;
    this_t::commit_textures( config_ptr, var_set_id, tex_unit ) ;
    this_t::commit_uniforms( config_ptr, var_set_id ) ;
    
    return so_gpu::ok ;
}

//*****************************************************************************************************
so_gpu::result gl_33_api::bind_attributes( GLuint /*id*/, config_ptr_t config_ptr ) 
{
    program_ptr_t prog = config_ptr->get_program() ;
    if( prog == nullptr ) return so_gpu::failed ;
    buffer_ptr_t vb_ptr = config_ptr->get_vertex_buffer() ;
    if( vb_ptr == nullptr ) return so_gpu::failed ;
    buffer_ptr_t ib_ptr = config_ptr->get_index_buffer() ;
    if( vb_ptr == nullptr ) return so_gpu::failed ;

    ivertex_buffer_ptr_t ivb_ptr = dynamic_cast<ivertex_buffer_ptr_t>(vb_ptr) ;
    if( ivb_ptr == nullptr ) return so_gpu::failed ;

    if( so_log::log::error( api_object_helper_t::has_no_driver_object(prog), 
        "[gl_33_api::bind_attributes] : no valid program object" ) )
        return so_gpu::failed ;

    if( so_log::log::error( api_object_helper_t::has_no_driver_object(vb_ptr), 
        "[gl_33_api::bind_attributes] : no valid vertex buffer object" ) ) 
        return so_gpu::failed ;

    so_gl::config * api_obj = api_object_helper_t::get_cast_api_object<so_gl::config>(config_ptr) ;
    
    // bind vertex/index buffer
    {
        {
            auto vb_api_obj = api_object_helper_t::get_cast_api_object<so_gl::vertex_buffer>(vb_ptr) ;
            so_gli::gl::glBindBuffer( GL_ARRAY_BUFFER, vb_api_obj->id ) ;
            if( gl_log::error("[gl_33_api::bind_attributes] : glBindBuffer GL_ARRAY_BUFFER ") ) 
                return so_gpu::failed_gl_api ;
        }
        
        {
            GLuint const ib_id = ib_ptr != nullptr ? 
                api_object_helper_t::get_cast_api_object<so_gl::index_buffer>(ib_ptr)->id : 0 ;

            so_gli::gl::glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ib_id) ;
            if( gl_log::error("[gl_33_api::bind_attributes] : glBindBuffer GL_ELEMENT_ARRAY_BUFFER ") ) 
                return so_gpu::failed_gl_api ;
        }
    }
    
    // disable old locations
    for( size_t i=0; i<api_obj->locations.size(); ++i )
    {
        so_gli::gl::glDisableVertexAttribArray( api_obj->locations[i] ) ;
    }
    api_obj->locations.clear() ;

    size_t uiStride = vb_ptr->get_buffer_layout().get_sib() ;
    size_t uiBegin = 0 ;
    size_t uiOffset = 0 ;

    auto const & vbl = ivb_ptr->get_vertex_buffer_layout() ;

    /// run over all vertex attributes
    for( size_t i=0; i<vbl.get_num_elements(); ++i )
    {
        vertex_attribute attribute = ivb_ptr->get_vertex_attribute(i) ;
        shader_variable_cptr_t variable_ptr = config_ptr->get_input_variable( attribute ) ;

        so_gpu::buffer_layout_element ble = vb_ptr->get_buffer_layout_element(i) ;

        // the offset must be added before the variable_ptr check, because
        // if an attribute is not bound doesn't mean, the attribute is not in the buffer!
        // In other words, the vertex layout wins! If the vertex layout sais, there is 
        // an attribute, the buffer will have an attribute at that position if bound or 
        // not. So that attribute must be jumped over.
        uiBegin += uiOffset ;
        uiOffset = ble.get_sib() ;

        // is there any shader variable for that vertex attribute of the vertex buffer
        if( variable_ptr == nullptr || api_object_helper_t::has_no_driver_object(variable_ptr) ) continue ;

        GLuint attribute_location = api_object_helper_t::get_cast_api_object<so_gl::shader_variable>(variable_ptr)->id ;

        const GLint size = (GLint)so_core::size_of(ble.get_type_structure()) ;
        const GLenum type = so_gl::convert( so_gpu::convert_from(ble.get_type()) ) ;

        so_gli::gl::glEnableVertexAttribArray(attribute_location) ;
        if( gl_log::error("[gl_33_api::bind_attributes] : glEnableVertexAttribArray") ) continue ;

        so_gli::gl::glVertexAttribPointer( 
            attribute_location, 
            size,
            type,
            GL_FALSE, 
            (GLsizei)uiStride, 
            reinterpret_cast<const GLvoid*>(uiBegin)
            ) ;
        if( gl_log::error("[gl_33_api::bind_attributes] : glVertexAttribPointer") ) continue ;

        api_obj->locations.push_back( attribute_location ) ;
    }

    return so_gpu::ok ;
}

//*****************************************************************************************************
so_gpu::result gl_33_api::bind_uniforms( config_ptr_t conf_ptr, size_t var_set_id ) 
{
    program_ptr_t prog = conf_ptr->get_program() ;
    auto * var_set_ptr = conf_ptr->get_variable_set(var_set_id) ;
    if( so_log::log::error( var_set_ptr == nullptr, 
        "[gl_33_api::bind_uniforms] : invalid var set id." ) )
        return so_gpu::failed ;

    for( auto item : var_set_ptr->data_variables() )
    {
        auto * shadar = prog->find_shader_variable( item.second ) ;
        auto * var_ptr = item.first ;
        if( so_log::log::warning(shadar == nullptr,
            "[gl_33_api::bind_uniforms] : no corresponding shader variable : " + item.second) ) 
        {
            // kick out?
            continue ; 
        }
        if( so_log::log::error(api_object_helper_t::has_no_driver_object(shadar),
            "[gl_33_api::bind_uniforms] : no corresponding shader variable : "+ item.second) ) 
            continue ; 
        if( so_log::log::error(api_object_helper_t::has_no_driver_object(var_ptr),
            "[gl_33_api::bind_uniforms] : no config variable : " + item.second) ) 
            continue ; 

        auto * shadar_api_obj = api_object_helper_t::get_cast_api_object<drv::shader_variable>( shadar ) ;
        auto * variable_api_obj = api_object_helper_t::get_cast_api_object< drv::variable> (var_ptr) ;

        variable_api_obj->loc = shadar_api_obj->id ;
    }

    return so_gpu::ok ;
}

//*****************************************************************************************************
so_gpu::result gl_33_api::bind_textures( config_ptr_t conf_ptr, size_t var_set_id) 
{
    program_ptr_t prog = conf_ptr->get_program() ;
    auto * var_set_ptr = conf_ptr->get_variable_set(var_set_id) ;
    if( so_log::log::error( var_set_ptr == nullptr, "[gl_33_api::bind_uniforms] : invalid var set id." ) )
        return so_gpu::failed ;

    // bind gl shader variable location to textures
    for( auto item : var_set_ptr->texture_variables() )
    {
        auto * shadar = prog->find_shader_variable( item.second ) ;
        auto * var_ptr = item.first ;
        
        if( so_log::log::error(api_object_helper_t::has_no_driver_object(shadar),
            "[gl_33_api::bind_uniforms] : no corresponding shader variable:" + item.second) ) 
            continue ; 

        if( so_log::log::error(api_object_helper_t::has_no_driver_object(var_ptr),
            "[gl_33_api::bind_uniforms] : no config variable : " + item.second) ) 
            continue ; 

        auto * shadar_api_obj = api_object_helper_t::get_cast_api_object<drv::shader_variable>( shadar ) ;
        auto * variable_api_obj = api_object_helper_t::get_cast_api_object<drv::variable>(var_ptr) ;        

        variable_api_obj->loc = shadar_api_obj->id ;
    }

    // bind gl shader variable location to texture buffers
    for( auto item : var_set_ptr->data_buffer_variables() )
    {
        auto * shadar = prog->find_shader_variable( item.second ) ;
        auto * var_ptr = item.first ;
        
        if( so_log::log::error(api_object_helper_t::has_no_driver_object(shadar),
            "[gl_33_api::bind_uniforms] : no corresponding shader variable : " + item.second) ) 
            continue ; 

        if( so_log::log::error(api_object_helper_t::has_no_driver_object(var_ptr),
            "[gl_33_api::bind_uniforms] : no config variable : " + item.second) ) 
            continue ; 

        auto * shadar_api_obj = api_object_helper_t::get_cast_api_object<drv::shader_variable>( shadar ) ;
        auto * variable_api_obj = api_object_helper_t::get_cast_api_object<drv::variable>(var_ptr) ;        

        variable_api_obj->loc = shadar_api_obj->id ;
    }

    return so_gpu::ok ;
}

//*****************************************************************************************************
so_gpu::result gl_33_api::commit_uniforms( config_ptr_t conf_ptr, size_t var_set_index ) 
{
    auto * var_set_ptr = conf_ptr->get_variable_set(var_set_index) ;
    if( so_log::log::error( var_set_ptr == nullptr, "[gl_33_api::commit_uniforms] : invalid var set id." ) )
        return so_gpu::failed ;

    for( auto item : var_set_ptr->data_variables() )
    {
        if( so_log::log::error(api_object_helper_t::has_no_driver_object(item.first),
            "[gl_33_api::bind_uniforms] : no config variable : " + item.second ) ) 
            continue ; 
        auto * variable_api_obj = api_object_helper_t::get_cast_api_object< drv::variable> (item.first) ;        
        
        variable_api_obj->do_uniform( variable_api_obj->loc, 1, variable_api_obj->data_ptr ) ;
    }

    return so_gpu::ok ;
}

//*****************************************************************************************************
so_gpu::result gl_33_api::commit_textures( config_ptr_t conf_ptr, size_t var_set_index, GLuint & tex_unit ) 
{
    auto * var_set_ptr = conf_ptr->get_variable_set(var_set_index) ;
    if( so_log::log::error( var_set_ptr == nullptr, 
        "[gl_33_api::commit_textures] : invalid var set id." ) )
        return so_gpu::failed ;

    // bind texture to texture unit
    for( auto item : var_set_ptr->texture_variables() )
    {
        auto * var_ptr = item.first ;
        auto * tex_ptr = var_ptr->get_texture() ;
        
        if( so_log::log::error(api_object_helper_t::has_no_driver_object(var_ptr),
            "[gl_33_api::commit_textures] : no api object in variable: " + item.second) ) 
            continue ; 

        if( so_log::log::error(api_object_helper_t::has_no_driver_object(tex_ptr),
            "[gl_33_api::commit_textures] : no api object in texture: " + item.second) ) 
            continue ; 

        if( so_log::log::error(api_object_helper_t::has_no_driver_object(tex_ptr->get_image()),
            "[gl_33_api::commit_textures] : no api object in image: " + item.second) ) 
            continue ; 

        auto * var_api_obj = api_object_helper_t::get_cast_api_object<drv::variable>( var_ptr ) ;
        auto * img_api_obj = api_object_helper_t::get_cast_api_object<drv::image>( tex_ptr->get_image() ) ;
        auto * tex_api_obj = api_object_helper_t::get_cast_api_object<drv::sampler>( tex_ptr ) ;

        if( so_gpu::no_success( 
            this_t::bind_image( img_api_obj->id, tex_unit, img_api_obj->image_target ) ) ) 
            continue ;

        if( so_gpu::no_success( 
            this_t::use_texture( tex_unit, tex_api_obj->id, tex_ptr ) ) ) 
            continue ;

        so_gli::gl::glUniform1i( var_api_obj->loc, tex_unit ) ;
        if( gl_log::error("[gl_33_api::commit_textures] : glUniform1i") ) continue ;

        ++tex_unit ;
    }

    // bind texture buffer to texture unit
    for( auto item : var_set_ptr->data_buffer_variables() )
    {
        auto * var_ptr = item.first ;
        auto * buf_ptr = var_ptr->get_buffer() ;
        
        if( so_log::log::error(api_object_helper_t::has_no_driver_object(var_ptr),
            "[gl_33_api::commit_textures] : no api object in variable: " + item.second) ) 
            continue ; 

        if( so_log::log::error(api_object_helper_t::has_no_driver_object(buf_ptr),
            "[gl_33_api::commit_textures] : no api object in texture: " + item.second) ) 
            continue ; 

        auto * var_api_obj = api_object_helper_t::get_cast_api_object<drv::variable>( var_ptr ) ;
        auto * buf_api_obj = api_object_helper_t::get_cast_api_object<drv::data_buffer>( buf_ptr ) ;
        
        if( so_gpu::no_success( 
            this_t::bind_image( buf_api_obj->tid, tex_unit, GL_TEXTURE_BUFFER ) ) ) 
            continue ;

        so_gli::gl::glUniform1i( var_api_obj->loc, tex_unit ) ;
        if( gl_log::error("[gl_33_api::commit_textures] : glUniform1i") ) continue ;

        ++tex_unit ;
    }

    return so_gpu::ok ;
}

