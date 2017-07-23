//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_IMEX_MANAGER_MANAGER_REGISTRY_H_
#define _SNAKEOIL_IMEX_MANAGER_MANAGER_REGISTRY_H_

#include "../typedefs.h"
#include "../result.h"
#include "../api.h"
#include "../protos.h"

#include <snakeoil/std/container/map.hpp>
#include <snakeoil/std/string/string.hpp>

namespace so_imex
{
    class SNAKEOIL_IMEX_API manager_registry
    {
        so_this_typedefs( manager_registry ) ;

        typedef so_std::map< so_std::string_t, audio_manager_ptr_t > audio_managers_t ;
        typedef so_std::map< so_std::string_t, image_manager_ptr_t > image_managers_t ;
        typedef so_std::map< so_std::string_t, graph_manager_ptr_t > graph_managers_t ;
        typedef so_std::map< so_std::string_t, mesh_manager_ptr_t > mesh_managers_t ;
        typedef so_std::map< so_std::string_t, camera_manager_ptr_t > cam_managers_t ;
        typedef so_std::map< so_std::string_t, material_manager_ptr_t > material_managers_t ;
        typedef so_std::map< so_std::string_t, vertex_shader_manager_ptr_t > vertex_shader_managers_t ;
        typedef so_std::map< so_std::string_t, pixel_shader_manager_ptr_t > pixel_shader_managers_t ;
        

    private:

        audio_managers_t _audio_managers ;
        image_managers_t _image_managers ;
        graph_managers_t _graph_managers ;
        mesh_managers_t _mesh_managers ;
        cam_managers_t _camera_managers ;
        material_managers_t _material_managers ;
        vertex_shader_managers_t _vs_managers ;
        pixel_shader_managers_t _ps_managers ;

    public:

        manager_registry( void_t ) ;
        manager_registry( this_rref_t ) ;
        ~manager_registry( void_t ) ;

    public: 

        static this_ptr_t create( so_memory::purpose_cref_t ) ;
        static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
        static void_t destroy( this_ptr_t ) ;

    public:

        audio_manager_ptr_t get_audio_manager( void_t ) ;
        image_manager_ptr_t get_image_manager( void_t ) ;
        graph_manager_ptr_t get_graph_manager( void_t ) ;
        mesh_manager_ptr_t get_mesh_manager( void_t ) ;
        camera_manager_ptr_t get_camera_manager( void_t ) ;
        material_manager_ptr_t get_material_manager( void_t ) ;
        vertex_shader_manager_ptr_t get_vertex_shader_manager( void_t ) ;
        pixel_shader_manager_ptr_t get_pixel_shader_manager( void_t ) ;

    public:

        so_imex::result take_over( this_ptr_t ) ;

    public: // idea: register_manager( key, manager ) 

    };
    so_typedef( manager_registry ) ;
}

#endif
