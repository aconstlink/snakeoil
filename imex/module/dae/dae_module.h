//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_IMEX_MODULE_DAE_DAE_MODULE_H_
#define _SNAKEOIL_IMEX_MODULE_DAE_DAE_MODULE_H_

#include "../iscene_module.h"

#include "dae_export_context.h"

#include <snakeoil/thread/mutex.h>
#include <snakeoil/math/matrix/matrix4.hpp>
#include <snakeoil/math/vector/vector3.hpp>
#include <snakeoil/math/vector/vector4.hpp>

#include <snakeoil/std/container/map.hpp>

namespace so_imex
{
    class SNAKEOIL_IMEX_API dae_module : public so_imex::iscene_module
    {
        so_this_typedefs( dae_module ) ;


    public:

        static this_ptr_t create( so_memory::purpose_cref_t );
        static void_t destroy( this_ptr_t );

    protected:

        virtual so_imex::result initialize( init_params_cref_t ) ;
        virtual bool_t create_properties( property_map_inout_t ) const ;

    public:

        virtual bool_t is_format_supported( so_std::string_cref_t ) const ;
        virtual so_imex::file_extension_names_t get_file_extension_names( void_t ) ;

        virtual so_thread::task_graph_t import_scene( import_params_cref_t ) ;
        virtual so_thread::task_graph_t export_scene( export_params_cref_t ) ;

    public:

        virtual so_imex::result destroy( void_t ) ;
    };
    so_typedef( dae_module ) ;
}

#endif