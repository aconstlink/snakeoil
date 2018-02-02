//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "wgl_context.h"

#include "../win32/win32_window_handle.h"

#include <snakeoil/core/assert.h>

#include <snakeoil/gli/gl/gl.h>
#include <snakeoil/gli/wgl/wgl.h>

#include <snakeoil/std/string/split.hpp>

#include <snakeoil/log/global.h>

using namespace so_app ;
using namespace so_app::so_win32 ;

//***********************************************************************
wgl_context::wgl_context( void_t )
{
    
}

//***********************************************************************
wgl_context::wgl_context( HWND hwnd, HGLRC ctx ) 
{
    _hwnd = hwnd ;
    _hrc = ctx ;
}

//***********************************************************************
wgl_context::wgl_context( this_rref_t rhv )
{
    *this = std::move( rhv ) ;
}

//***********************************************************************
wgl_context::~wgl_context( void_t )
{
    this_t::deactivate() ;

    if( _hrc != NULL )
        wglDeleteContext( _hrc ) ;
}

//***********************************************************************
wgl_context::this_ref_t wgl_context::operator = ( this_rref_t rhv )
{
    _hwnd = rhv._hwnd ;
    rhv._hwnd = NULL ;
    _hdc = rhv._hdc ;
    rhv._hdc = NULL ;
    _hrc = rhv._hrc ;
    rhv._hrc = NULL ;

    return *this ;
}

//***********************************************************************
wgl_context::this_ptr_t wgl_context::create( this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_app::memory::alloc( std::move( rhv ), p ) ;
}

//***********************************************************************
void_t wgl_context::destroy( this_ptr_t ptr )
{
    so_app::memory::dealloc( ptr ) ;
}

//***********************************************************************
so_app::result wgl_context::activate( void_t ) 
{
    so_assert( _hdc == NULL ) ;

    _hdc = GetDC( _hwnd ) ;

    if( so_log::global::error( wglMakeCurrent( _hdc, _hrc ) == FALSE, 
        "[wgl_context::activate] : wglMakeCurrent") ) 
        return so_app::failed_wgl ;
        
    return so_app::ok ;
}

//***********************************************************************
so_app::result wgl_context::deactivate( void_t ) 
{
    if( _hdc == NULL ) return so_app::ok ;

    if( so_log::global::error( wglMakeCurrent( 0,0 ) == FALSE, 
        "[wgl_context::deactivate] : wglMakeCurrent") ) 
        return so_app::failed_wgl ;

    if( so_log::global::error( ReleaseDC( _hwnd, _hdc ) == FALSE, 
        "[wgl_context::deactivate] : ReleaseDC") ) 
        return so_app::failed_wgl ;
    
    _hdc = NULL ;

    return so_app::ok ;
}

//***********************************************************************
so_app::result wgl_context::vsync( bool_t on_off ) 
{
    so_app::result const res = this_t::is_extension_supported("WGL_EXT_swap_control") ;

    if( so_log::global::error( so_app::no_success(res), 
        "[wgl_context::vsync] : vsync not supported." ) ) 
        return res ;
    
    if( so_log::global::error( so_gli::so_wgl::wgl::wglSwapInterval(on_off) != TRUE, 
        "[wgl_context::vsync] : wglSwapIntervalEXT" ) )
        return so_app::failed_wgl ;

    return so_app::ok ;
}

//***********************************************************************
so_app::result wgl_context::swap( void_t ) 
{
    if( _hdc == NULL ) 
        return so_app::invalid_win32_handle ;

    if( so_log::global::error( SwapBuffers( _hdc ) == FALSE, 
        "[wgl_context::swap] : SwapBuffers") ) 
        return so_app::failed_wgl ;
    
    return so_app::ok ;
}

//***********************************************************************
irender_context_ptr_t wgl_context::create_shared_context( void_t )
{    
    return this_t::create_shared() ;
}

//***********************************************************************
so_app::result wgl_context::create_context( gl_info_cref_t gli, iwindow_handle_ptr_t wnd_ptr ) 
{
    if( so_log::global::error( wnd_ptr == nullptr, 
        "[wgl_context::create_context] : Window handle is nullptr" ) )
        return so_app::invalid_argument ;
    
    _hwnd = this_t::get_win32_handle(wnd_ptr) ;
    
    if( so_log::global::error( _hwnd == NULL, 
        "[wgl_context::create_context] : Window handle is no win32 handle." ) )
        return so_app::invalid_argument ;

    return this_t::create_the_context( gli ) ;
}

//***********************************************************************
wgl_context::this_ptr_t wgl_context::create_shared( void_t ) 
{
    HGLRC ctx = this_t::create_the_shared() ;
    return so_app::memory::alloc( this_t( _hwnd, ctx), "[wgl_context::create_shared_context]" ) ;
}

//***********************************************************************
void_t wgl_context::destroy_shared( this_ptr_t shared_ptr ) 
{
    so_app::memory::dealloc( shared_ptr ) ;
}

//***********************************************************************
so_app::result wgl_context::is_extension_supported( so_std::string_cref_t extension_name ) 
{
    so_std::vector< so_std::string_t > ext_list ;
    if( so_app::no_success( get_wgl_extension(ext_list) ) ) return so_app::failed_wgl ;

    auto const iter = std::find( ext_list.begin(), ext_list.end(), extension_name ) ;

    return iter != ext_list.end() ? 
        so_app::ok : so_app::invalid_extension ;
}

//***********************************************************************
so_app::result wgl_context::get_wgl_extension( so_std::vector< so_std::string_t > & ext_list )
{
    if( !so_gli::so_wgl::wgl::wglGetExtensionsString ) 
        return so_app::invalid_extension ;

    char_cptr_t ch = so_gli::so_wgl::wgl::wglGetExtensionsString( _hdc ) ;
    so_std::string_ops::split( so_std::string_t(ch), ' ', ext_list ) ;

    return so_app::ok ;
}

//***********************************************************************
so_app::result wgl_context::get_gl_extension( so_std::vector< so_std::string_t > & ext_list )
{
    const GLubyte * ch = so_gli::gl::glGetString( GL_EXTENSIONS ) ;
    if( !ch ) return so_app::failed ;

    so_std::string_ops::split( so_std::string_t(char_cptr_t(ch)), ' ', ext_list ) ;

    return so_app::ok ;
}

//***********************************************************************
so_app::result wgl_context::get_gl_version( so_app::gl_version & version ) const 
{
    const GLubyte* ch = so_gli::gl::glGetString(GL_VERSION) ;
    if( !ch ) return failed ;

    std::string version_string = std::string((const char*)ch) ;

    GLint major = 0 ;
    GLint minor = 0 ;

    {
        so_gli::gl::glGetIntegerv( GL_MAJOR_VERSION, &major ) ;
        GLenum err = so_gli::gl::glGetError() ;
        if( err != GL_NO_ERROR )
        {
            so_std::string_t const es = std::to_string(err) ;
            so_log::global::error( "[wgl_context::get_gl_version] : get gl major <"+es+">" ) ;
        }
    }
    {
        so_gli::gl::glGetIntegerv( GL_MINOR_VERSION, &minor) ;
        GLenum err = so_gli::gl::glGetError() ;
        if( err != GL_NO_ERROR )
        {
            so_std::string_t es = std::to_string(err) ;
            so_log::global::error( "[wgl_context::get_gl_version] : get gl minor <"+es+">" ) ;
        }
    }
    version.major = major ;
    version.minor = minor ;

    return so_app::ok ;
}

//***********************************************************************
void_t wgl_context::clear_now( so_math::vec4f_t const & vec ) 
{
    so_gli::gl::glClearColor( vec.x(), vec.y(), vec.z(), vec.w() ) ;
    so_gli::gl::glClear( GL_COLOR_BUFFER_BIT ) ;
    
    GLenum const gler = so_gli::gl::glGetError() ;
    so_log::global::error( gler != GL_NO_ERROR, "[wgl_context::clear_now] : glClear" ) ;
}

//***********************************************************************
HWND wgl_context::get_win32_handle( iwindow_handle_ptr_t hnd_ptr ) 
{
    so_win32::win32_window_handle_ptr_t win32_handle = dynamic_cast<win32_window_handle_ptr_t>( hnd_ptr ) ;
    
    return win32_handle == nullptr ? NULL : win32_handle->get_handle() ;
}

//***********************************************************************
so_app::result wgl_context::create_the_context( gl_info_cref_t gli ) 
{
    typedef std::chrono::high_resolution_clock local_clock_t ;
    auto t1 = local_clock_t::now() ;

    HDC hdc = GetDC( _hwnd ) ;

    PIXELFORMATDESCRIPTOR pfd ;
    ZeroMemory( &pfd, sizeof(pfd) ) ;
    pfd.nSize = sizeof( pfd ) ;
    pfd.nVersion = 1 ;
    {
        DWORD double_buffer = gli.double_buffer == true ? PFD_DOUBLEBUFFER : DWORD(0) ;
        pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | double_buffer ;
    }
    pfd.iPixelType = PFD_TYPE_RGBA ;
    pfd.cColorBits = (BYTE)gli.color_bits ;
    pfd.cDepthBits = (BYTE)gli.depth_bits ;
    pfd.iLayerType = PFD_MAIN_PLANE ;
    int format = ChoosePixelFormat( hdc, &pfd ) ;

    SetPixelFormat( hdc, format, &pfd ) ;

    // create the temporary context.
    // we still need to determine if OpenGL 3.x is supported.
    _hrc = wglCreateContext( hdc ) ;

    if( so_log::global::error(_hrc == NULL, 
        "[wgl_context::create_the_context] : Failed to create the temporary context.") ) 
        return failed_gfx_context_creation ;

    if( so_log::global::error( wglMakeCurrent( hdc, _hrc ) == FALSE, 
        "[wgl_context::create_the_context] : wglMakeCurrent" ) )
    {
        return so_app::failed ;
    }

    // init the current "old" gl context. We need it in
    // order to determine the "new" 3.x+ context availability.
    so_gli::so_wgl::wgl::init( hdc ) ;

    HGLRC new_context ;
    if( so_gli::so_wgl::wgl::is_supported("WGL_ARB_create_context") )
    {
        const int attribList[] =
        {
            WGL_CONTEXT_MAJOR_VERSION_ARB, gli.version.major,
            WGL_CONTEXT_MINOR_VERSION_ARB, gli.version.minor,
#if defined( SNAKEOIL_DEBUG )
            WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB | WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
#else
            WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
#endif
            WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,// WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB, 
            0,        //End
        };

        // We create the new OpenGL 3.x+ context.
        //new_context = wglCreateContextAttribsARB(hdc, NULL, attribList) ;
        new_context = so_gli::so_wgl::wgl::wglCreateContextAttribs(hdc, NULL, attribList) ;
        if( new_context != NULL )
        {
            wglMakeCurrent( 0, 0 ) ;
            wglDeleteContext(_hrc) ;
            wglMakeCurrent(hdc, new_context) ;
            _hrc = new_context ;
        }

        // now using the new 3.x+ context.
        so_gli::gl::init() ;
    }

    //
    // Check ogl version against passed gl context info
    {
        gl_version version ;
        if( !success(this->get_gl_version( version )) ){
            //ups, more info? This should not happen
            wglMakeCurrent( 0, 0 ) ;
            return failed_gfx_context_creation ;
        }

        // check if the requested version is lower of equal to the queried version.
        if( gli.version.major > version.major ){
            // if major version can not be supported by the hardware, the program must fail.
            return invalid_gfx_api_version ;
        }
    }

    wglMakeCurrent( 0, 0 ) ;
    ReleaseDC( _hwnd, hdc ) ;
 
    {
        this_t::activate() ;
        
        so_log::global::warning( so_app::no_success( this_t::vsync( gli.vsync_enabled ) ), 
            "[wgl_context::create_the_context] : vsync setting failed." ) ;
        
        this_t::deactivate() ;
    }

    // timing end
    {
        size_t const milli = size_t(std::chrono::duration_cast<std::chrono::milliseconds>(
            local_clock_t::now() - t1).count()) ;

        so_log::global::status("[wgl_context] : created [" + std::to_string(milli) +" ms]" ) ;
    }

    return so_app::ok ;
}

//***********************************************************************
HGLRC wgl_context::create_the_shared( void_t ) 
{
    typedef std::chrono::high_resolution_clock local_clock_t ;
    auto t1 = local_clock_t::now() ;

    gl_version version ;

    so_log::global::error( so_app::no_success( this_t::activate() ), 
        "[wgl_context] : could not activate wgl context." ) ;

    if( so_app::no_success( this_t::get_gl_version( version ) ) )
    {
        so_log::global::error("[wgl_context::create_shared_context] : Requested version failed.") ;
        wglMakeCurrent( 0, 0 ) ;
        return NULL ;
    }
    
    const int attribList[] =
    {
        WGL_CONTEXT_MAJOR_VERSION_ARB, version.major,
        WGL_CONTEXT_MINOR_VERSION_ARB, version.minor,
        WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB,//WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
        WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,//WGL_CONTEXT_CORE_PROFILE_BIT_ARB, //WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
        0,        //End
    };
    
    HGLRC shared_context = so_gli::so_wgl::wgl::wglCreateContextAttribs( _hdc, _hrc, attribList ) ;

    /// don't forget to init
    so_log::global::error( so_gli::no_success( so_gli::gl::init() ), 
        "[wgl_context::create_the_shared] : gl init failed." ) ;

    this_t::deactivate() ;
        
    // timing end
    {
        size_t const milli = 
            size_t(std::chrono::duration_cast<std::chrono::milliseconds>( local_clock_t::now() - t1).count()) ;
        so_log::global::status("[wgl_context] : shared created [" + 
            std::to_string(milli) +" ms]" ) ;
    }

    return shared_context ;
}


