//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_APPX_MICRO_APPX_SYSTEM_H_
#define _SNAKEOIL_APPX_MICRO_APPX_SYSTEM_H_

#include "../typedefs.h"
#include "../protos.h"
#include "../result.h"

#include "window_state_informer.h"
#include "appx_context.h"
#include "update_data.h"
#include "render_data.h"
#include "states.h"

#include <snakeoil/gpx/protos.h>
#include <snakeoil/device/protos.h>
#include <snakeoil/imex/system/isystem.h>
#include <snakeoil/io/protos.h>
#include <snakeoil/thread/protos.h>
#include <snakeoil/std/container/vector.hpp>

namespace so_appx
{    
    class appx_system
    {
        so_this_typedefs( appx_system ) ;

        /// internal storage for micro app management.
        struct __app_data
        {
            iapp_ptr_t app_ptr ;
            so_appx::appx_state state ;
        };

        typedef std::vector< __app_data > apps_t ;

    private:

        struct window_data
        {
            size_t id ;
            std::string name ;
            so_appx::window_state_informer state_informer ;
        };
        typedef std::vector< window_data > window_datas_t ;

    private:

        /// all the apps that are managed.
        apps_t _apps ;

        so_gpx::render_system_ptr_t _rsys_ptr = nullptr ;

        so_imex::system_ptr_t _imexs_ptr = nullptr ;
        
        so_appx::window_event_manager_str_t _wnd_evt_mgr = nullptr ;

    private: // window data

        size_t _window_id = 0 ;
        window_datas_t _window_datas ;
        bool_t _has_any_window_state_change = false ;

    private:

        /// the micro system context containing all
        /// important app data.
        appx_context * _ctx ;
        
    private:
        
        /// non-copyable
        appx_system( this_cref_t ) = delete ;
        
    public:

        appx_system( void_t ) ;
        appx_system( this_rref_t ) ;
        virtual ~appx_system( void_t ) ;

    public:

        static this_ptr_t create( void_t ) ;
        static void_t destroy( this_ptr_t ) ;

    public:

        so_appx::result register_app( iapp_ptr_t ) ;

        /// register a window and request a window id.
        size_t register_window( std::string const & name ) ;

        so_appx::result set_render_system( so_gpx::render_system_ptr_t ) ;
        so_appx::result set_window_event_manager( so_appx::window_event_manager_str_t ) ;

    public:

        so_appx::result update( size_t wid, so_app::iwindow_message_listener_ptr_t ) ;
        so_appx::result update( update_data_cref_t ) ;
        so_appx::result render( render_data_cref_t ) ;

        /// shutdown everything.
        /// 1. shuts down all apps
        /// if all apps are fully shutdown,
        /// 2. shuts down everything else
        /// This function is designed to be called multiple times.
        /// Each time the function is called, it checks the shutdown status 
        /// of all internal components.
        /// @return
        /// so_appx::ok if the shutdown has finished.
        /// so_appx::not ready if the shutdown has not finished.
        so_appx::result shutdown( void_t ) ;

    };    
}

#endif

