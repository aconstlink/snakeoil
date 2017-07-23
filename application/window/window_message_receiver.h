//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_APPLICATION_WINDOW_WINDOW_MESSAGE_RECEIVER_H_
#define _SNAKEOIL_APPLICATION_WINDOW_WINDOW_MESSAGE_RECEIVER_H_

#include "../api.h"
#include "../protos.h"
#include "../result.h"
#include "../typedefs.h"

#include "iwindow_message_listener.h"

#include <snakeoil/thread/container/double_buffered/item.hpp>
#include <snakeoil/thread/typedefs.h>

namespace so_app
{
    class window_message_receiver : public iwindow_message_listener
    {
        so_this_typedefs( window_message_receiver ) ;

        struct state_vector
        {
            so_app::resize_message resize_msg ;
            bool_t resize_changed = false ;

            so_app::show_message show_msg ;
            bool_t show_changed = false ;

            so_app::close_message close_msg ;
            bool_t close_changed = false ;

            so_app::screen_dpi_message dpi_msg ;
            bool_t dpi_msg_changed = false ;

            so_app::screen_size_message msize_msg ;
            bool_t msize_msg_changed = false ;
        };

        so_typedef( state_vector ) ;
        typedef so_thread::so_double_buffered::item< state_vector > state_vector_db_t ;
        typedef so_thread::so_double_buffered::item< bool_t > change_state_t ;

    private:

        /// lock the states vector
        so_thread::mutex_t _mtx ;
        state_vector_db_t _states ;

        change_state_t _has_any_change ;

    public:

        static SNAKEOIL_APPLICATION_API this_ptr_t create( std::string const & purpose ) ;
        static SNAKEOIL_APPLICATION_API this_ptr_t create( this_rref_t, std::string const & purpose ) ;
        static SNAKEOIL_APPLICATION_API void_t destroy( this_ptr_t ) ;

    private:

        /// non-copyable
        window_message_receiver( this_cref_t ) {}

    public:

        window_message_receiver( void_t ) ;
        window_message_receiver( this_rref_t ) ;
        virtual ~window_message_receiver( void_t ) ;

        /// swap the read/write buffers.
        SNAKEOIL_APPLICATION_API void_t swap( void_t ) ;

        SNAKEOIL_APPLICATION_API bool_t has_any_change( void_t ) ;

    private:

        /// sets all changes flags to false for
        /// the current write buffer.
        void_t reset_change_flags( void_t ) ;

        void_t set_changed( void_t ) ;

    public: // listener interface

        virtual void_t on_screen( screen_dpi_message_cref_t ) ;
        virtual void_t on_screen( screen_size_message_cref_t ) ;

        virtual void_t on_resize( resize_message_cref_t ) ;
        virtual void_t on_visible( show_message_cref_t ) ;
        virtual void_t on_close( close_message_cref_t ) ;

        virtual bool_t has_screen_changed( screen_dpi_message_ref_t ) ;
        virtual bool_t has_screen_changed( screen_size_message_ref_t ) ;

        virtual bool_t has_resize_changed( resize_message_ref_t ) ;
        virtual bool_t has_visible_changed( show_message_ref_t ) ;
        virtual bool_t has_close_changed( close_message_ref_t ) ;

        virtual void_t destroy( void_t ) ;
    };
}
    
#endif

