//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_MANAGER_GRAPHICS_VARIABLE_VARIABLE_MANAGER_H_
#define _SNAKEOIL_MANAGER_GRAPHICS_VARIABLE_VARIABLE_MANAGER_H_

#include "ivariable_manager.h"

#include <snakeoil/gpx/protos.h>
#include <snakeoil/gpx/event/event.h>

#include <snakeoil/thread/mutex.h>

namespace so_manager
{
    namespace so_graphics
    {
        class SNAKEOIL_MANAGER_API variable_manager : public ivariable_manager
        {
            so_this_typedefs( variable_manager ) ;

        private:

            so_thread::mutex_t _mtx ;

            struct variable_set_data
            {
                so_gpu::variable_set_ptr_t var_set_ptr = nullptr ;
            };
            typedef so_std::map< so_manager::key_t, variable_set_data > key_to_varset_t ;

        private:

            key_to_varset_t _varsets ;
            key_to_varset_t _varsets_shutdown ;

            /// used for signaling the manager's shutdown
            so_gpx::event _shutdown_ready ;

            bool_t _shutdown_called = false ;

            so_gpx::iuser_system_ptr_t _gpxs_ptr = nullptr ;

        public:

            variable_manager( void_t ) ;
            variable_manager( so_gpx::iuser_system_ptr_t ) ;
            variable_manager( this_rref_t ) ;
            virtual ~variable_manager( void_t ) ;

        public:

            static this_ptr_t create( so_memory::purpose_cref_t ) ;
            static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
            static void_t destroy( this_ptr_t ) ;

        public:

            virtual so_manager::result destroy( void_t ) ;

            /// initiate the shutdown process. The shutdown process could be
            /// async, so be sure to check the progress via has_shutdown().
            virtual so_manager::result shutdown( void_t ) ;

            /// ask the manager if the shutdown has finished.
            virtual so_manager::result has_shutdown( void_t ) ;

        public:


            virtual so_manager::result create_variable_set(
                so_manager::key_cref_t, so_memory::purpose_cref_t ) ;

            virtual so_manager::result destroy_variable_set(
                so_manager::key_cref_t ) ;

            /// allows to destroy a unmanaged variable set. The manager will take care of 
            /// the destruction of the driver object and the set pointer itself.
            virtual so_manager::result destroy_variable_set( so_gpu::variable_set_ptr_t ) ;

            virtual so_manager::result manage_variable_set(
                so_manager::key_cref_t, so_gpu::variable_set_ptr_t ) ;


            virtual so_gpu::variable_set_ptr_t get_variable_set(
                so_manager::key_cref_t ) ;
        };
        so_typedef( variable_manager ) ;
    }
}

#endif