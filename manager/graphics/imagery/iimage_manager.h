//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_MANAGER_GRAPHICS_IIMAGE_MANAGER_H_
#define _SNAKEOIL_MANAGER_GRAPHICS_IIMAGE_MANAGER_H_

#include "../../so_api.h"
#include "../../typedefs.h"
#include "../../result.h"
#include "../../protos.h"

#include <snakeoil/gpu/protos.h>

namespace so_manager
{
    namespace so_graphics
    {
        class SNAKEOIL_MANAGER_API iimage_manager
        {
        public:

            struct image_2d_info
            {
                so_gpu::iimage_2d_ptr_t image_ptr = nullptr ;
            };
            so_typedef( image_2d_info ) ;

            struct image_3d_info
            {
                so_gpu::iimage_3d_ptr_t image_ptr = nullptr ;
            };
            so_typedef( image_3d_info ) ;

        public:

            virtual so_manager::result manage( 
                so_manager::key_cref_t key_in, so_gpu::iimage_2d_ptr_t ) = 0 ;

            virtual so_manager::result manage( 
                so_manager::key_cref_t key_in, so_gpu::iimage_3d_ptr_t ) = 0 ;

            /// ask the manager if a particular 2d image exists.
            virtual so_manager::result has_image_2d( so_manager::key_cref_t key_in ) = 0 ;

            /// ask the manager if a particular 3d image exists.
            virtual so_manager::result has_image_3d( so_manager::key_cref_t key_in ) = 0 ;

            /// returns ok if the output structure is filled and the input
            /// key was valid.
            virtual so_manager::result get_image_2d( 
                so_manager::key_cref_t key_in, image_2d_info_ref_t info_out ) = 0 ;

            /// returns ok if the output structure is filled and the input
            /// key was valid.
            virtual so_manager::result get_image_3d( 
                so_manager::key_cref_t key_in, image_3d_info_ref_t info_out ) = 0 ;

            virtual so_manager::result alloc_and_upload_image_2d( so_manager::key_cref_t ) = 0 ;
            virtual so_manager::result upload_image_3d( so_manager::key_cref_t ) = 0 ;

        public:

            virtual void_t destroy( void_t ) = 0 ;
            virtual so_manager::result shutdown( void_t ) = 0 ;
            virtual so_manager::result has_shutdown( void_t ) = 0 ;
        };
    }
}

#endif
