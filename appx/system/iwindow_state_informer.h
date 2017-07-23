//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_APPX_APP_IWINDOW_STATE_INFORMER_H_
#define _SNAKEOIL_APPX_APP_IWINDOW_STATE_INFORMER_H_

#include "../typedefs.h"
#include "../protos.h"
#include "../result.h"
#include "../api.h"

#include <snakeoil/application/window/window_message.h>

namespace so_appx
{
    class SNAKEOIL_APPX_API iwindow_state_informer
    {
    public:

        virtual bool_t has_screen_message( so_app::screen_dpi_message_ref_t ) const = 0 ;
        virtual bool_t has_screen_message( so_app::screen_size_message_ref_t ) const = 0 ;
        virtual bool_t has_resize_message( so_app::resize_message_ref_t ) const = 0 ;
        virtual bool_t has_show_message( so_app::show_message_ref_t ) const = 0 ;
    };
}

#endif

