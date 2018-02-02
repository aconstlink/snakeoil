//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "../api.h"
#include "../typedefs.h"
#include "../protos.h"
#include "../result.h"

#include "line_render_2d_shared_data.h"

#include <snakeoil/gpx/system/render_system.h>

#include <functional>

namespace so_gfx
{
    class SNAKEOIL_GFX_API line_render_2d
    {
        so_this_typedefs( line_render_2d ) ;

    public:

        struct line_info
        {
            so_math::vec2f_t pos0 ;
            so_math::vec2f_t pos1 ;
            so_math::vec4f_t color ;
        };
        so_typedef( line_info ) ;

    private:

        struct group_info
        {
            so_this_typedefs( group_info ) ;

            so_thread::mutex_t mtx ;
            so_std::vector< line_info > line_infos ;

            size_t group_id = 0 ;

            so_math::mat4f_t proj ;
            so_math::mat4f_t view ;

            float_t line_width = 1.0f ;

            group_info( void_t ) {}
            group_info( this_cref_t rhv ) {
                line_infos = rhv.line_infos ;
                group_id = rhv.group_id ;
                proj = rhv.proj ;
                view = rhv.view ;
                line_width = rhv.line_width ;
            }
            group_info( this_rref_t rhv ) {
                line_infos = std::move( rhv.line_infos ) ;
                group_id = rhv.group_id ;
                proj = rhv.proj ;
                view = rhv.view ;
                line_width = rhv.line_width ;
            }
            ~group_info( void_t ) {}

            this_ref_t operator = ( this_cref_t rhv ) {
                line_infos = rhv.line_infos ;
                group_id = rhv.group_id ;
                proj = rhv.proj ;
                view = rhv.view ;
                line_width = rhv.line_width ;
                return *this ;
            }
            this_ref_t operator = ( this_rref_t rhv ) {
                line_infos = std::move( rhv.line_infos ) ;
                group_id = rhv.group_id ;
                proj = rhv.proj ;
                view = rhv.view ;
                line_width = rhv.line_width ;
                return *this ;
            }

            bool_t operator == ( this_cref_t rhv ) const {
                return group_id == group_id ;
            }
        };
        so_typedef( group_info ) ;
        so_typedefs( so_std::vector< group_info_ptr_t >, group_infos ) ;

    private:

        so_gfx::line_render_2d_plug_factory_ptr_t _fac_ptr = nullptr ;

        so_gfx::line_render_2d_shared_data_ptr_t _sd_ptr = nullptr ;

        so_gpx::render_system_ptr_t _gpxr = nullptr ;

        so_gpx::technique_id_t _t_rnd = so_gpx::technique_id_t( -1 ) ;

        so_thread::mutex_t _mtx_group ;
        group_infos_t _group_infos ;

        so_typedefs( so_std::vector< size_t >, render_group_ids ) ;
        render_group_ids_t _render_groups ;

    private: 

        group_info_ptr_t find_or_create_group_info( size_t const ) ;

    public:

        line_render_2d( so_gpx::render_system_ptr_t ) ;
        line_render_2d( this_cref_t ) = delete ;
        line_render_2d( this_rref_t ) ;
        ~line_render_2d( void_t ) ;

    public:

        static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
        static void_t destroy( this_ptr_t ) ;

        void_t init( void_t ) ;

        void_t set_view_projection( so_math::mat4f_cref_t view, so_math::mat4f_cref_t proj ) ;

    public: // draw
        
        void_t prepare_for_rendering( void_t ) ;
        bool_t need_to_render( size_t const ) const ;

        typedef std::function< line_info_t ( size_t const ) > draw_line_funk_t ;
        void_t draw_lines( size_t const, size_t const, draw_line_funk_t ) ;

        /// per group line width. this is a temporary function until 
        /// a per line - line width is implemented
        void_t set_line_width( size_t const, float_t const ) ;

    public: // render

        void_t render( size_t const ) ;
        void_t release( void_t ) ;

    };
    so_typedef( line_render_2d ) ;
}