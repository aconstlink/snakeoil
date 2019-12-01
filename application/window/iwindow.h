//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_APPLICATION_WINDOW_IWINDOW_H_
#define _SNAKEOIL_APPLICATION_WINDOW_IWINDOW_H_

#include "../api.h"
#include "../result.h"
#include "../typedefs.h"
#include "../protos.h"

#include "window_message.h"
#include "../event/toggle_window.h"

namespace so_app
{
    class SNAKEOIL_APPLICATION_API iwindow
    {
    public:

        /// allows to register a listener for all kinds of window messages.
        virtual so_app::result subscribe( iwindow_message_listener_ptr_t ) = 0 ;
        virtual so_app::result unsubscribe( iwindow_message_listener_ptr_t ) = 0 ;

        virtual so_app::result destroy( void_t ) = 0 ;
        virtual iwindow_handle_ptr_t get_handle( void_t ) = 0 ;

        virtual so_std::string_cref_t get_name( void_t ) const = 0 ;

        virtual void_t send_close( void_t ) = 0 ;
        virtual void_t send_toggle( so_app::toggle_window_in_t ) = 0 ;
    };
    so_typedef( iwindow ) ;
    
}

#endif

