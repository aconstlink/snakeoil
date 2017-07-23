//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_MANAGER_GRAPHICS_SHADER_SHADER_MANAGER_H_
#define _SNAKEOIL_MANAGER_GRAPHICS_SHADER_SHADER_MANAGER_H_

#include "ishader_manager.h"

#include "core_shader_manager.hpp"

#include <snakeoil/gpu/shader/vertex_shader.h>
#include <snakeoil/gpu/shader/geometry_shader.h>
#include <snakeoil/gpu/shader/pixel_shader.h>
#include <snakeoil/thread/typedefs.h>

namespace so_manager
{
    namespace so_graphics
    {
        class SNAKEOIL_MANAGER_API shader_manager : public ishader_manager
        {
            so_this_typedefs( shader_manager ) ;

        private:

            typedef so_manager::so_graphics::core_shader_manager<so_gpu::vertex_shader> 
                vertex_shader_manager_t ;
            
            typedef so_manager::so_graphics::core_shader_manager<so_gpu::geometry_shader> 
                geometry_shader_manager_t ;

            typedef so_manager::so_graphics::core_shader_manager<so_gpu::pixel_shader> 
                pixel_shader_manager_t ;

        private:

            so_thread::mutex_t _mtx ;

            /// required for shader composition.
            so_manager::code_manager_ptr_t _code_mgr_ptr = nullptr ;
            
            /// required for shader compilation.
            so_gpx::isystem_ptr_t _gpxs_ptr = nullptr ;

            vertex_shader_manager_t _vs_manager ;
            geometry_shader_manager_t _gs_manager ;
            pixel_shader_manager_t _ps_manager ;

            bool_t _shutdown_called = false ;

        private:

            // no copy support
            shader_manager( this_cref_t ) : _vs_manager(nullptr),
            _gs_manager(nullptr), _ps_manager(nullptr) {}

        public:

            shader_manager( code_manager_ptr_t, so_gpx::iuser_system_ptr_t ) ;
            shader_manager( this_rref_t ) ;
            
            /// by destruction of the shader manager, all core managers
            /// are also destructing. That will delete all shutdown shaders.
            virtual ~shader_manager( void_t ) ;

        public:

            static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
            static void_t destroy( this_ptr_t ) ;

        public:

            virtual so_manager::result destroy( void_t ) ;

            /// this function does not destroy objects. It only released
            /// all shaders at the gpx system.
            virtual so_manager::result shutdown( void_t ) ;
            virtual so_manager::result has_shutdown( void_t ) ;

        public:

            virtual so_manager::result compose_shader( so_gpu::shader_type, 
                so_manager::key_cref_t key, so_manager::key_list_cref_t code_keys ) ;

            virtual so_manager::result compose_and_compile_shader( so_gpu::shader_type, 
                so_manager::key_cref_t key, so_manager::key_list_cref_t code_keys ) ;

            virtual so_manager::result compile_shader( so_gpu::shader_type, 
                so_manager::key_cref_t ) ;

            virtual so_gpu::result get_compilation_status( so_gpu::shader_type, 
                so_manager::key_cref_t ) ;

            virtual so_gpu::result wait_for_compilation( so_gpu::shader_type, 
                so_manager::key_cref_t ) ;

        public:

            virtual so_gpu::vertex_shader_ptr_t get_vertex_shader( so_manager::key_cref_t ) ;
            virtual so_gpu::geometry_shader_ptr_t get_geometry_shader( so_manager::key_cref_t ) ;
            virtual so_gpu::pixel_shader_ptr_t get_pixel_shader( so_manager::key_cref_t ) ;

        private:

        };
        so_typedef( shader_manager ) ;
    }
}

#endif
