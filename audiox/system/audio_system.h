//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_AUDIOX_SYSTEM_AUDIO_SYSTEM_H_
#define _SNAKEOIL_AUDIOX_SYSTEM_AUDIO_SYSTEM_H_

#include "../typedefs.h"
#include "../protos.h"
#include "../api.h"
#include "../result.h"

#include "../technique/api_id.h"
#include "../technique/technique_id.h"
#include "../technique/technique_state.h"

#include "../service/service.h"

#include <snakeoil/audio/protos.h>
#include <snakeoil/thread/mutex.h>
#include <snakeoil/std/string/string.hpp>
#include <snakeoil/std/container/map.hpp>
#include <snakeoil/std/container/vector.hpp>

namespace so_audiox
{
    class SNAKEOIL_AUDIOX_API audio_system
    {
        so_this_typedefs( audio_system ) ;

    private:
        
        struct schedule_data
        {
            technique_state ts ;
            audio_technique_ptr_t tptr ;
        };
        so_typedef( schedule_data ) ;
        so_typedefs( so_std::vector<schedule_data_t>, schedule_datas ) ;

        struct shared_object
        {
            so_this_typedefs( shared_object ) ;

        public:

            shared_object( void_t ) {}
            shared_object( this_cref_t ) = delete ;
            shared_object( this_rref_t rhv ) 
            {
                has_init = rhv.has_init ;
                running = rhv.running ;
                t = std::move(t) ;
                so_move_member_ptr( aptr, rhv ) ;
            }
            virtual ~shared_object( void_t ) {}

        public:

            std::thread t ;

            so_thread::condition_variable_t cv_init ;
            so_thread::mutex_t mtx_init ;
            bool_t has_init = false ;

            so_thread::condition_variable_t cv_run ;
            so_thread::mutex_t mtx_run ;
            bool_t running = true ;

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

            so_audio::iapi_ptr_t aptr ;
            so_audiox::service_ptr_t service_ptr = nullptr ;

        };
        so_typedef( shared_object ) ;
        so_typedefs( so_std::vector< shared_object_ptr_t >, apis ) ;

    private:

        struct technique_data
        {
            so_audiox::audio_technique_ptr_t tptr ;
        };
        so_typedef( technique_data ) ;

        typedef so_std::map< technique_id_t, technique_data_t > __techniques_t ;
        so_typedefs( __techniques_t, techniques ) ;

    private: // variables

        apis_t _apis ;

        so_thread::mutex_t _mtx_tech ;
        technique_id_t _cur_tech_id = technique_id_t(-1) ;
        techniques_t _tecs ;

        /// the global service
        so_audiox::service_ptr_t _service_ptr = nullptr ;

    public:

        audio_system( void_t ) ;
        audio_system( this_cref_t ) = delete ;
        audio_system( this_rref_t ) ;
        ~audio_system( void_t ) ;

    public:

        static this_ptr_t create( so_memory::purpose_cref_t ) ;
        static void_t destroy( this_ptr_t ) ;

    public: // serivce

        /// returns the global service object
        so_audiox::service_ptr_t get_service( void_t ) ;

    public: // update thread

        void_t update( void_t ) ;
        void_t render_begin( void_t ) ;
        void_t render_end( void_t ) ;

    public:

        technique_id_t register_technique( so_audiox::iplug_factory_ptr_t ) ;
        so_audiox::result schedule_for_init( so_audiox::technique_id_t ) ;
        so_audiox::result schedule_for_release( so_audiox::technique_id_t ) ;

        so_audiox::result schedule( technique_id_t ) ;

        so_audiox::result schedule_for( technique_state, technique_id_t ) ;

        so_audiox::result schedule_for_init_and_wait( so_audiox::technique_id_t ) ;
        so_audiox::result wait_for_init( so_audiox::technique_id_t ) ;

    private: // sync points

        void_t send_sync_00( void_t ) ;
        void_t wait_for_sync_01( void_t ) ;
    

    };
    so_typedef( audio_system ) ;
}

#endif