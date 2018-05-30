//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "../../api.h"
#include "../../protos.h"
#include "../../typedefs.h"
#include "predef_post_type.h"

#include <snakeoil/gpx/system/render_system.h>
#include <snakeoil/gpu/viewport/viewport_2d.h>
#include <snakeoil/flow/variable/variable_set.h>
#include <snakeoil/flow/variable/variable.hpp>
#include <snakeoil/flow/node/variable/variable_node.h>
#include <snakeoil/std/string/string.hpp>

namespace so_gfx
{
    class SNAKEOIL_GFX_API predef_post
    {
        so_this_typedefs( predef_post ) ;

    private:

    public:

        struct shared_data
        {
            so_this_typedefs( shared_data ) ;

            // post name/type
            // source fb name

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

        so_gfx::predef_post_plug_factory_ptr_t _fac_ptr = nullptr ;

        so_gpx::technique_id_t _tid ;
        so_gpx::render_system_ptr_t _rs ;

        predef_post_type _type ;

    public:

        predef_post( predef_post_type, so_gpx::render_system_ptr_t ) ;
        predef_post( this_cref_t ) = delete ;
        predef_post( this_rref_t ) ;
        ~predef_post( void_t ) ;

    public:

        so_flow::variable_set_ptr_t get_varset( void_t ) ;

        void_t set_source_color_name( so_std::string_cref_t ) ;
        void_t set_source_rect( so_math::vec4f_cref_t ) ;
        void_t set_dest_rect( so_math::vec4f_cref_t ) ;
        void_t set_dest_rect( so_gpu::viewport_2d_cref_t ) ;

    public:

        static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
        static void_t destroy( this_ptr_t ) ;

    public:

        void_t schedule( void_t ) ;
        void_t schedule_for_reload( void_t ) ;
    };
    so_typedef( predef_post ) ;
}