//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_APPLICATION_RENDER_WINDOW_GL_WINDOW_H_
#define _SNAKEOIL_APPLICATION_RENDER_WINDOW_GL_WINDOW_H_

#if defined( SNAKEOIL_WINDOW_SYSTEM_WIN32 )
#include "wgl/wgl_window.h"
#elif defined( SNAKEOIL_WINDOW_SYSTEM_XLIB )
#include "glx/glx_window.h"
#else
#error "No Window system specified."
#endif

namespace so_app
{
#if defined( SNAKEOIL_WINDOW_SYSTEM_WIN32 )
    so_typedefs( so_win32::wgl_window, gl_window ) ;
#elif defined( SNAKEOIL_WINDOW_SYSTEM_XLIB )
    so_typedefs( so_xlib::glx_window, gl_window ) ;
#endif
}

#endif

