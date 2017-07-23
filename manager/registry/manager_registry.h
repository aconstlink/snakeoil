//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_MANAGER_REGISTRY_MANAGER_REGISTRY_H_
#define _SNAKEOIL_MANAGER_REGISTRY_MANAGER_REGISTRY_H_

#include "../typedefs.h"
#include "../protos.h"
#include "../api.h"
#include "../result.h"

#include <snakeoil/gpx/protos.h>
#include <snakeoil/std/container/vector.hpp>

namespace so_manager
{
    class SNAKEOIL_MANAGER_API manager_registry
    {
        so_this_typedefs( manager_registry ) ;

    private:

        so_typedefs( so_std::vector<code_manager_ptr_t>, code_managers ) ;
        so_typedefs( so_std::vector<camera_manager_ptr_t>, camera_managers ) ;
        
        so_typedefs( so_std::vector<so_graphics::shader_manager_ptr_t>, gfx_shader_managers ) ;
        so_typedefs( so_std::vector<so_graphics::program_manager_ptr_t>, gfx_program_managers ) ;
        so_typedefs( so_std::vector<so_graphics::geometry_manager_ptr_t>, gfx_geometry_managers ) ;
        so_typedefs( so_std::vector<so_graphics::variable_manager_ptr_t>, gfx_variable_managers ) ;
        so_typedefs( so_std::vector<so_graphics::framebuffer_manager_ptr_t>, gfx_framebuffer_managers ) ;
        so_typedefs( so_std::vector<so_graphics::texture_manager_ptr_t>, gfx_texture_managers ) ;
        so_typedefs( so_std::vector<so_graphics::image_manager_ptr_t>, gfx_image_managers ) ;

    private:

        so_gpx::iuser_system_ptr_t _gpx_ptr = nullptr ;

        code_manager_ptr_t _code_manager_ptr = nullptr ;
        camera_manager_ptr_t _camera_manager_ptr = nullptr ;

        gfx_shader_managers_t _gfx_shader_managers ;
        gfx_program_managers_t _gfx_program_managers ;
        gfx_geometry_managers_t _gfx_geometry_managers ;
        gfx_variable_managers_t _gfx_variable_managers ;
        gfx_framebuffer_managers_t _gfx_framebuffer_managers ;
        gfx_texture_managers_t _gfx_texture_managers ;
        gfx_image_managers_t _gfx_image_managers ;

        bool_t _shutdown_called = false ;

    public:

        manager_registry( so_gpx::iuser_system_ptr_t ) ;
        manager_registry( this_rref_t ) ;
        ~manager_registry( void_t ) ;

    public:

        static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
        static void_t destroy( this_ptr_t ) ;

    public:

        so_manager::code_manager_ptr_t code_manager( void_t ) ;
        so_manager::camera_manager_ptr_t camera_manager( void_t ) ;

        so_manager::so_graphics::shader_manager_ptr_t gpu_shader_manager( size_t i=0 ) ;
        so_manager::so_graphics::program_manager_ptr_t gpu_program_manager( size_t i=0 ) ;
        so_manager::so_graphics::geometry_manager_ptr_t gpu_geometry_manager( size_t i=0 ) ;
        so_manager::so_graphics::variable_manager_ptr_t gpu_variable_manager( size_t i=0 ) ;
        so_manager::so_graphics::texture_manager_ptr_t gpu_texture_manager( size_t i=0 ) ;
        so_manager::so_graphics::image_manager_ptr_t gpu_image_manager( size_t i=0 ) ;
        so_manager::so_graphics::framebuffer_manager_ptr_t gpu_framebuffer_manager( size_t i=0 ) ;

    public:

        so_manager::result shutdown( void_t ) ;
        

    };
    so_typedef( manager_registry ) ;
}

#endif