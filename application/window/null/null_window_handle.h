//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_APPLICATION_WINDOW_NULL_NULL_WINDOW_HANDLE_H_
#define _SNAKEOIL_APPLICATION_WINDOW_NULL_NULL_WINDOW_HANDLE_H_

#include "../iwindow_handle.h"

namespace so_app
{
    namespace so_null
    {
        class SNAKEOIL_APPLICATION_API null_window_handle : public iwindow_handle
        {
            so_this_typedefs( null_window_handle ) ;

        public:

            null_window_handle( void_t ) ;
            null_window_handle( this_rref_t rhv ) ;
            null_window_handle( this_cref_t rhv ) = delete ;
            virtual ~null_window_handle( void_t ) ;

        public:

            this_ref_t operator = ( this_rref_t ) ;

        };
        so_typedef( null_window_handle ) ;
    }
}

#endif