//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_MANAGER_GRAPHICS_ISHADER_MANAGER_H_
#define _SNAKEOIL_MANAGER_GRAPHICS_ISHADER_MANAGER_H_

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
        /// managing graphics shaders by creating and
        /// compiling them
        class SNAKEOIL_MANAGER_API ishader_manager
        {
        public:

            virtual so_manager::result destroy( void_t ) = 0 ;

            /// initiate the shutdown process.
            virtual so_manager::result shutdown( void_t ) = 0 ;

            /// ask the manager if the shutdown has finished.
            virtual so_manager::result has_shutdown( void_t ) = 0 ;

        public:

            /// compose a shader given by type using the passed code keys.
            /// a code manager is required for this task.
            virtual so_manager::result compose_shader( so_gpu::shader_type, 
                so_manager::key_cref_t key, so_manager::key_list_cref_t code_keys ) = 0 ;

            /// compose a shader given by type using the passed code keys.
            /// a code manager is required for this task.
            /// this function will also init a shader compilation process.
            /// use get_compilation_status for compilation information.
            virtual so_manager::result compose_and_compile_shader( so_gpu::shader_type, 
                so_manager::key_cref_t key, so_manager::key_list_cref_t code_keys ) = 0 ;

            /// initiates a compilation of the passed shader. 
            /// use get_compilation_status for compilation information.
            virtual so_manager::result compile_shader( so_gpu::shader_type, 
                so_manager::key_cref_t ) = 0 ;

            /// with a prior issued compilation process, this function returns the
            /// compilation information. This function does not block execution.
            virtual so_gpu::result get_compilation_status( so_gpu::shader_type, 
                so_manager::key_cref_t ) = 0 ;

            /// allows to wait for the compilation to finish.
            /// this function blocks until the compilation has finished.
            virtual so_gpu::result wait_for_compilation( so_gpu::shader_type, 
                so_manager::key_cref_t ) = 0 ;

        public:

            /// get a previously composed vertex shader
            virtual so_gpu::vertex_shader_ptr_t get_vertex_shader( so_manager::key_cref_t ) = 0 ;
            
            /// get a previously composed geometry shader
            virtual so_gpu::geometry_shader_ptr_t get_geometry_shader( so_manager::key_cref_t ) = 0 ;
            
            /// get a previously composed pixel shader
            virtual so_gpu::pixel_shader_ptr_t get_pixel_shader( so_manager::key_cref_t ) = 0 ;
        };
    }
}

#endif
