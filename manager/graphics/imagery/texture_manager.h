//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_MANAGER_GRAPHICS_TEXTURE_MANAGER_H_
#define _SNAKEOIL_MANAGER_GRAPHICS_TEXTURE_MANAGER_H_

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
        class SNAKEOIL_MANAGER_API texture_manager
        {
            so_this_typedefs( texture_manager ) ;

        public:

            struct t2d_manage_params
            {
                so_gpu::texture_2d_ptr_t obj_ptr = nullptr ;
            };
            so_typedef( t2d_manage_params ) ;

            struct t3d_manage_params
            {
                so_gpu::texture_3d_ptr_t obj_ptr = nullptr ;
            };
            so_typedef( t3d_manage_params ) ;

        private:

            struct t2d_store_item
            {
                so_gpu::texture_2d_ptr_t obj_ptr ;
            };
            so_typedef( t2d_store_item ) ;

            struct t3d_store_item
            {
                so_gpu::texture_3d_ptr_t obj_ptr ;
            };
            so_typedef( t3d_store_item ) ;

            so_typedefs( so_resource::resource_manager<t2d_store_item_t>, t2d_resource_manager ) ;
            so_typedefs( so_resource::resource_manager<t3d_store_item_t>, t3d_resource_manager ) ;

        public:

            so_typedefs( t2d_resource_manager_t::handle_t, t2d_handle ) ;
            so_typedefs( t3d_resource_manager_t::handle_t, t3d_handle ) ;

            so_typedefs( t2d_resource_manager_t::handles_t, t2d_handles ) ;
            so_typedefs( t3d_resource_manager_t::handles_t, t3d_handles ) ;

        private: // variables

            t2d_resource_manager_t _t2d_res_mgr ;
            t3d_resource_manager_t _t3d_res_mgr ;

            so_gpx::iuser_system_ptr_t _gpx_ptr = nullptr ;

            so_thread::mutex_t _mtx ;
            bool_t _shutdown_called = false ;
            so_gpx::event_t _shutdown_ready ;

        public:

            texture_manager( so_gpx::iuser_system_ptr_t ) ;
            texture_manager( this_rref_t ) ;
            virtual ~texture_manager( void_t ) ;

        public:

            static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
            static void_t destroy( this_ptr_t ) ;

        public:

            bool_t acquire( so_manager::key_cref_t key_in,
                so_resource::purpose_cref_t p, t2d_handle_out_t hnd_out ) ;

            bool_t acquire( so_manager::key_cref_t key_in,
                so_resource::purpose_cref_t p, t3d_handle_out_t hnd_out ) ;

            bool_t release( t2d_handle_rref_t hnd ) ;
            bool_t release( t3d_handle_rref_t hnd ) ;

            so_manager::result insert( so_manager::key_in_t, t2d_manage_params_in_t ) ;
            so_manager::result insert( so_manager::key_in_t, t3d_manage_params_in_t ) ;

        public:

            virtual void_t destroy( void_t ) ;
            virtual so_manager::result shutdown( void_t ) ;
        };
        so_typedef( texture_manager ) ;
    }
}

#endif
