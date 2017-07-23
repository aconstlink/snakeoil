//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_MANAGER_GRAPHICS_FRAMEBUFFER_MANAGER_H_
#define _SNAKEOIL_MANAGER_GRAPHICS_FRAMEBUFFER_MANAGER_H_

#include "iframebuffer_manager.h"

#include <snakeoil/gpx/protos.h>
#include <snakeoil/thread/mutex.h>
#include <snakeoil/std/string/string.hpp>
#include <snakeoil/std/container/map.hpp>

namespace so_manager
{
    namespace so_graphics
    {
        class SNAKEOIL_MANAGER_API framebuffer_manager : public iframebuffer_manager
        {
            so_this_typedefs( framebuffer_manager ) ;

        private:

            struct framebuffer_2d_data
            {
                so_gpu::framebuffer_2d_ptr_t obj_ptr ;
            };
            so_typedef( framebuffer_2d_data ) ;

            struct framebuffer_3d_data
            {
                so_gpu::framebuffer_3d_ptr_t obj_ptr ;
            };
            so_typedef( framebuffer_3d_data ) ;

            typedef so_std::map< so_std::string, framebuffer_2d_data > key_to_framebuffer_2d_t ;
            typedef so_std::map< so_std::string, framebuffer_3d_data > key_to_framebuffer_3d_t ;

        private:

            so_gpx::iuser_system_ptr_t _gpx_ptr = nullptr ;

            so_thread::mutex_t _2d_mtx ;
            key_to_framebuffer_2d_t _2ds ;

            so_thread::mutex_t _3d_mtx ;
            key_to_framebuffer_3d_t _3ds ;

            bool_t _shutdown_called = false ;
            bool_t _shutdown_complete = false ;

        public:

            framebuffer_manager( so_gpx::iuser_system_ptr_t ) ;
            framebuffer_manager( this_rref_t ) ;
            virtual ~framebuffer_manager( void_t ) ;

        public:

            static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
            static void_t destroy( this_ptr_t ) ;

        public:

            virtual so_manager::result manage( 
                so_manager::key_cref_t key_in, so_gpu::framebuffer_2d_ptr_t ) ;

            virtual so_manager::result manage( 
                so_manager::key_cref_t key_in, so_gpu::framebuffer_3d_ptr_t ) ;

        
            virtual so_manager::result has_framebuffer_2d( so_manager::key_cref_t key_in ) ;
            virtual so_manager::result has_framebuffer_3d( so_manager::key_cref_t key_in ) ;


            virtual so_manager::result get_framebuffer_2d( 
                so_manager::key_cref_t key_in, framebuffer_2d_info_ref_t info_out ) ;

            virtual so_manager::result get_framebuffer_3d( 
                so_manager::key_cref_t key_in, framebuffer_3d_info_ref_t info_out ) ;

            virtual so_manager::result validate_and_wait( so_manager::key_cref_t ) ;

        public:

            virtual void_t destroy( void_t ) ;
            virtual so_manager::result shutdown( void_t ) ;
            virtual so_manager::result has_shutdown( void_t ) ;
        };
        so_typedef( framebuffer_manager ) ;
    }
}

#endif
