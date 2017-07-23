//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_MANAGER_GRAPHICS_IMAGE_MANAGER_H_
#define _SNAKEOIL_MANAGER_GRAPHICS_IMAGE_MANAGER_H_

#include "../../api.h"
#include "../../typedefs.h"
#include "../../result.h"
#include "../../protos.h"

#include <snakeoil/gpu/protos.h>

#include <snakeoil/gpx/protos.h>
#include <snakeoil/gpx/event/event.h>
#include <snakeoil/thread/typedefs.h>

#include <snakeoil/resource/handle/handle.hpp>
#include <snakeoil/resource/manager/resource_manager.hpp>

namespace so_manager
{
    namespace so_graphics
    {
        class SNAKEOIL_MANAGER_API image_manager
        {
            so_this_typedefs( image_manager ) ;

        public:

            struct i2d_manage_params
            {
                so_gpu::iimage_2d_ptr_t obj_ptr = nullptr ;
            };
            so_typedef( i2d_manage_params ) ;

            struct i3d_manage_params
            {
                so_gpu::iimage_3d_ptr_t obj_ptr = nullptr ;
            };
            so_typedef( i3d_manage_params ) ;

        private:

            struct i2d_store_item
            {
                so_gpu::iimage_2d_ptr_t obj_ptr ;
            };
            so_typedef( i2d_store_item ) ;

            struct i3d_store_item
            {
                so_gpu::iimage_3d_ptr_t obj_ptr ;
            };
            so_typedef( i3d_store_item ) ;

            so_typedefs( so_resource::resource_manager<i2d_store_item_t>, i2d_resource_manager ) ;
            so_typedefs( so_resource::resource_manager<i3d_store_item_t>, i3d_resource_manager ) ;

        public:

            so_typedefs( i2d_resource_manager_t::handle_t, i2d_handle ) ;
            so_typedefs( i3d_resource_manager_t::handle_t, i3d_handle ) ;

            so_typedefs( i2d_resource_manager_t::handles_t, i2d_handles ) ;
            so_typedefs( i3d_resource_manager_t::handles_t, i3d_handles ) ;


        private: // variables

            i2d_resource_manager_t _i2d_res_mgr ;
            i3d_resource_manager_t _i3d_res_mgr ;

            so_gpx::iuser_system_ptr_t _gpx_ptr = nullptr ;

            so_thread::mutex_t _mtx ;
            bool_t _shutdown_called = false ;
            so_gpx::event_t _shutdown_ready ;

        public:

            image_manager( so_gpx::iuser_system_ptr_t ) ;
            image_manager( this_rref_t ) ;
            virtual ~image_manager( void_t ) ;

        public:

            static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
            static void_t destroy( this_ptr_t ) ;

        public:

            bool_t acquire( so_manager::key_in_t,
                so_resource::purpose_in_t p, i2d_handle_out_t ) ;

            bool_t acquire( so_manager::key_in_t key_in,
                so_resource::purpose_in_t p, i3d_handle_out_t hnd_out ) ;

            bool_t release( i2d_handle_rref_t hnd ) ;
            bool_t release( i3d_handle_rref_t hnd ) ;

            so_manager::result insert( so_manager::key_in_t, i2d_manage_params_in_t ) ;
            so_manager::result insert( so_manager::key_in_t, i3d_manage_params_in_t ) ;

            so_manager::result alloc_and_upload_image_2d( i2d_handle_ref_t ) ;
            

        public:

            virtual void_t destroy( void_t ) ;
            virtual so_manager::result shutdown( void_t ) ;
        };
        so_typedef( image_manager ) ;
    }
}

#endif
