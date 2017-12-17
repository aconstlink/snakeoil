//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "../protos.h"
#include "../result.h"
#include "../api.h"

#include "rect_render_2d_shared_data.h"

#include <snakeoil/gpx/protos.h>
#include <snakeoil/gpx/technique/technique_id.h>

namespace so_gfx
{
    class SNAKEOIL_GFX_API rect_render_2d
    {
        so_this_typedefs( rect_render_2d ) ;


    private:

        struct rect_info
        {
            so_math::vec2f_t pos ;
            so_math::vec2f_t scale ;
            float_t rot ;
            so_math::vec4f_t color ;
        };
        so_typedef( rect_info ) ;

        struct group_info
        {
            so_this_typedefs( group_info ) ;

            so_thread::mutex_t mtx ;

            typedef so_std::vector< rect_info > rect_infos_t ;
            rect_infos_t rect_infos ;

            size_t group_id = 0 ;

            so_math::mat4f_t proj ;
            so_math::mat4f_t view ;

            group_info( void_t ) {}
            group_info( this_cref_t rhv ) {
                rect_infos = rhv.rect_infos ;
                group_id = rhv.group_id ;
                proj = rhv.proj ;
                view = rhv.view ;
            }
            group_info( this_rref_t rhv ) {
                rect_infos = std::move( rhv.rect_infos ) ;
                group_id = rhv.group_id ;
                proj = rhv.proj ;
                view = rhv.view ;
            }
            ~group_info( void_t ) {}

            this_ref_t operator = ( this_cref_t rhv ) {
                rect_infos = rhv.rect_infos ;
                group_id = rhv.group_id ;
                proj = rhv.proj ;
                view = rhv.view ;
                return *this ;
            }
            this_ref_t operator = ( this_rref_t rhv ) {
                rect_infos = std::move( rhv.rect_infos ) ;
                group_id = rhv.group_id ;
                proj = rhv.proj ;
                view = rhv.view ;
                return *this ;
            }

            bool_t operator == ( this_cref_t rhv ) const {
                return group_id == group_id ;
            }
        };
        so_typedef( group_info ) ;
        so_typedefs( so_std::vector< group_info_ptr_t >, group_infos ) ;

        so_thread::mutex_t _mtx_group ;
        group_infos_t _group_infos ;

        so_typedefs( so_std::vector< size_t >, render_group_ids ) ;
        render_group_ids_t _render_groups ;

    private:

        so_gpx::technique_id_t _t_rnd = so_gpx::technique_id_t( -1 ) ;
        so_gfx::rect_render_2d_plug_factory_ptr_t _fac_ptr = nullptr ;

        so_gfx::rect_render_2d_shared_data_ptr_t _sd_ptr = nullptr ;

        so_gpx::render_system_ptr_t _gpxr = nullptr ;

        so_math::mat4f_t _proj ;
        so_math::mat4f_t _view ;

    public:

        rect_render_2d( so_gpx::render_system_ptr_t ) ;
        rect_render_2d( this_cref_t ) = delete ;
        rect_render_2d( this_rref_t ) ;
        ~rect_render_2d( void_t ) ;

    public:

        static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
        static void_t destroy( this_ptr_t ) ;

    public:

        void_t init( void_t ) ;

        void_t set_view_projection( so_math::mat4f_cref_t view, so_math::mat4f_cref_t proj ) ;

        so_gfx::result draw_rect( size_t const group, so_math::vec2f_cref_t pos, so_math::vec2f_cref_t scale, 
            float_t const rot, so_math::vec4f_cref_t color ) ;

        so_gfx::result prepare_for_rendering( void_t ) ;
        bool_t need_to_render( size_t const ) const ;
        so_gfx::result render( size_t const ) ;

        so_gfx::result release( void_t ) ;

    private:

        group_info_ptr_t insert_group( size_t const ) ;

    };
    so_typedef( rect_render_2d ) ;
}