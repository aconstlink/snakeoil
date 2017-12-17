//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_APPLICATION_WINDOW_WINDOW_INFO_H_
#define _SNAKEOIL_APPLICATION_WINDOW_WINDOW_INFO_H_

#include "../typedefs.h"

namespace so_app
{    
    struct window_info
    {
        int_t x = 0 ;
        int_t y = 0 ;
        int_t w = 100 ;
        int_t h = 100 ;

        bool_t borderless = false ;
        bool_t fullscreen = false ;
        bool_t show_cursor = true ;
        std::string window_name = std::string("window name");

        bool_t show = true ;

        bool_t dpi_aware = false ;
    };
    so_typedef( window_info ) ;    
}

#endif

