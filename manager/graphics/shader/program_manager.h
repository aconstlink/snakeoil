//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_MANAGER_GRAPHICS_SHADER_PROGRAM_MANAGER_H_
#define _SNAKEOIL_MANAGER_GRAPHICS_SHADER_PROGRAM_MANAGER_H_

#include "iprogram_manager.h"

#include <snakeoil/gpx/protos.h>
#include <snakeoil/gpx/event/event.h>

#include <snakeoil/thread/typedefs.h>

namespace so_manager
{
    namespace so_graphics
    {
        class SNAKEOIL_MANAGER_API program_manager : public iprogram_manager
        {
            so_this_typedefs( program_manager ) ;

        private:

            struct program_data
            {
                so_gpu::program_ptr_t program_ptr = nullptr ;
                so_gpx::event_ptr_t link_status = nullptr ;
            };
            
            typedef so_std::map< so_manager::key_t, program_data > key_to_program_t ;

            struct config_data
            {
                so_gpu::config_ptr_t config_ptr = nullptr ;
            };
            
            typedef so_std::map< so_manager::key_t, config_data > key_to_config_t ;

        private:

            so_thread::mutex_t _mtx ;
            key_to_program_t _key_to_program ;
            key_to_program_t _key_to_program_shutdown ;

            key_to_config_t _key_to_config ;
            key_to_config_t _key_to_config_shutdown ;

            /// used for signaling the manager's shutdown
            so_gpx::event _shutdown_ready ;

            bool_t _shutdown_called = false ;

            so_thread::mutex_t _mtx_unique_counter ;
            size_t _unique_counter = 0 ;

        private: // external 

            so_manager::so_graphics::shader_manager_ptr_t _shader_mgr_ptr = nullptr;
            so_gpx::iuser_system_ptr_t _gpxs_ptr = nullptr ;

        public:

            program_manager( so_manager::so_graphics::shader_manager_ptr_t ptr, 
                so_gpx::iuser_system_ptr_t ) ;

            program_manager( this_rref_t ) ;
            virtual ~program_manager( void_t ) ;

        public: // static create/destroy

            static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
            static void_t destroy( this_ptr_t ) ;

        public: // program

            virtual so_manager::result compose_program( 
                so_manager::key_cref_t, shader_keys_cref_t shk ) ;
            
            virtual so_manager::result link_program( 
                so_manager::key_cref_t key_in ) ;
            
            virtual so_manager::result compose_and_link_program( 
                so_manager::key_cref_t, shader_keys_cref_t shk ) ;

            virtual so_gpu::result get_link_status( 
                so_manager::key_cref_t ) ;

            virtual so_gpu::result wait_for_link( 
                so_manager::key_cref_t ) ;

            virtual so_gpu::program_ptr_t get_program( 
                so_manager::key_cref_t ) ;

        public: // config

            virtual so_manager::key_t compose_unique_config_key( so_manager::key_cref_t ) ;

            virtual so_manager::result compose_config( 
                so_manager::key_cref_t config_key_in, so_manager::key_cref_t prog_key_in ) ;
            
            virtual so_gpu::config_ptr_t get_config( 
                so_manager::key_cref_t ) ;

        public: // shutdown

            virtual so_manager::result destroy( void_t ) ;
            virtual so_manager::result shutdown( void_t ) ;
            virtual so_manager::result has_shutdown( void_t ) ;

        };
        so_typedef( program_manager ) ;
    }
}

#endif
