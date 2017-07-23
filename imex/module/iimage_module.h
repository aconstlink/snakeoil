//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_IMEX_MODULE_IIMAGE_MODULE_H_
#define _SNAKEOIL_IMEX_MODULE_IIMAGE_MODULE_H_

#include "../result.h"
#include "../typedefs.h"
#include "../api.h"
#include "../protos.h"
#include "../enums/image_file_formats.h"

#include "../sync_object.h"

#include <snakeoil/thread/task/task_graph.h>

#include <snakeoil/io/typedefs.h>
#include <snakeoil/io/protos.h>

namespace so_imex
{
    /// 
    class SNAKEOIL_IMEX_API iimage_module
    {

    public:

        struct init_params
        {};
        so_typedef( init_params ) ;

        struct import_params
        {
            /// if image_file_format::undefined, the format
            /// will be deduced from the file name
            so_imex::image_file_format file_format = so_imex::image_file_format::undefined;

            so_imex::key_t key ;
            so_io::path_t path_to_file ;

            so_imex::sync_object_ptr_t sync_ptr = nullptr ;

            so_imex::image_manager_ptr_t img_mgr_ptr = nullptr ;
        };
        so_typedef( import_params ) ;

        struct export_params
        {
            /// if image_file_format::undefined, the format
            /// will be deduced from the file name
            so_imex::image_file_format file_format = so_imex::image_file_format::undefined ;

            so_io::path_t path_to_file ;

            /// the key at which the image is stored
            /// in the image manager.
            so_imex::key_t image_key ;
            so_imex::image_manager_ptr_t img_mgr_ptr = nullptr ;

            so_imex::sync_object_ptr_t sync_ptr = nullptr ;
        };
        so_typedef( export_params ) ;

    protected:

        virtual so_imex::result initialize( init_params_cref_t ) = 0 ;
        
    public:

        virtual bool_t create_properties( property_map_inout_t ) const = 0 ;

        virtual bool_t is_format_supported( so_std::string_cref_t ) const = 0 ;
        virtual bool_t is_format_supported( so_imex::image_file_format ) const = 0 ;

        virtual so_imex::file_extension_names_t get_file_extension_names( void_t ) const = 0 ;

    public:

        virtual so_thread::task_graph_t import_image( import_params_cref_t ) = 0 ;
        virtual so_thread::task_graph_t export_image( export_params_cref_t ) = 0 ;

    public:

        virtual so_imex::result destroy( void_t ) = 0 ;

    };
}

#endif
