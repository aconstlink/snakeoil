//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_APPX_MICRO_IMICRO_APP_H_
#define _SNAKEOIL_APPX_MICRO_IMICRO_APP_H_

#include "../system/init_data.h"
#include "../system/update_data.h"
#include "../system/render_data.h"
#include "../system/event_data.h"

#include "../api.h"
#include "../typedefs.h"
#include "../result.h"
#include "../protos.h"

namespace so_appx
{
    class SNAKEOIL_APPX_API iapp
    {

    public: // initialize

        /// called once on initialization state
        /// this function also introduces the micro context to the app
        /// is called once in the app's life time.
        virtual so_appx::result on_initialize( so_appx::iappx_context_ptr_t, init_data_cref_t ) = 0 ;
        /// called repetitively after on_initialize was called
        virtual so_appx::result has_initialized( void_t ) = 0 ;

    public: // run-time
        
        /// informs the app about any window state changes
        /// the state informer is passed along with the window id the app can
        /// check if it is interested in it. The function will only be called
        /// if actual window state changes occurred.
        virtual so_appx::result on_event( so_appx::window_event_data_cref_t ) = 0 ;

        /// the apps main loop. Is to be used for logic update only.
        virtual so_appx::result on_update( update_data_cref_t ) = 0 ;

        /// the app's render callback. Is to be used only for rendering.
        virtual so_appx::result on_render( render_data_cref_t ) = 0 ;

    public:

        /// will be called if the app is brought back from deactive state.
        /// can be called multiple times during the app's life time.
        virtual so_appx::result on_activate( void_t ) = 0 ;

        /// will ask multiple times if the activation process has finished.
        virtual so_appx::result has_activated( void_t ) = 0 ;

        /// will be called if the app is moving to the background or
        /// will be unavailable in any other way.
        /// can be called multiple times during the app's life time.
        virtual so_appx::result on_deactivate( void_t ) = 0 ;

        /// will ask multiple times if the deactivation process has finished.
        virtual so_appx::result has_deactivated( void_t ) = 0 ;

    public: // shutdown 

        /// called once on shutdown state
        /// is called once in the app's life time.
        virtual so_appx::result on_shutdown( void_t ) = 0 ;
        /// called repetitively after on_shutdown was called 
        virtual so_appx::result has_shutdown( void_t ) = 0 ;

    public:

        /// called for final destruction of the object.
        /// @see on_shutdown for object de-initialization.
        /// only use this function for really final destruction.
        virtual so_appx::result destroy( void_t ) = 0 ;
    };
}
    
#endif

