//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_APPLICATION_WINDOW_WIN32_WIN32_WINDOW_H_
#define _SNAKEOIL_APPLICATION_WINDOW_WIN32_WIN32_WINDOW_H_

#include "../iwindow.h"
#include "../window_info.h"

#include "win32_info.h"
#include "win32_window_handle.h"

#include <vector>

namespace so_app
{
    namespace so_win32
    {
        class SNAKEOIL_APPLICATION_API win32_window : public iwindow
        {
            so_this_typedefs( win32_window ) ;

            typedef so_std::vector< so_app::iwindow_listener_ptr_t > listeners_t ;
            typedef so_std::vector< so_app::iwindow_message_listener_ptr_t > msg_listeners_t ;

        private:

            win32_window_handle _handle ;
            
            msg_listeners_t _msg_listeners ;

            so_std::string _name ;

            bool_t _is_fullscreen = false ;

        public:

            win32_window( window_info const & ) ;
            win32_window( win32_window_handle_rref_t ) ;
            win32_window( this_rref_t rhv ) ;
            virtual ~win32_window( void_t ) ;

        public: // interface

            static this_ptr_t create( this_rref_t rhv ) ;
            static this_ptr_t create( this_rref_t rhv, so_memory::purpose_cref_t msg ) ;
            static void_t destroy( this_ptr_t rhv ) ;

            
            virtual so_app::result subscribe( iwindow_message_listener_ptr_t ) ;
            virtual so_app::result unsubscribe( iwindow_message_listener_ptr_t ) ;

            virtual so_app::result destroy( void_t ) ;
            virtual iwindow_handle_ptr_t get_handle( void_t ) ;
            
            virtual std::string const & get_name( void_t ) const ;

            virtual void_t send_close( void_t ) ;
            virtual void_t send_toggle_fullscreen( void_t ) ;
            virtual void_t send_toggle_vsync( void_t ) ;

        private:

            HWND create_window( window_info const & ) ;
            void_t destroy_window( void_t ) ;

            void_t remove_invalid_window_listeners( void_t ) ;

        private:

            void_t send_resize( HWND ) ;
            
            
        private: 

            void_t get_monitor_info( HWND, MONITORINFO & ) ;
            void_t send_screen_dpi( HWND ) ;
            void_t send_screen_dpi( HWND, uint_t dpix, uint_t dpiy ) ;
            void_t send_screen_size( HWND ) ;
            void_t send_screen_size( HWND, uint_t width, uint_t height ) ;

        protected: // virtual

            /// Here the real code is executed in order to do the callback actions.
            virtual LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam ) ;

        public:

            /// used to pass to windows for the callback. It is the wrapper that calls the real proc function.
            static LRESULT CALLBACK StaticWndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam ) ;
        };
        so_typedefs( win32_window, win32_window ) ;
    }
}
    
#endif

