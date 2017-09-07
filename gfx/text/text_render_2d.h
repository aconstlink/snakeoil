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

namespace so_gfx
{
    class SNAKEOIL_GFX_API text_render_2d
    {
        so_this_typedefs( text_render_2d ) ;

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

    private:

        struct glyph_info
        {
            size_t offset ;
            so_math::vec2f_t pos ;
            so_math::vec3f_t color ;
        };
        so_typedef( glyph_info ) ;

        struct layer_info
        {
            so_this_typedefs( layer_info ) ;

            so_thread::mutex_t mtx ;
            so_std::vector< glyph_info > glyph_infos ;

            size_t layer_id = 0 ;

            layer_info( void_t ){}
            layer_info( this_cref_t rhv ){
                glyph_infos = rhv.glyph_infos ;
                layer_id = rhv.layer_id ;
            }
            layer_info( this_rref_t rhv ) {
                glyph_infos = std::move( rhv.glyph_infos ) ;
                layer_id = rhv.layer_id ;
            }
            ~layer_info( void_t ) {}

            this_ref_t operator = ( this_cref_t rhv ) {
                glyph_infos = rhv.glyph_infos ;
                layer_id = rhv.layer_id ;
                return *this ;
            }
            this_ref_t operator = ( this_rref_t rhv ) {
                glyph_infos = std::move( rhv.glyph_infos ) ;
                layer_id = rhv.layer_id ;
                return *this ;
            }

            bool_t operator == ( this_cref_t rhv ) const{
                return layer_id == layer_id ;
            }
        };
        so_typedef( layer_info ) ;
        so_typedefs( so_std::vector< layer_info >, layer_infos ) ;

        so_thread::mutex_t _mtx_lis ;
        layer_infos_t _lis ;

    public:

        text_render_2d( canvas_info_cref_t, so_gpx::render_system_ptr_t ) ;
        text_render_2d( this_cref_t ) = delete ;
        text_render_2d( this_rref_t ) ;
        ~text_render_2d( void_t ) ;

    public:

        static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
        static void_t destroy( this_ptr_t ) ;

    public:

        void_t init_fonts( std::vector< so_io::path_t > const & ) ;

        so_gfx::result draw_text( size_t const layer, size_t const font_id, size_t const point_size,
            so_math::vec2ui_cref_t pos, so_std::string_cref_t ) ;

        so_gfx::result render( void_t ) ;
    };
    so_typedef( text_render_2d ) ;
}

#endif