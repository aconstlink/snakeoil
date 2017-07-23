//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_IMEX_SYSTEM_ISYSTEM_H_
#define _SNAKEOIL_IMEX_SYSTEM_ISYSTEM_H_

#include "../typedefs.h"
#include "../protos.h"
#include "../result.h"
#include "../api.h"

#include "../sync_object.h"
#include "../enums/image_file_formats.h"

#include <snakeoil/thread/protos.h>
#include <snakeoil/thread/task/task_graph.h>

#include <snakeoil/std/container/vector.hpp>
#include <snakeoil/std/string/string.hpp>

namespace so_imex
{
    class SNAKEOIL_IMEX_API isystem
    {
        so_typedefs( so_std::vector< so_std::string_t >, strings ) ;

    public: // register

        virtual so_imex::result register_module( so_imex::iscene_module_ptr_t ) = 0 ;
        virtual so_imex::result register_module( so_imex::iimage_module_ptr_t ) = 0 ;
        virtual so_imex::result register_module( so_imex::imesh_module_ptr_t ) = 0 ;
        virtual so_imex::result register_module( so_imex::icode_module_ptr_t ) = 0 ;
        virtual so_imex::result register_module( so_imex::iaudio_module_ptr_t ) = 0 ;

        virtual bool_t find_module_for_path( so_io::path_cref_t, so_imex::iscene_module_ptr_t & ) = 0 ;
        virtual bool_t find_module_for_path( so_io::path_cref_t, so_imex::iimage_module_ptr_t & ) = 0 ;
        virtual bool_t find_module_for_path( so_io::path_cref_t, so_imex::imesh_module_ptr_t & ) = 0 ;
        virtual bool_t find_module_for_path( so_io::path_cref_t, so_imex::iaudio_module_ptr_t & ) = 0 ;
        
    public: // registry

        virtual module_registry_ptr_t get_module_registry( void_t ) = 0 ;
        virtual manager_registry_ptr_t get_manager_registry( void_t ) = 0 ;

    public: // access

        virtual so_imex::graph_manager_ptr_t get_graph_manager( void_t ) = 0 ;
        virtual so_imex::camera_manager_ptr_t get_camera_manager( void_t ) = 0 ;
        virtual so_imex::mesh_manager_ptr_t get_mesh_manager( void_t ) = 0 ;
        virtual so_imex::image_manager_ptr_t get_image_manager( void_t ) = 0 ;
        virtual so_imex::audio_manager_ptr_t get_audio_manager( void_t ) = 0 ;

    public:

        virtual so_imex::result destroy( void_t ) = 0 ;
    };
}

#endif
