//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_IMEX_MODULE_CTM_CTM_MODULE_H_
#define _SNAKEOIL_IMEX_MODULE_CTM_CTM_MODULE_H_

#include "../imesh_module.h"

#include <snakeoil/geometry/mesh/flat_tri_mesh.h>

namespace so_imex
{
    class SNAKEOIL_IMEX_API ctm_module : public imesh_module
    {
        so_this_typedefs( ctm_module ) ;

    private:

        ctm_module( this_cref_t ) {}

    public:

        ctm_module( void_t ) ;
        ctm_module( this_rref_t ) ;
        virtual ~ctm_module( void_t ) ;

    public:

        static this_ptr_t create( so_memory::purpose_cref_t ) ;
        static void_t destroy( this_ptr_t ) ;

    public:

        virtual so_imex::result initialize( init_params_cref_t ) ;
        virtual bool_t create_properties( property_map_inout_t ) const ;

    public:

        virtual bool_t is_format_supported( so_std::string_cref_t ) const ;
        virtual so_imex::file_extension_names_t get_file_extension_names( void_t ) ;

        virtual so_thread::task_graph_t import_mesh( import_params_cref_t ) ;
        virtual so_thread::task_graph_t export_mesh( export_params_cref_t ) ;

    public:

        virtual so_imex::result destroy( void_t ) ;

    private:

        so_imex::result do_import( so_io::path_cref_t, so_geo::flat_tri_mesh_ref_t ) ;
        so_imex::result do_export( so_io::path_cref_t, so_geo::flat_tri_mesh_cref_t ) ;

    };
    so_typedef( ctm_module ) ;
}

#endif
