//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "gl_33_api.h"

#include "../gl_any/converter/gl_convert_render_state.h"
#include "../gl_any/converter/gl_convert_front_face.h"
#include "../gl_any/converter/gl_convert_cull_mode.h"
#include "../gl_any/converter/gl_convert_fill_mode.h"
#include "../gl_any/converter/gl_convert_blend_factor.h"

#include "../gl_any/log.h"

using namespace so_gpu ;
using namespace so_gpu::so_gl ;

namespace drv = so_gpu::so_gl ;
using gl_log = so_gpu::so_gl::log ;

//*****************************************************************************************************
so_gpu::result gl_33_api::enable( render_state rs ) 
{
    if( rs == so_gpu::render_state::undefined ) return so_gpu::invalid_argument ;

    GLenum state = so_gl::convert( rs ) ;

    switch( rs ) 
    {
    case so_gpu::render_state::blend:
        if( !_the_state.blend_s.blend ) 
        {
            _the_state.blend_s.blend = true ;
            so_gli::gl::glEnable( state ) ;
        }
        break ;
    case so_gpu::render_state::depth_test:
        if( !_the_state.depth_s.depth_test ) 
        {
            _the_state.depth_s.depth_test = true ;
            so_gli::gl::glEnable( GL_DEPTH_TEST ) ;
        }
        break ;
    case so_gpu::render_state::depth_write:
    {
        if( !_the_state.depth_s.depth_write ) 
        {
            _the_state.depth_s.depth_write = true ;
            so_gli::gl::glDepthMask( GL_TRUE ) ;
        }
        break ;
    }
    case so_gpu::render_state::cullmode:
        if( !_the_state.polygon_s.cull ) 
        {
            _the_state.polygon_s.cull = true ;
            so_gli::gl::glEnable( state ) ;
        }
        break ;
            
        default: break ;
    }

    return so_gpu::ok ;
}

//*****************************************************************************************************
so_gpu::result gl_33_api::disable( render_state rs )
{
    if( rs == so_gpu::render_state::undefined ) return so_gpu::invalid_argument ;

    GLenum state = so_gl::convert( rs ) ;

    switch( rs ) 
    {
    case so_gpu::render_state::blend:
        if( _the_state.blend_s.blend ) 
        {
            _the_state.blend_s.blend = false ;
            so_gli::gl::glDisable( state ) ;
        }
        break ;

    case so_gpu::render_state::depth_test:
        if( _the_state.depth_s.depth_test ) 
        {
            _the_state.depth_s.depth_test = false ;
            so_gli::gl::glDisable( GL_DEPTH_TEST ) ;
        }
        break ;

    case so_gpu::render_state::depth_write:

        if( _the_state.depth_s.depth_write ) 
        {
            _the_state.depth_s.depth_write = false ;
            so_gli::gl::glDepthMask( GL_FALSE ) ;
        }
        break ;

    case so_gpu::render_state::cullmode:
        if( _the_state.polygon_s.cull ) 
        {
            _the_state.polygon_s.cull = false ;
            so_gli::gl::glDisable( state ) ;
        }
        break ;

    default: break ;

    }

    return so_gpu::ok ;
}

//*****************************************************************************************************
so_gpu::result gl_33_api::set_state( cull_mode /*cm*/ )
{

    return so_gpu::ok ;
}

//*****************************************************************************************************
so_gpu::result gl_33_api::set_state( front_face ff )
{
    _the_state.polygon_s.ff = ff ;
    so_gli::gl::glFrontFace( convert( ff ) ) ;
    if( gl_log::error( "[gl_33_api::set_state] : glFrontFace " ) ) return so_gpu::failed_gl_api ;

    return so_gpu::ok ;
}

//*****************************************************************************************************
so_gpu::result gl_33_api::set_state( cull_mode cm, fill_mode fm )
{
    _the_state.polygon_s.cm_fill = cm ;
    _the_state.polygon_s.fm = fm ;
    const GLenum cull_mode = convert( cm ) ;
    const GLenum fill_mode = convert( fm ) ;
    so_gli::gl::glPolygonMode( cull_mode, fill_mode ) ;
    if( gl_log::error( "[gl_33_api::set_state] : glPolygonMode " ) ) return so_gpu::failed_gl_api ;

    return so_gpu::ok ;
}

//*****************************************************************************************************
so_gpu::result gl_33_api::set_state( blend_factor src, blend_factor dst )
{
    _the_state.blend_s.src_blend_factor = src ;
    _the_state.blend_s.dst_blend_factor = dst ;
    const GLenum glsrc = convert( src ) ;
    const GLenum gldst = convert( dst ) ;
    so_gli::gl::glBlendFunc( glsrc, gldst ) ;
    if( gl_log::error( "[gl_33_api::set_state] : glBlendFunc " ) ) return so_gpu::failed_gl_api ;

    return so_gpu::ok ;
}

//*****************************************************************************************************
so_gpu::result gl_33_api::set_state( blend_factor /*src_color*/, blend_factor /*dst_color*/, 
    blend_factor /*src_alpha*/, blend_factor /*dst_alpha*/ ) 
{
    return so_gpu::ok ;
}

//*****************************************************************************************************
so_gpu::result gl_33_api::set_state( blend_function /*func*/ ) 
{
    return so_gpu::ok ;
}

//*****************************************************************************************************
so_gpu::result gl_33_api::set_state( blend_function /*func_color*/, blend_function /*func_alpha*/ ) 
{
    return so_gpu::ok ;
}

//*****************************************************************************************************
so_gpu::result gl_33_api::push_state( attribute_state_field const & ps ) 
{
    if( ps.is_empty( ) ) return so_gpu::ok ;

    _attrib_stack.push( ps ) ;
    _state_sets.push( _the_state ) ;

    return so_gpu::ok ;
}

//*****************************************************************************************************
so_gpu::result gl_33_api::pop_state( void_t ) 
{
    if( so_core::is_not( _attrib_stack.has_item() ) )
        return so_gpu::ok ;

    attribute_state_field ps = _attrib_stack.pop( ) ;
    state_sets ss = _state_sets.pop( ) ;

    return this->change_states( ps, &ss ) ;
}

//*****************************************************************************************************
so_gpu::result gl_33_api::change_states( 
    attribute_state_field_cref_t af, state_sets_cptr_t states_ptr ) 
{
    if( states_ptr == nullptr )
        return so_gpu::invalid_argument ;

    state_sets_cref_t ss = *states_ptr ;

    if( af.has_set( so_gpu::attribute_state::color ) ) 
    {
        this->set_clear_color( ss.color_s.clear_color ) ;
    }
    if( af.has_set( so_gpu::attribute_state::blend ) ) 
    {
        if( ss.blend_s.blend ) this->enable( so_gpu::render_state::blend ) ;
        else this->disable( so_gpu::render_state::blend ) ;

        if( ss.blend_s.blend_factor_separate ) 
        {
            this->set_state( ss.blend_s.src_color_blend_factor, ss.blend_s.dst_color_blend_factor,
                    ss.blend_s.src_alpha_blend_factor, ss.blend_s.dst_alpha_blend_factor ) ;
        }
        else 
        {
            this->set_state( ss.blend_s.src_blend_factor, ss.blend_s.dst_alpha_blend_factor ) ;
        }

        if( ss.blend_s.blend_func_separate ) 
        {
            this->set_state( ss.blend_s.color_blend_func, ss.blend_s.alpha_blend_func ) ;
        }
        else 
        {
            this->set_state( ss.blend_s.blend_func ) ;
        }
    }
    if( af.has_set( so_gpu::attribute_state::depth ) ) 
    {
        
        if( ss.depth_s.depth_write ) this->enable( so_gpu::render_state::depth_write ) ;
        else this->disable( so_gpu::render_state::depth_write ) ;

        if( ss.depth_s.depth_test ) this->enable( so_gpu::render_state::depth_test ) ;
        else this->disable( so_gpu::render_state::depth_test ) ;
    }
    if( af.has_set( so_gpu::attribute_state::stencil ) ) 
    {
        // GL_STENCIL_TEST enable bit
        // Stencil function and reference value
        // Stencil fail, pass, and depth buffer pass actions
        // Stencil buffer clear value
        // Stencil buffer writemask

    }
    /*if( af.has_set(psa_enable) )
    {
        // GL_BLEND flag
        // GL_CULL_FACE flag
        // GL_DEPTH_TEST flag
        // GL_SAMPLE_ALPHA_TO_COVERAGE flag
        // GL_SAMPLE_ALPHA_TO_ONE flag
        // GL_SAMPLE_COVERAGE flag
        // GL_SCISSOR_TEST flag
        // GL_STENCIL_TEST flag
    }*/
    if( af.has_set( so_gpu::attribute_state::polygon ) ) 
    {
        // GL_CULL_FACE enable bit
        // GL_CULL_FACE_MODE value
        // GL_FRONT_FACE indicator
        // GL_POLYGON_MODE setting
        if( ss.polygon_s.cull ) this->enable( so_gpu::render_state::cullmode ) ;
        else this->disable( so_gpu::render_state::cullmode ) ;

        this->set_state( ss.polygon_s.cm ) ;
        this->set_state( ss.polygon_s.ff ) ;
        this->set_state( ss.polygon_s.cm_fill, ss.polygon_s.fm ) ;
    }
    if( af.has_set( so_gpu::attribute_state::viewport ) ) 
    {
        // Depth range (near and far)
        // Viewport origin and extent
        this->set_viewport( ss.viewport_s.vp ) ;
    }

    return so_gpu::ok ;
}

//*****************************************************************************************************
float_t gl_33_api::line_width( float_t const w )
{
    GLfloat ret = 0.0f ;
    so_gli::gl::glGetFloatv( GL_LINE_WIDTH, &ret ) ;
    so_gli::gl::glLineWidth( GLfloat( w ) ) ;

    return float_t( ret ) ;
}