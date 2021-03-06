//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GFX_TEXT_TEXT_RENDER_2D_H_
#define _SNAKEOIL_GFX_TEXT_TEXT_RENDER_2D_H_

#include "../protos.h"
#include "../result.h"
#include "../api.h"

#include "text_render_2d_shared_data.h"

#include <snakeoil/gpx/protos.h>
#include <snakeoil/gpx/window/window_id.h>
#include <snakeoil/gpx/technique/technique_id.h>
#include <snakeoil/gpu/viewport/viewport_2d.h>

#include <snakeoil/font/protos.h>

#include <snakeoil/io/typedefs.h>

#include <snakeoil/thread/mutex.h>
#include <snakeoil/std/string/string.hpp>
#include <snakeoil/std/container/vector.hpp>
#include <snakeoil/math/vector/vector2.hpp>
#include <snakeoil/math/vector/vector3.hpp>

#include <functional>

namespace so_gfx
{
    class SNAKEOIL_GFX_API text_render_2d
    {
        so_this_typedefs( text_render_2d ) ;

    public:

        so_typedefs( std::function< so_math::vec2f_t ( so_math::vec2f_cref_t ) >, offset_funk ) ;

    public:
        
        struct canvas_info
        {
            size_t dpi = 96 ;
            so_gpu::viewport_2d_t vp ;
        };
        so_typedef( canvas_info ) ;

    private:

        canvas_info _ci ;

        so_gpx::technique_id_t _t_rnd  = so_gpx::technique_id_t(-1) ;
        so_gfx::text_render_2d_plug_factory_ptr_t _fac_ptr = nullptr ;

        so_gfx::text_render_2d_shared_data_ptr_t _sd_ptr = nullptr ;

        so_font::glyph_atlas_ptr_t _gaptr = nullptr ;

        so_gpx::render_system_ptr_t _gpxr = nullptr ;

        size_t _point_size = 20 ;

        so_math::vec2f_t _glyph_atlas_size = so_math::vec2f_t( 1024.0f, 1024.0f ) ;

    private: // default matrices

        so_math::mat4f_t _proj ;
        so_math::mat4f_t _view ;

    private:

        struct glyph_info
        {
            size_t offset ;
            so_math::vec2f_t pos ;
            so_math::vec3f_t color ;

            float_t point_size_scale ;
        };
        so_typedef( glyph_info ) ;

        struct group_info
        {
            so_this_typedefs( group_info ) ;

            so_thread::mutex_t mtx ;
            so_std::vector< glyph_info > glyph_infos ;

            size_t group_id = 0 ;

            so_math::mat4f_t proj ;
            so_math::mat4f_t view ;

            group_info( void_t ){}
            group_info( this_cref_t rhv ){
                glyph_infos = rhv.glyph_infos ;
                group_id = rhv.group_id ;
                proj = rhv.proj ;
                view = rhv.view ;
            }
            group_info( this_rref_t rhv ) {
                glyph_infos = std::move( rhv.glyph_infos ) ;
                group_id = rhv.group_id ;
                proj = rhv.proj ;
                view = rhv.view ;
            }
            ~group_info( void_t ) {}

            this_ref_t operator = ( this_cref_t rhv ) {
                glyph_infos = rhv.glyph_infos ;
                group_id = rhv.group_id ;
                proj = rhv.proj ;
                view = rhv.view ;
                return *this ;
            }
            this_ref_t operator = ( this_rref_t rhv ) {
                glyph_infos = std::move( rhv.glyph_infos ) ;
                group_id = rhv.group_id ;
                proj = rhv.proj ;
                view = rhv.view ;
                return *this ;
            }

            bool_t operator == ( this_cref_t rhv ) const{
                return group_id == group_id ;
            }
        };
        so_typedef( group_info ) ;
        so_typedefs( so_std::vector< group_info >, group_infos ) ;

        so_thread::mutex_t _mtx_lis ;
        group_infos_t _gis ;

        so_typedefs( so_std::vector< size_t >, render_group_ids ) ;
        render_group_ids_t _render_groups ;

    public:

        text_render_2d( so_gpx::render_system_ptr_t ) ;
        text_render_2d( this_cref_t ) = delete ;
        text_render_2d( this_rref_t ) ;
        ~text_render_2d( void_t ) ;

    public:

        static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
        static void_t destroy( this_ptr_t ) ;

    public:

        void_t init_fonts( size_t const, std::vector< so_io::path_t > const & ) ;
        void_t init_fonts( size_t const, std::vector< so_io::path_t > const &, so_std::vector<so_font::utf32_t> const & ) ;
        
        void_t set_view_projection( so_math::mat4f_cref_t view, so_math::mat4f_cref_t proj ) ;
        void_t set_view_projection( size_t const, so_math::mat4f_cref_t view, so_math::mat4f_cref_t proj ) ;

        so_gfx::result draw_text( size_t const group, size_t const font_id, size_t const point_size,
            so_math::vec2f_cref_t pos, so_math::vec3f_cref_t color, so_std::string_cref_t ) ;

        so_gfx::result draw_text( size_t const group, size_t const font_id, size_t const point_size,
            so_math::vec2f_cref_t pos, offset_funk_t, so_math::vec3f_cref_t color, so_std::string_cref_t ) ;

        so_gfx::result set_canvas_info( canvas_info_cref_t ) ;
        so_gfx::result prepare_for_rendering( void_t ) ;
        bool_t need_to_render( size_t const ) const ;
        so_gfx::result render( size_t const ) ;

        so_gfx::result release( void_t ) ;

    private:

        this_t::group_infos_t::iterator insert_group( size_t const ) ;

    };
    so_typedef( text_render_2d ) ;
}

#endif