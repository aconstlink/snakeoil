//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_APPLICATION_WINDOW_WIN32_WIN32_INFO_H_
#define _SNAKEOIL_APPLICATION_WINDOW_WIN32_WIN32_INFO_H_

#include "../../typedefs.h"

namespace so_app
{
    namespace so_win32 
    {
        /// @todo can we delete it. it should have been replaced
        /// by the window_info sucture.
        struct win32_window_info
        {
            int_t x = 0 ;
            int_t y = 0 ;
            int_t w = 100 ;
            int_t h = 100 ;

            bool_t fullscreen = false ;
            bool_t show_cursor = true ;
            std::string window_name = std::string("window name");

        };
        so_typedefs( win32_window_info, win32_window_info ) ;
    }
}

#endif

