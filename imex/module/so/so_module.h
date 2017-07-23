//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_IMEX_MODULE_SNAKEOIL_SNAKEOIL_MODULE_H_
#define _SNAKEOIL_IMEX_MODULE_SNAKEOIL_SNAKEOIL_MODULE_H_

#include "../iscene_module.h"

#include "so_import_context.h"

#include <snakeoil/thread/mutex.h>
#include <snakeoil/math/matrix/matrix4.hpp>
#include <snakeoil/math/vector/vector3.hpp>
#include <snakeoil/math/vector/vector4.hpp>

#include <snakeoil/std/container/map.hpp>

namespace so_imex
{
    class SNAKEOIL_IMEX_API snakeoil_module : public iscene_module
    {
        so_this_typedefs( snakeoil_module ) ;

    public:

        snakeoil_module( void_t ) ;
        snakeoil_module( this_rref_t ) ;
        virtual ~snakeoil_module( void_t ) ;

    public:

        static this_ptr_t create( so_memory::purpose_cref_t ) ;
        static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
        static void_t destroy( this_ptr_t ) ;
    
    public:

        virtual so_imex::result initialize( init_params_cref_t ) ;
        virtual so_imex::result destroy( void_t ) ;
        
    public:

        virtual bool_t create_properties( property_map_inout_t ) const ;

        virtual bool_t is_format_supported( so_std::string_cref_t ) const ;
        virtual so_imex::file_extension_names_t get_file_extension_names( void_t ) ;

        virtual so_thread::task_graph_t import_scene( import_params_cref_t ) ;
        virtual so_thread::task_graph_t export_scene( export_params_cref_t ) ;

    private: // import

        so_thread::itask_ptr_t load_file( so_io::path_cref_t, snakeoil_import_context_ref_t ) ;

    
    };
    so_typedef( snakeoil_module ) ;
}

#endif
