//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_APPLICATION_WINOW_NULL_NULL_WINDOW_H_
#define _SNAKEOIL_APPLICATION_WINOW_NULL_NULL_WINDOW_H_

#include "../iwindow.h"

#include "null_window_handle.h"

#include <snakeoil/gpx/window/iwindow.h>
#include <snakeoil/gpu/api/null/null_api.h>

namespace so_app
{
    namespace so_null
    {
        class SNAKEOIL_APPLICATION_API null_window : public iwindow, public so_gpx::iwindow
        {
            so_this_typedefs( null_window ) ;

        private:

            null_window_handle_ptr_t _handle = nullptr ;
            so_std::string_t _name = "null_window" ;

            so_gpu::so_null::null_api_ptr_t _api = nullptr ;

        public:

            null_window( void_t ) ;
            null_window( this_cref_t ) = delete ;
            null_window( this_rref_t ) ;
            virtual ~null_window( void_t ) ;

        public:

            static this_ptr_t create( so_memory::purpose_cref_t ) ;
            static void_t destroy( this_ptr_t ) ;

        public:

            virtual so_app::result subscribe( iwindow_message_listener_ptr_t ) ;
            virtual so_app::result unsubscribe( iwindow_message_listener_ptr_t ) ;

            virtual so_app::result destroy( void_t ) ;
            virtual iwindow_handle_ptr_t get_handle( void_t ) ;

            virtual so_std::string_cref_t get_name( void_t ) const ;
            virtual void_t send_close( void_t ) ;
            virtual void_t send_toggle_fullscreen( void_t ) {}
            virtual void_t send_toggle_vsync( void_t ) {}

        public: // gpu iwindow interface

            virtual void_t render_thread_begin( void_t ) ;
            virtual void_t render_thread_end( void_t ) ;
            virtual void_t end_frame( void_t ) ;
            virtual so_gpu::iapi_ptr_t get_api( void_t ) ;
            virtual so_gpu::api_type get_api_type( void_t ) ;
        };
        so_typedef( null_window ) ;
    }
}

#endif