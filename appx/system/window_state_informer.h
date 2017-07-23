//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_APPX_APP_WINDOW_STATE_INFORMER_H_
#define _SNAKEOIL_APPX_APP_WINDOW_STATE_INFORMER_H_

#include "iwindow_state_informer.h"

#include <snakeoil/application/protos.h>

namespace so_appx
{
    class window_state_informer : public iwindow_state_informer
    {
        so_this_typedefs( window_state_informer ) ;

        struct states
        {
            so_app::resize_message resize_msg ;
            bool_t resize_changed = false ;

            so_app::show_message show_msg ;
            bool_t show_changed = false ;

            so_app::screen_dpi_message dpi_msg ;
            bool_t dpi_msg_changed = false ;

            so_app::screen_size_message msize_msg ;
            bool_t msize_msg_changed = false ;
        };
        so_typedef( states ) ;

    private:

        states _states ;

    public:

        window_state_informer( void_t ) ;
        window_state_informer( so_app::iwindow_message_listener_ptr_t ) ;
        window_state_informer( this_rref_t ) ;
        window_state_informer( this_cref_t ) ;
        virtual ~window_state_informer( void_t ) ;

    public:
        
        this_ref_t operator = ( this_rref_t ) ;
        
    public:

        virtual bool_t has_screen_message( so_app::screen_dpi_message_ref_t ) const ;
        virtual bool_t has_screen_message( so_app::screen_size_message_ref_t ) const ;
        virtual bool_t has_resize_message( so_app::resize_message_ref_t ) const ;
        virtual bool_t has_show_message( so_app::show_message_ref_t ) const ;

    public:

        /// reset all change flags.
        void_t reset( bool_t b = false ) ;

    };
}

#endif

