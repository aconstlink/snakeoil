

#pragma once

#include "../api.h"
#include "../typedefs.h"
#include "../protos.h"

namespace so_appx
{
    class SNAKEOIL_APPX_API window_state_setter
    {
        so_this_typedefs( window_state_setter ) ;
        friend class window_event_manager ;

    private:

        window_event_manager_str_t _mgr ;

    public:

        window_state_setter( window_event_manager_str_t ptr ) ;
        window_state_setter( this_rref_t rhv ) ;
        ~window_state_setter( void_t ) ;

    public:

        void_t toggle_vsync( size_t const wid ) ;

        void_t toggle_fullscreen( size_t const wid ) ;

    };
    so_typedef( window_state_setter ) ;
}