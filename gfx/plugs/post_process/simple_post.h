//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "../../api.h"
#include "../../protos.h"
#include "../../typedefs.h"

#include <snakeoil/gpx/system/render_system.h>
#include <snakeoil/flow/variable/variable_set.h>
#include <snakeoil/flow/variable/variable.hpp>
#include <snakeoil/flow/node/variable/variable_node.h>
#include <snakeoil/std/string/string.hpp>

namespace so_gfx
{
    class SNAKEOIL_GFX_API simple_post
    {
        so_this_typedefs( simple_post ) ;

    private:

    public:

        struct shared_data
        {
            so_this_typedefs( shared_data ) ;

            // post name/type
            // source fb name
            so_std::string_t name ;

            // dest fb name

            

            so_flow::variable_set_t vs ;
            so_flow::variable_node_t vn ;

            shared_data( void_t )
            {}

            shared_data( this_cref_t ) = delete ;
            shared_data( this_rref_t rhv )
            {
                vs = std::move( rhv.vs ) ;
                vn = std::move( rhv.vn ) ;
            }

            ~shared_data( void_t ) {}
        };
        so_typedef( shared_data ) ;
        shared_data_ptr_t _sd ;

        so_gpx::technique_id_t _tid ;
        so_gpx::render_system_ptr_t _rs ;

    public:

        simple_post( so_gpx::render_system_ptr_t ) ;
        simple_post( this_cref_t ) = delete ;
        simple_post( this_rref_t ) ;
        ~simple_post( void_t ) ;

    public:

        void_t set_source_color_name( so_std::string_cref_t ) ;

        so_flow::variable_set_ptr_t get_varset( void_t ) ;

    public:

        static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
        static void_t destroy( this_ptr_t ) ;

    public:

        void_t schedule( void_t ) ;
    };
    so_typedef( simple_post ) ;
}