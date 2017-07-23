//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_APPLICATION_WINDOW_WIN32_WIN32_WINDOW_HANDLE_H_
#define _SNAKEOIL_APPLICATION_WINDOW_WIN32_WIN32_WINDOW_HANDLE_H_

#include "../iwindow_handle.h"

#include <windows.h>

namespace so_app
{
    namespace so_win32
    {
        class SNAKEOIL_APPLICATION_API win32_window_handle : public iwindow_handle
        {
            so_this_typedefs( win32_window_handle ) ;

        private:

            HWND _handle = NULL ;

        public:

            win32_window_handle( void_t ) ;
            win32_window_handle( HWND handle ) ;
            win32_window_handle( this_rref_t rhv ) ;
            virtual ~win32_window_handle( void_t ) ;

        public:

            this_ref_t operator = ( this_rref_t ) ;

        public:

            HWND set_handle( HWND ) ;
            HWND get_handle( void_t ) ;
            bool_t is_valid( void_t ) ;
        };
        so_typedefs( win32_window_handle, win32_window_handle ) ;
    }
}

#endif

