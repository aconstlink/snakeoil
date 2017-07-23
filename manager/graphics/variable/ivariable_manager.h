//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_MANAGER_GRAPHICS_VARIABLE_IVARIABLE_MANAGER_H_
#define _SNAKEOIL_MANAGER_GRAPHICS_VARIABLE_IVARIABLE_MANAGER_H_

#include "../../api.h"
#include "../../result.h"
#include "../../typedefs.h"
#include "../../protos.h"

#include <snakeoil/gpu/shader/shader_types.h>
#include <snakeoil/gpu/protos.h>
#include <snakeoil/gpu/result.h>

#include <snakeoil/gpx/protos.h>

namespace so_manager
{
    namespace so_graphics
    {
        class SNAKEOIL_MANAGER_API ivariable_manager
        {
        public:

            virtual so_manager::result destroy( void_t ) = 0 ;

            /// initiate the shutdown process. The shutdown process could be
            /// async, so be sure to check the progress via has_shutdown().
            virtual so_manager::result shutdown( void_t ) = 0 ;

            /// ask the manager if the shutdown has finished.
            virtual so_manager::result has_shutdown( void_t ) = 0 ;

        public:


            virtual so_manager::result create_variable_set( 
                so_manager::key_cref_t, so_memory::purpose_cref_t ) = 0 ;

            virtual so_manager::result destroy_variable_set( 
                so_manager::key_cref_t ) = 0 ;

            /// allows to destroy a unmanaged variable set. The manager will take care of 
            /// the destruction of the driver object and the set pointer itself.
            virtual so_manager::result destroy_variable_set( so_gpu::variable_set_ptr_t ) = 0 ;

            virtual so_manager::result manage_variable_set( 
                so_manager::key_cref_t, so_gpu::variable_set_ptr_t ) = 0 ;


            virtual so_gpu::variable_set_ptr_t get_variable_set( 
                so_manager::key_cref_t ) = 0 ;
        };
    }
}

#endif