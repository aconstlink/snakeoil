//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_IMEX_MODULE_MODULE_REGISTRY_H_
#define _SNAKEOIL_IMEX_MODULE_MODULE_REGISTRY_H_

#include "../typedefs.h"
#include "../api.h"
#include "../protos.h"
#include "../result.h"

#include "../property/property_map.h"

#include "iscene_module.h"
#include "imesh_module.h"
#include "iimage_module.h"
#include "iaudio_module.h"

#include <snakeoil/std/container/map.hpp>
#include <snakeoil/std/container/vector.hpp>

namespace so_imex
{
    class SNAKEOIL_IMEX_API module_registry
    {
        so_this_typedefs( module_registry ) ;
        
        so_typedefs( so_std::vector<iscene_module_ptr_t>, scene_modules ) ;
        so_typedefs( so_std::vector<imesh_module_ptr_t>, mesh_modules ) ;
        so_typedefs( so_std::vector<iimage_module_ptr_t>, image_modules ) ;
        so_typedefs( so_std::vector<iaudio_module_ptr_t>, audio_modules ) ;
        
        typedef so_std::map< so_std::string_t, scene_modules_t > __scene_module_registry_t ;
        typedef so_std::map< so_std::string_t, mesh_modules_t > __mesh_module_registry_t ;
        typedef so_std::map< so_std::string_t, image_modules_t > __image_module_registry_t ;
        typedef so_std::map< so_std::string_t, audio_modules_t > __audio_module_registry_t ;
        
        so_typedefs( __scene_module_registry_t, scene_module_registry ) ;
        so_typedefs( __mesh_module_registry_t, mesh_module_registry ) ;
        so_typedefs( __image_module_registry_t, image_module_registry ) ;
        so_typedefs( __audio_module_registry_t, audio_module_registry ) ;

    private:

        scene_module_registry_t _scene_registry ;
        mesh_module_registry_t _mesh_registry ;
        image_module_registry_t _image_registry ;
        audio_module_registry_t _audio_registry ;

    public:

        module_registry( void_t ) ;
        module_registry( this_rref_t ) ;
        ~module_registry( void_t ) ;

    public:

        static this_ptr_t create( so_memory::purpose_cref_t ) ;
        static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
        static void_t destroy( this_ptr_t ) ;

    public:

        so_imex::result register_module( so_imex::iscene_module_ptr_t ) ;
        so_imex::result register_module( so_imex::imesh_module_ptr_t ) ;
        so_imex::result register_module( so_imex::iimage_module_ptr_t ) ;
        so_imex::result register_module( so_imex::iaudio_module_ptr_t ) ;

    public:

        bool_t find_module_for_path( so_io::path_cref_t, so_imex::iscene_module_ptr_t & ) ;
        bool_t find_module_for_path( so_io::path_cref_t, so_imex::imesh_module_ptr_t & ) ;
        bool_t find_module_for_path( so_io::path_cref_t, so_imex::iimage_module_ptr_t & ) ;
        bool_t find_module_for_path( so_io::path_cref_t, so_imex::iaudio_module_ptr_t & ) ;

        bool_t find_modules_for_path( so_io::path_cref_t, this_t::scene_modules_ref_t ) ;
        bool_t find_modules_for_path( so_io::path_cref_t, this_t::mesh_modules_ref_t ) ;
        bool_t find_modules_for_path( so_io::path_cref_t, this_t::image_modules_ref_t ) ;
        bool_t find_modules_for_path( so_io::path_cref_t, this_t::audio_modules_ref_t ) ;

    public:

        bool_t create_properties( property_map_out_t ) const ;

        so_thread::task_graph_t import_scene( so_imex::iscene_module::import_params_cref_t ) ;
        so_thread::task_graph_t export_scene( so_imex::iscene_module::export_params_cref_t ) ;

        so_thread::task_graph_t import_mesh( so_imex::imesh_module::import_params_cref_t ) ;
        so_thread::task_graph_t export_mesh( so_imex::imesh_module::export_params_cref_t ) ;

        so_thread::task_graph_t import_image( so_imex::iimage_module::import_params_cref_t ) ;
        so_thread::task_graph_t export_image( so_imex::iimage_module::export_params_cref_t ) ;

        so_thread::task_graph_t import_audio( so_imex::iaudio_module::import_params_cref_t ) ;
        so_thread::task_graph_t export_audio( so_imex::iaudio_module::export_params_cref_t ) ;

    };
    so_typedef( module_registry ) ;
}

#endif
