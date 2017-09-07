//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPX_SYSTEM_RENDER_SYSTEM_H_
#define _SNAKEOIL_GPX_SYSTEM_RENDER_SYSTEM_H_

#include "../protos.h"
#include "../typedefs.h"
#include "../api.h"
#include "../result.h"

#include "../window/window_id.h"
#include "../technique/technique_id.h"
#include "../technique/technique_state.h"

#include "../service/service.h"

#include "render_system_structs.h"

#include <snakeoil/thread/mutex.h>
#include <snakeoil/std/container/map.hpp>


namespace so_gpx
{
    class SNAKEOIL_GPX_API render_system
    {
        so_this_typedefs( render_system ) ;

    private:

        struct schedule_data
        {
            so_gpx::technique_schedule_goal ts ;
            technique_ptr_t tptr ;
            so_gpx::schedule_instance_t si ;
        };
        so_typedef( schedule_data ) ;
        so_typedefs( so_std::vector<schedule_data_t>, schedule_datas ) ;

        struct shared_object
        {
            so_this_typedefs( shared_object ) ;

            shared_object( void_t ) {}
            shared_object( this_cref_t ) = delete ;
            shared_object( this_rref_t rhv )
            {
                sync_00 = rhv.sync_00 ;
                sync_01 = rhv.sync_01 ;
                shutdown = rhv.shutdown ;
                read_buffer_id = rhv.read_buffer_id ;
                scheduled[0] = std::move( rhv.scheduled[0] ) ;
                scheduled[ 1 ] = std::move( rhv.scheduled[ 1 ] ) ;
                so_move_member_ptr( service_ptr, rhv ) ;
                so_move_member_ptr( gpu_mgr_ptr, rhv ) ;
            }
            ~shared_object( void_t ){}

            so_thread::mutex_t mtx_00 ;
            so_thread::condition_variable_t cv_00 ;
            bool_t sync_00 = false ;

            so_thread::mutex_t mtx_01 ;
            so_thread::condition_variable_t cv_01 ;
            bool_t sync_01 = false ;

            so_thread::mutex_t mtx_sd ;
            so_thread::condition_variable_t cv_shutdown ;
            bool_t shutdown = false ;
            bool_t shutdown_ok = false ;

            so_thread::mutex_t mtx_scheduled ;
            size_t read_buffer_id = 0 ;
            schedule_datas_t scheduled[2] ;

            void_t sync_00_wait( void_t ) ;
            void_t sync_00_send( void_t ) ;
            void_t sync_01_wait( void_t ) ;
            void_t sync_01_send( void_t ) ;

            void_t send_shutdown( void_t ) ;
            void_t wait_for_shutdown( void_t ) ;
            void_t send_shutdown_ok( void_t ) ;

            void_t flip_scheduled( void_t ) ;
            schedule_datas_ref_t read_buffer( void_t ) ;
            schedule_datas_ref_t write_buffer( void_t ) ;

            so_gpx::service_ptr_t service_ptr = nullptr ;
            so_gpu::gpu_manager_ptr_t gpu_mgr_ptr = nullptr ;
        };
        so_typedef( shared_object ) ;

        struct window_info
        {
            so_this_typedefs( window_info ) ;

            window_info( void_t ) {}
            window_info( this_cref_t rhv )
            {
                name = rhv.name ;
                wnd_ptr = rhv.wnd_ptr ;
                shared_ptr = rhv.shared_ptr ;
            }

            window_info( this_rref_t rhv ){
                so_move_member_ptr( wnd_ptr, rhv ) ;
                so_move_member_ptr( shared_ptr, rhv ) ;
                name = std::move( rhv.name ) ;
            }
            ~window_info( void_t ) {}

            so_std::string_t name ;
            so_gpx::iwindow_ptr_t wnd_ptr ;
            shared_object_ptr_t shared_ptr ;
        };
        so_typedef( window_info ) ;
        so_typedefs( so_std::vector< window_info >, window_infos ) ;

    private:
        
        struct technique_data
        {
            so_gpx::technique_ptr_t tptr ;
        };
        so_typedef( technique_data ) ;
        typedef so_std::map< technique_id_t, technique_data > __techniques_t ;
        so_typedefs( __techniques_t, techniques ) ;

    private:

        window_infos_t _wis ;
        
        so_thread::mutex_t _mtx_tecs ;
        techniques_t _tecs ;
        technique_id_t _cur_id = technique_id_t(0) ;

        /// the global service
        so_gpx::service_ptr_t _service_ptr = nullptr ;

    public:

        render_system( void_t ) ;
        render_system( this_rref_t ) ;
        render_system( this_cref_t ) = delete ;
        ~render_system( void_t ) ;

    public:

        static this_ptr_t create( so_memory::purpose_cref_t ) ;
        static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
        static void_t destroy( this_ptr_t ) ;

    public:

        /// first window registered is considered the main window
        bool_t register_window( so_std::string_cref_t, so_gpx::iwindow_ptr_t ) ;
        so_gpx::window_id_t get_window_id( so_std::string_cref_t ) ;

        /// waits until all render threads are shut down
        so_gpx::result shutdown( void_t ) ;

    public: //service api

        /// returns the global service object
        so_gpx::iglobal_service_ptr_t get_service( void_t ) ;

        /// returns the window specific service object
        so_gpx::ilocal_service_ptr_t get_service( window_id_t ) ;

    public:

        so_gpx::technique_id_t register_technique( so_gpx::iplug_factory_ptr_t ) ;
        bool_t unregister_technique( so_gpx::technique_id_t ) ;

    public:
        
        /// schedule for main window
        so_gpx::result schedule( so_gpx::technique_id_t ) ;
        so_gpx::result schedule( so_gpx::technique_id_t, so_gpx::window_id_t ) ;
        so_gpx::result schedule( so_gpx::technique_id_t, so_gpx::window_id_t, so_gpx::schedule_instance_cref_t ) ;
        so_gpx::result schedule_for_init( so_gpx::technique_id_t ) ;
        so_gpx::result schedule_for_init( so_gpx::technique_id_t, so_gpx::window_id_t ) ;
        so_gpx::result schedule_for_release( so_gpx::technique_id_t ) ;
        so_gpx::result schedule_for_release( so_gpx::technique_id_t, so_gpx::window_id_t ) ;

        so_gpx::result schedule_for_init_and_wait( so_gpx::technique_id_t ) ;
        so_gpx::result wait_for_init( so_gpx::technique_id_t ) ;

    private:

        so_gpx::result schedule_for( so_gpx::technique_id_t, so_gpx::window_id_t, 
            so_gpx::technique_schedule_goal ) ;

        so_gpx::result schedule_for( so_gpx::technique_id_t, so_gpx::window_id_t,
            so_gpx::technique_schedule_goal, so_gpx::schedule_instance_cref_t ) ;
        
    public: // update thread

        void_t update( void_t ) ;
        void_t render_begin( void_t ) ;
        void_t render_end( void_t ) ;

    private: // sync points

        void_t send_sync_00( void_t ) ;
        void_t wait_for_sync_01( void_t ) ;
        void_t update_gpu_techniques( void_t ) ;
    };
    so_typedef( render_system ) ;
}

#endif