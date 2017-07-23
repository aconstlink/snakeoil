//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_APPLICATION_APPLICATION_APPLICATION_H_
#define _SNAKEOIL_APPLICATION_APPLICATION_APPLICATION_H_

#if defined( SNAKEOIL_WINDOW_SYSTEM_WIN32 )
#include "win32/win32_application.h"
#elif defined( SNAKEOIL_WINDOW_SYSTEM_XLIB )
#include "xlib/xlib_application.h"
#else
#error "no window system defined"
#endif

namespace so_app
{
#if defined( SNAKEOIL_WINDOW_SYSTEM_WIN32 )
    so_typedefs( so_win32::win32_application, application ) ;
#elif defined(SNAKEOIL_WINDOW_SYSTEM_XLIB)
    so_typedefs( so_xlib::xlib_application, application ) ;
#endif    
}

#endif

