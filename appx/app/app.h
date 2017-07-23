//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_APPX_MICRO_APPX_APP_H_
#define _SNAKEOIL_APPX_MICRO_APPX_APP_H_

#include "iapp.h"

#include <snakeoil/imex/protos.h>
#include <snakeoil/device/protos.h>
#include <snakeoil/gpx/protos.h>
#include <snakeoil/io/protos.h>
#include <snakeoil/thread/protos.h>

namespace so_appx
{
    /// a concrete implementation of the micro application interface.
    /// it helps using the whole application api by providing the
    /// most important objects of the current application.
    class SNAKEOIL_APPX_API app : public iapp
    {
        so_this_typedefs( app ) ;

    private:

        /// contains all the important objects all apps require
        /// for running.
        so_appx::iappx_context_ptr_t _ctx_ptr = nullptr ;

    private:

        // can not copy
        app( this_cref_t ) {}

    public:

        app( void_t ) ;
        app( this_rref_t ) ;
        virtual ~app( void_t ) ;

    public:

        so_appx::iappx_context_ptr_t ctx( void_t ) ;
        so_device::idevice_system_ptr_t dev( void_t ) ;
        so_imex::isystem_ptr_t imexsys( void_t ) ;
        so_gpx::render_system_ptr_t rsys( void_t ) ;

    private: // interface 

        /// intercepting the virtual function in order to store the context.
        /// this function will call on_initialize without any argument. The
        /// context will be present to the derived class/object.
        virtual so_appx::result on_initialize( 
            so_appx::iappx_context_ptr_t, so_appx::init_data_cref_t ) final ;

    public: // interface

        virtual so_appx::result on_initialize( so_appx::init_data_cref_t ) = 0 ;
        virtual so_appx::result has_initialized( void_t ) = 0 ;
        virtual so_appx::result on_event( so_appx::window_event_data_cref_t ) = 0 ;
        virtual so_appx::result on_update( so_appx::update_data_cref_t ) = 0 ;
        virtual so_appx::result on_render( so_appx::render_data_cref_t ) = 0 ;
        virtual so_appx::result on_activate( void_t ) = 0 ;
        virtual so_appx::result has_activated( void_t ) = 0 ;
        virtual so_appx::result on_deactivate( void_t ) = 0 ;
        virtual so_appx::result has_deactivated( void_t ) = 0 ;
        virtual so_appx::result on_shutdown( void_t ) = 0 ;
        virtual so_appx::result has_shutdown( void_t ) = 0 ;
        virtual so_appx::result destroy( void_t ) = 0 ;
    };
}
    
#endif

