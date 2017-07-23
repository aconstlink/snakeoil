//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "gl_33_api.h"

#include <snakeoil/gli/gl/gl.h>

using namespace so_gpu ;
using namespace so_gpu::so_gl ;

//*****************************************************************************************************
gl_33_api::gl_33_api( void_t )
{

}

//*****************************************************************************************************
gl_33_api::gl_33_api( gl_33_api && rhv ) : base_t( std::move(rhv) )
{
    _context = rhv._context ;
    rhv._context = nullptr ;

    _last_vao = GLuint(-1) ;
    _last_po = GLuint(-1) ;
    //_last_image = GLuint(-1) ;
    _last_framebuffer = GLuint(-1) ;
    _last_feedback_buffer = nullptr ;

    _attrib_stack = std::move( rhv._attrib_stack ) ;
    _state_sets = std::move( rhv._state_sets ) ;
    _the_state = rhv._the_state ;
}

//*****************************************************************************************************
gl_33_api::gl_33_api( gl_context_ptr_t ctx ) : _context(ctx)
{
}

//*****************************************************************************************************
gl_33_api::~gl_33_api( void_t )
{
}

//*****************************************************************************************************
gl_33_api::this_ptr_t gl_33_api::create( this_rref_t rhv, std::string const & purpose ) 
{
    return so_gpu::memory::alloc( std::move(rhv), purpose ) ;
}

//*****************************************************************************************************
api_type gl_33_api::get_type( void_t ) const
{
    return so_gpu::api_type::gl_33 ;
}

//*****************************************************************************************************
void_t gl_33_api::destroy( this_ptr_t ptr ) 
{
    so_gpu::memory::dealloc( ptr ) ;
}

//*****************************************************************************************************
void_t gl_33_api::destroy( void_t )
{
    this_t::destroy( this ) ;
}

//*****************************************************************************************************
so_gpu::result gl_33_api::initialize( void_t ) 
{
    {
        GLboolean const b = so_gli::gl::glIsEnabled( GL_DEPTH_TEST ) ;
        _the_state.depth_s.depth_test = b == GL_TRUE ? true : false ;
    }
    return so_gpu::ok ;
}

