//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_IMEX_MODULE_IMESH_MODULE_H_
#define _SNAKEOIL_IMEX_MODULE_IMESH_MODULE_H_

#include "../result.h"
#include "../typedefs.h"
#include "../api.h"
#include "../protos.h"

#include "../sync_object.h"

#include <snakeoil/thread/task/task_graph.h>
#include <snakeoil/thread/task/tasks.h>
#include <snakeoil/io/typedefs.h>

namespace so_imex
{
    /// a mesh module is supposed to im/export meshes only
    /// a mesh module can interact with
    /// -> a mesh manager
    class SNAKEOIL_IMEX_API imesh_module
    {

    public:

        struct init_params
        {
        };
        so_typedef( init_params ) ;

        struct import_params
        {
            /// allows to wait for the operation
            so_imex::sync_object_ptr_t sync_ptr = nullptr ;

            so_imex::key_t key ;
            so_io::path_t path_to_file ;

            so_imex::mesh_manager_ptr_t mesh_mgr_ptr = nullptr ;
        };
        so_typedef( import_params ) ;

        struct export_params
        {
            /// allows to wait for the operation
            so_imex::sync_object_ptr_t sync_ptr = nullptr ;

            so_io::path_t path_to_file ;

            /// the key at which the mesh is stored
            /// in the mesh manager.
            so_imex::key_t mesh_key ;

            so_imex::mesh_manager_ptr_t mesh_mgr_ptr = nullptr ;
        };
        so_typedef( export_params ) ;

    protected:

        virtual so_imex::result initialize( init_params_cref_t ) = 0 ;
        
    public:

        virtual bool_t create_properties( property_map_inout_t ) const = 0 ;

        virtual bool_t is_format_supported( so_std::string_cref_t ) const = 0  ;
        virtual so_imex::file_extension_names_t get_file_extension_names( void_t ) = 0 ;

        virtual so_thread::task_graph_t import_mesh( import_params_cref_t ) = 0 ;
        virtual so_thread::task_graph_t export_mesh( export_params_cref_t ) = 0 ;

    public:

        virtual so_imex::result destroy( void_t ) = 0 ;

    };
}

#endif
