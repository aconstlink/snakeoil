//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_APPLICATION_WINDOW_WINDOW_MESSAGE_H_
#define _SNAKEOIL_APPLICATION_WINDOW_WINDOW_MESSAGE_H_

#include "../typedefs.h"

namespace so_app
{
    struct show_message
    {
        bool_t show ;
    };
    so_typedef( show_message ) ;

    struct resize_message
    {
        size_t x ;
        size_t y ;
        size_t w ;
        size_t h ;
    };
    so_typedef( resize_message ) ;

    struct close_message
    {
        bool_t close ;
    };
    so_typedef( close_message ) ;

    struct screen_dpi_message
    {
        uint_t dpi_x ;
        uint_t dpi_y ;
    };
    so_typedef( screen_dpi_message ) ;

    struct screen_size_message
    {
        uint_t width ;
        uint_t height ;
    };
    so_typedef( screen_size_message ) ;
}

#endif

