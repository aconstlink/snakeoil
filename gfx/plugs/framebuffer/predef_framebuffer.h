//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "../../api.h"
#include "../../protos.h"
#include "../../typedefs.h"
#include "../../result.h"

#include "predef_framebuffer_types.h"

#include <snakeoil/gpx/system/render_system.h>
#include <snakeoil/gpu/viewport/viewport_2d.h>
#include <snakeoil/flow/variable/variable_set.h>
#include <snakeoil/flow/variable/variable.hpp>
#include <snakeoil/flow/node/variable/variable_node.h>
#include <snakeoil/std/string/string.hpp>

namespace so_gfx
{
    class SNAKEOIL_GFX_API predef_framebuffer
    {
        so_this_typedefs( predef_framebuffer ) ;

    private:

    public:

        struct shared_data
        {
            so_this_typedefs( shared_data ) ;

            // post name/type
            // source fb name

            so_std::string_t fb_name ;
            size_t width ;
            size_t height ;
            size_t num_color ;

            so_gpu::viewport_2d vp ;
            so_math::vec4f_t color ;

            predef_framebuffer_type type ;

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

        so_gfx::predef_framebuffer_plug_factory_ptr_t _fac_ptr = nullptr ;

        so_gpx::technique_id_t _tid ;
        so_gpx::render_system_ptr_t _rs ;

        predef_framebuffer_type _type ;

    public:

        predef_framebuffer( predef_framebuffer_type t, so_gpx::render_system_ptr_t ) ;
        predef_framebuffer( this_cref_t ) = delete ;
        predef_framebuffer( this_rref_t ) ;
        ~predef_framebuffer( void_t ) ;

    public:

        so_flow::variable_set_ptr_t get_varset( void_t ) ;

        void_t init( so_std::string_cref_t name, size_t const w, size_t const h, size_t const nc=1 ) ;
        void_t set_viewport( so_gpu::viewport_2d_cref_t ) ;
        void_t set_clear_color( so_math::vec4f_cref_t ) ;

        void_t schedule_for_clear( void_t ) ;
        void_t schedule_for_begin( void_t ) ;
        void_t schedule_for_end( void_t ) ;
        void_t schedule_for_reload( void_t ) ;

    public:

        static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
        static void_t destroy( this_ptr_t ) ;        
    };
    so_typedef( predef_framebuffer ) ;
}