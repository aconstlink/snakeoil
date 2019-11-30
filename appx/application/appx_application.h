//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "../protos.h"
#include "../typedefs.h"
#include "../result.h"
#include "../api.h"
#include "../window/window_event_manager.h"

#include <snakeoil/application/protos.h>
#include <snakeoil/application/application/iapplication.h>
#include <snakeoil/application/window/gl_info.h>
#include <snakeoil/application/window/window_info.h>

#include <snakeoil/audiox/protos.h>
#include <snakeoil/gpx/protos.h>

#include <snakeoil/thread/mutex.h>
#include <snakeoil/std/container/vector.hpp>

namespace so_appx
{
    class SNAKEOIL_APPX_API appx_application : public so_app::iapplication
    {
        so_this_typedefs( appx_application ) ;

    private:

        struct rwindow_data
        {
            size_t appx_wid ;
            so_app::iwindow_ptr_t window_ptr ;
            so_app::window_message_receiver_ptr_t receiver_ptr ;
        };

        typedef so_std::vector< rwindow_data > rwindows_t ;

        window_event_manager_ptr_t _wnd_evt_mgr = nullptr ;

    private:

        /// data structure for sharing data between
        /// this application and the update thread.
        struct shared_data
        {
            so_this_typedefs( shared_data ) ;

            so_thread::mutex_t mtx ;

            shared_data( void_t ) {}
            shared_data( this_rref_t rhv )
            {
                so_move_member_ptr( rsystem_ptr, rhv ) ;
            }
            
            so_gpx::render_system_ptr_t rsystem_ptr = nullptr ;
            so_audiox::audio_system_ptr_t asystem_ptr = nullptr ;

            bool_t shutdown_triggered = false ;

        private:

            shared_data( this_cref_t ){}
        };
        so_typedef( shared_data ) ;

    private:

        so_gpx::render_system_ptr_t _rs_ptr = nullptr ;
        so_audiox::audio_system_ptr_t _as_ptr = nullptr ;

        /// will save window specific data.
        rwindows_t _rwindows ;

        /// thread-to-thread shared data.
        shared_data_ptr_t _shared_data_ptr = nullptr ;

        /// handles all micro apps and allows to interact 
        /// with all registered micro apps.
        appx_system_ptr_t _appx_ptr = nullptr ;

        /// the window application will help to create and shutdown
        /// all windows properly. The window application is platform
        /// dependent. This is compile-time determined.
        so_app::window_application_ptr_t _win_app_ptr = nullptr ;

    private:

        /// non-copyable
        appx_application( this_cref_t ) {}

    public:

        appx_application( void_t ) ;
        appx_application( this_rref_t ) ;
        virtual ~appx_application( void_t ) ;

    public:

        static this_ptr_t create( std::string const & purpose ) ;
        static this_ptr_t create( this_rref_t, std::string const & purpose ) ;
        static void_t destroy( this_ptr_t ) ;

    public:

        /// register an app that will perform the user's logic and rendering.
        /// it is recommended to derive from appx_app instead of iappx_app.
        /// @see appx_app
        /// @see iappx_app
        so_appx::result register_app( so_appx::iapp_ptr_t ) ;

        /// creates an OpenGL render window.
        so_appx::result create_window( 
            so_app::gl_info_cref_t, so_app::window_info_cref_t ) ;

    public: // interface

        virtual so_app::result destroy( void_t ) ;
        
        /// overwritten in order to start the internal update thread.
        /// this way, the start of the thread can be deferred to this point
        /// which allows to omit certain synchronization.
        /// a derived call is required to implement exec_derived() in order 
        /// to do its execution work.
        virtual so_app::result exec( void_t ) final ;

    private:

        so_appx::result kill_window_spcific_data( void_t ) ;
        so_appx::result kill_window_application( void_t ) ;
        so_appx::result trigger_update_thread_shutdown( void_t ) ;
    };
    so_typedef( appx_application ) ;
}
