//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "gl_33_api.h"

//#include "../object.h"
#include "../../api_object.h"


// api objects 

#include "../gl_any/objects/framebuffer.h"
#include "../gl_any/objects/image.h"

#include "../gl_any/converter/gl_convert_color_attachment.h"

// engine objects 

#include "../../../image/iimage_2d.h"
#include "../../../texture/texture_2d.h"
#include "../../../texture/depth_texture_2d.h"
#include "../../../framebuffer/framebuffer_2d.h"

#include "../gl_any/log.h"

using namespace so_gpu ;
using namespace so_gpu::so_gl ;

namespace drv = so_gpu::so_gl ;
using gl_log = so_gpu::so_gl::log ;


//*****************************************************************************************************
so_gpu::result gl_33_api::use_framebuffer( GLuint id ) 
{
    GLuint use_id = id != GLuint(-1) ? id : 0 ;
    
    so_gli::gl::glBindFramebuffer( GL_FRAMEBUFFER, use_id ) ;
    if( gl_log::error("[gl_33_api::use_framebuffer] : glBindFramebuffer") ) 
        return so_gpu::failed_gl_api ;

    return so_gpu::ok ;
}

//*****************************************************************************************************
so_gpu::result gl_33_api::create_framebuffer( so_gpu::framebuffer_2d_ptr_t ptr )
{
    if( api_object_helper_t::has_driver_object( ptr ) ) return so_gpu::ok ;

    GLuint id = GLuint(-1) ;
    so_gli::gl::glGenFramebuffers(1, &id) ;
    if( gl_log::error("[gl_33_api::create_framebuffer] : glGenFramebuffers") ) 
        return so_gpu::failed_gl_api ;

    api_object_helper_t::set_driver_object( ptr, so_gpu::memory::alloc( drv::framebuffer(id) ) ) ;
    
    return so_gpu::ok ;
}

//*****************************************************************************************************
so_gpu::result gl_33_api::release_framebuffer( so_gpu::framebuffer_2d_ptr_t  ptr )
{
    if( api_object_helper_t::has_no_driver_object( ptr ) ) return so_gpu::ok ;

    auto * api_obj = api_object_helper_t::get_cast_api_object<drv::framebuffer>(ptr) ;

    {
        GLuint id = api_obj->id ;
        so_gli::gl::glDeleteFramebuffers(1, &id) ;
        gl_log::error("[gl_33_api::release_framebuffer] : glDeleteFramebuffers") ;
    
        if( _last_framebuffer == id ) _last_framebuffer = GLuint(-1) ;
    }

    so_gpu::memory::dealloc( api_obj ) ;
    api_object_helper_t::set_driver_object( ptr, nullptr ) ;

    return so_gpu::ok ;

}

//*****************************************************************************************************
so_gpu::result gl_33_api::validate( so_gpu::framebuffer_2d_ptr_t ptr )
{
    if( api_object_helper_t::has_no_driver_object( ptr ) ) return so_gpu::invalid_argument ;

    auto * api_obj_ptr = api_object_helper_t::get_cast_api_object<drv::framebuffer>(ptr) ;

    if( so_gpu::no_success( this_t::use_framebuffer( api_obj_ptr->id ) ) )
        return so_gpu::failed ;

    // color targets
    {
        size_t attachment = 0 ;
        for( auto * iter : ptr->get_color_targets() )
        {
            if( so_log::log::warning( api_object_helper_t::has_no_driver_object(iter->get_image()), 
                "[gl_33_api::validate] : no valid color image.") )
                continue ;

            auto * img_api_obj = api_object_helper_t::get_cast_api_object<drv::image>(iter->get_image()) ;
            so_gli::gl::glFramebufferTexture2D( GL_FRAMEBUFFER, convert_color_attachment(attachment), 
                img_api_obj->image_target, img_api_obj->id, 0 ) ; 
            if( gl_log::error("[gl_33_api::validate] : glFramebufferTexture2D : color") ) continue ;

            ++attachment ;
        }
    }

    // depth target
    if( api_object_helper_t::has_driver_object( ptr->get_depth_target() ) &&
        api_object_helper_t::has_driver_object( ptr->get_depth_target()->get_image() ) )
    {
        auto * img_api_obj = api_object_helper_t::get_cast_api_object<drv::image>( ptr->get_depth_target()->get_image() ) ;

        so_gli::gl::glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, img_api_obj->image_target, img_api_obj->id, 0 ) ;   
        gl_log::error("[gl_33_api::validate] : glFramebufferTexture2D : depth") ;
    }

    // stencil
    {
    }

    GLenum status = so_gli::gl::glCheckFramebufferStatus( GL_FRAMEBUFFER ) ;
    if( gl_log::error("[gl_33_api::validate] : glCheckFramebufferStatus") ) 
        return so_gpu::failed_gl_api ;    

    if( gl_log::error(status != GL_FRAMEBUFFER_COMPLETE, status, "[gl_33_api::validate] : INCOMPLETE.") ) 
    {
        // @todo print more precise error message using the status
        return so_gpu::failed_gl_api ;  
    }  

    return this_t::use_framebuffer( _last_framebuffer ) ;
}

//*****************************************************************************************************
so_gpu::result gl_33_api::use_framebuffer( so_gpu::framebuffer_2d_ptr_t ptr )
{
    return this_t::use_framebuffer( ptr, so_gpu::color_target::all ) ;
}

//*****************************************************************************************************
so_gpu::result gl_33_api::use_framebuffer( so_gpu::framebuffer_2d_ptr_t ptr, so_gpu::color_target ct ) 
{
    if( ptr == nullptr )
    {
        _last_framebuffer = GLuint(-1) ;
        so_gli::gl::glBindFramebuffer(GL_FRAMEBUFFER, 0) ;
        return gl_log::error("[gl_33_api::use_framebuffer] : glBindFramebuffer") ?
            so_gpu::failed_gl_api : so_gpu::ok ;        
    }

    if( api_object_helper_t::has_no_driver_object( ptr ) ) return so_gpu::invalid_argument ;

    auto * api_obj_ptr = api_object_helper_t::get_cast_api_object<drv::framebuffer>(ptr) ;

    if( _last_framebuffer != api_obj_ptr->id )
    {
        so_gli::gl::glBindFramebuffer( GL_FRAMEBUFFER, api_obj_ptr->id ) ;
        if( gl_log::error("[gl_33_api::use_framebuffer] : glBindFramebuffer") ) 
            return so_gpu::failed_gl_api ;

        _last_framebuffer = api_obj_ptr->id ;
    }

    {
        GLenum attachments[15] ;

        size_t num_color = ptr->get_num_color_targets() ;
        if( num_color == 0 ) ct = so_gpu::color_target::none ;

        if( ct == so_gpu::color_target::none ) num_color = 0 ;
        else if (num_color > 0 && ct == so_gpu::color_target::all )
        {
            for( size_t i=0; i<num_color; ++i )
            {
                attachments[i] = so_gl::convert_color_attachment(i) ;
            }
        }
        else if( num_color > 0 )
        {
            attachments[0] = so_gl::convert(ct) ;
            num_color = 1 ;
        }

        so_gli::gl::glDrawBuffers( GLsizei(num_color), attachments ) ;
        if( gl_log::error("[gl_33_api::use_framebuffer] : glDrawBuffers") ) 
            return so_gpu::failed_gl_api ;
    }

    return so_gpu::ok ;
}

