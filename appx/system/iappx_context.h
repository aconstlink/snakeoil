//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_APPX_MICRO_IMICRO_CONTEXT_H_
#define _SNAKEOIL_APPX_MICRO_IMICRO_CONTEXT_H_

#include "../typedefs.h"
#include "../protos.h"
#include "../result.h"
#include "../window/window_state_setter.h"

#include <snakeoil/imex/system/isystem.h>
#include <snakeoil/gpx/protos.h>
#include <snakeoil/io/protos.h>
#include <snakeoil/thread/protos.h>
#include <snakeoil/device/protos.h>

namespace so_appx
{
    class iappx_context
    {
        so_this_typedefs( iappx_context ) ;

    public:

        virtual so_gpx::render_system_ptr_t render_system( void_t ) = 0 ;
        
        /// request the imex system for im/export stuff
        virtual so_imex::isystem_ptr_t  imexsys( void_t ) = 0 ;

        virtual so_appx::window_state_setter_t wnd_state_setter( void_t ) = 0 ;

    };
}

#endif

