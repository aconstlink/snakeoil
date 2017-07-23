//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_APPLICATION_RENDER_WINDOW_IRENDER_CONTEXT_H_
#define _SNAKEOIL_APPLICATION_RENDER_WINDOW_IRENDER_CONTEXT_H_

#include "../protos.h"
#include "../result.h"
#include "../typedefs.h"
#include "../api.h"

#include "render_context_info.h"

namespace so_app
{
    class SNAKEOIL_APPLICATION_API irender_context
    {
    public:
        
        virtual so_app::result activate( void_t ) = 0 ;
        virtual so_app::result deactivate( void_t ) = 0 ;

        /// enables or disables the vsync
        virtual so_app::result vsync( bool_t on_off ) = 0 ;

        /// swaps the buffers. May wait for vsync if enabled.
        virtual so_app::result swap( void_t ) = 0 ;

        /// create a shared context that allows you to perform
        /// multi-threaded rendering operations.
        virtual irender_context_ptr_t create_shared_context( void_t ) = 0 ;
    };
}

#endif

