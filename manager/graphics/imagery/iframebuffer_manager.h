//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_MANAGER_GRAPHICS_IFRAMEBUFFER_MANAGER_H_
#define _SNAKEOIL_MANAGER_GRAPHICS_IFRAMEBUFFER_MANAGER_H_

#include "../../api.h"
#include "../../typedefs.h"
#include "../../result.h"
#include "../../protos.h"

#include <snakeoil/gpu/protos.h>

namespace so_manager
{
    namespace so_graphics
    {
        class SNAKEOIL_MANAGER_API iframebuffer_manager
        {
        public:

            struct framebuffer_2d_info
            {
                so_gpu::framebuffer_2d_ptr_t obj_ptr = nullptr ;
            };
            so_typedef( framebuffer_2d_info ) ;

            struct framebuffer_3d_info
            {
                so_gpu::framebuffer_3d_ptr_t obj_ptr = nullptr ;
            };
            so_typedef( framebuffer_3d_info ) ;

        public:

            virtual so_manager::result manage( 
                so_manager::key_cref_t key_in, so_gpu::framebuffer_2d_ptr_t ) = 0 ;

            virtual so_manager::result manage( 
                so_manager::key_cref_t key_in, so_gpu::framebuffer_3d_ptr_t ) = 0 ;

            /// ask the manager if a particular 2d image exists.
            virtual so_manager::result has_framebuffer_2d( so_manager::key_cref_t key_in ) = 0 ;

            /// ask the manager if a particular 3d image exists.
            virtual so_manager::result has_framebuffer_3d( so_manager::key_cref_t key_in ) = 0 ;

            /// returns ok if the output structure is filled and the input
            /// key was valid.
            virtual so_manager::result get_framebuffer_2d( 
                so_manager::key_cref_t key_in, framebuffer_2d_info_ref_t info_out ) = 0 ;

            /// returns ok if the output structure is filled and the input
            /// key was valid.
            virtual so_manager::result get_framebuffer_3d( 
                so_manager::key_cref_t key_in, framebuffer_3d_info_ref_t info_out ) = 0 ;

            virtual so_manager::result validate_and_wait( so_manager::key_cref_t ) = 0 ;

        public:

            virtual void_t destroy( void_t ) = 0 ;
            virtual so_manager::result shutdown( void_t ) = 0 ;
            virtual so_manager::result has_shutdown( void_t ) = 0 ;

        };
    }
}

#endif
