//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_APPLICATION_APPLICATION_WINDOW_APPLICATION_H_
#define _SNAKEOIL_APPLICATION_APPLICATION_WINDOW_APPLICATION_H_

#include "iapplication.h"
#include "../window/window_message.h"

#include <snakeoil/gpx/protos.h>
#include <snakeoil/std/container/vector.hpp>
#include <snakeoil/thread/typedefs.h>

namespace so_app
{
    class SNAKEOIL_APPLICATION_API window_application : public iapplication
    {
        so_this_typedefs( window_application ) ;

        struct window_data
        {
            iwindow_ptr_t window_ptr ;
        };
        typedef so_std::vector< window_data > windows_t ;

    public:

        class application_state
        {
            so_this_typedefs( application_state ) ;

            friend class window_application ;

        public:

            application_state( void_t ) {}
            application_state( this_cref_t ) = delete ;
            application_state( this_rref_t rhv ) { _running = rhv._running ; }
            ~application_state( void_t ) {}

        public:

            bool_t is_running( void_t ) const 
            { 
                return _running ; 
            }
            
            void_t update_finised( void_t )
            {
                {
                    so_thread::lock_guard_t lk( _mtx ) ;
                    update_ended = true ;
                }
                _cv.notify_all() ;
            }

        private:

            mutable so_thread::mutex_t _mtx ;
            mutable so_thread::condition_variable_t _cv ;
            bool_t update_ended = true ;
            
            bool_t _running = true ;

        private:

            void_t stop( void_t ) { _running = false ; }

            void_t update_begin( void_t )
            {
                so_thread::lock_t lk( _mtx ) ;
                update_ended = false ;
            }

            void_t wait_for_finish( void_t ) const
            {
                so_thread::lock_t lk( _mtx ) ;
                while( so_core::is_not( update_ended ) ) _cv.wait( lk ) ;
            }
        };
        so_typedef( application_state ) ;
        
    private:

        /// all registered windows
        windows_t _windows ;

        /// this application provides one update thread
        /// where all user data is executed.
        so_thread::thread_t _update_thread ;
       
        /// allows to inform the update thread owner how the application is
        /// it may also be possible to reveice window events and discover the
        /// same window events
        application_state_ptr_t _app_state_ptr = nullptr  ;

    private:

        /// non-copyable
        window_application( this_cref_t ) {}

    public:

        window_application( void_t ) ;
        window_application( this_rref_t ) ;
        virtual ~window_application( void_t ) ;
        
        /// register a non rendering window.
        so_app::result register_window( so_app::iwindow_ptr_t wptr ) ;

        
    public: // interface

        virtual so_app::result destroy( void_t ) = 0 ;
        
        /// overwritten in order to start the internal update thread.
        /// this way, the start of the thread can be deferred to this point
        /// which allows to omit certain synchronization.
        /// a derived call is required to implement exec_derived() in order 
        /// to do its execution work.
        virtual so_app::result exec( void_t ) final ;

        /// allows to specify the update function. The function will
        /// be executed in parallel.
        so_app::result exec( std::function< void_t( application_state_ptr_t ) > ) ;

    protected:

        virtual so_app::result exec_derived( void_t ) = 0 ;

    protected:

        /// designed to be called from a derived class object.
        so_app::result shutdown_and_kill_all_windows( void_t ) ;

    private:

        so_app::result shutdown_update_thread( void_t ) ;
    };
}
    
#endif

