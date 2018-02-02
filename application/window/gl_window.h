//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#if defined( SNAKEOIL_WINDOW_SYSTEM_WIN32 ) 
#if defined( SNAKEOIL_TARGET_GRAPHICS_OPENGL )
#include "wgl/wgl_window.h"
#else
#include "null/null_window.h"
#endif
#elif defined( SNAKEOIL_WINDOW_SYSTEM_XLIB )
#include "glx/glx_window.h"
#else
#error "No Window system specified."
#endif

namespace so_app
{
#if defined( SNAKEOIL_WINDOW_SYSTEM_WIN32 )
    
#if defined( SNAKEOIL_TARGET_GRAPHICS_OPENGL )
    so_typedefs( so_win32::wgl_window, gl_window ) ;
#else
    so_typedefs( so_null::null_window, gl_window ) ;
#endif

#elif defined( SNAKEOIL_WINDOW_SYSTEM_XLIB )
    so_typedefs( so_xlib::glx_window, gl_window ) ;
#endif
}