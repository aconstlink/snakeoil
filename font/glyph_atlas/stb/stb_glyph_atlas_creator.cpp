//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "stb_glyph_atlas_creator.h"

#include <snakeoil/io/io.h>
#include <snakeoil/log/log.h>

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb/stb_truetype.h>

using namespace so_font ;
using namespace so_font::so_stb ;

glyph_atlas_ptr_t glyph_atlas_creator::create_glyph_atlas( face_infos_cref_t face_infos, size_t const pixel_height,
    code_points_cref_t cps, size_t const max_img_width, size_t const max_img_height )
{
    // will be returned
    glyph_atlas_t ga ;

    struct font_data
    {
        size_t face_id ;
        uchar_ptr_t raw_data = nullptr ;
        stbtt_fontinfo font ;
    };
    so_typedef( font_data ) ;
    so_typedefs( so_std::vector<font_data_t>, font_datas ) ;

    font_datas_t fonts ;

    so_std::vector< so_io::load_handle_t > load_handles ;

    // load font
    {
        for( auto const & idp : face_infos )
        {
            load_handles.push_back( so_io::io::load( idp.path ) ) ;
        }

        for( auto & lh : load_handles )
        {
            auto const res = lh.wait_for_operation( [&]( char_cptr_t din, size_t sib, so_io::result res )
            {
                if( so_io::no_success( res ) ) return ;

                stbtt_fontinfo font ;
                uchar_ptr_t raw_data = so_memory::memory::alloc_raw<uchar_t>( sib ) ;
                memcpy( raw_data, din, sib ) ;

                int const stb_res = stbtt_InitFont( &font, raw_data,
                    stbtt_GetFontOffsetForIndex( raw_data, 0 ) ) ;

                so_log::log::error_and_exit( stb_res == 0,
                    "[so_stb::glyph_atlas_creator::create_glyph_atlas] : stbtt_InitFont" ) ;

                font_data_t fd ;
                fd.face_id = face_infos[fonts.size()].id ;
                fd.font = font ;
                fd.raw_data = raw_data ;
                fonts.push_back( fd ) ;
            } ) ;

            if( so_io::no_success( res ) )
                return nullptr ;
        }
    }

    struct internal_glyph_info
    {
        size_t id ;
        size_t face_id ;
        uint32_t cp ;
        int width ;
        int height ;
        int voffset;
    };
    so_std::vector< internal_glyph_info > internal_glyh_infos ;

    {
        size_t index = 0 ;
        for( auto const & fd : fonts )
        {
            stbtt_fontinfo const & font = fd.font ;

            float s = stbtt_ScaleForPixelHeight( &font, ( float_t ) pixel_height );
            
            int ascent, descent, linegap ;
            stbtt_GetFontVMetrics(&font, &ascent, &descent, &linegap ) ;

            for( auto const i : cps )
            {
                int x_0, x_1, y_0, y_1 ;
                stbtt_GetCodepointBitmapBox( &font, i, s, s,
                    &x_0, &y_0, &x_1, &y_1 );

                int w = x_1 - x_0 ;
                int h = y_1 - y_0 ;

                if( w == 0 || h == 0 ) continue ;

                int cpb_x0, cpb_y0, cpb_x1, cpb_y1 ;
                stbtt_GetCodepointBox( &font, i, &cpb_x0, &cpb_y0, &cpb_x1, &cpb_y1 ) ;

                internal_glyph_info bb ;
                bb.id = index ;
                bb.face_id = fd.face_id ;
                bb.cp = i ;
                bb.width = w ;
                bb.height = h ;
                bb.voffset = cpb_y0 ;
                internal_glyh_infos.push_back( bb ) ;
            }
            ++index ;
        }
    }

    std::sort( internal_glyh_infos.begin(), internal_glyh_infos.end(), 
        [&]( internal_glyph_info const & a, internal_glyph_info const & b )
    {
        return a.height < b.height ;
    } ) ;

    size_t const iw = max_img_width ;
    size_t const ih = max_img_height ;

    glyph_atlas_t::image_t img( iw, ih ) ;

    size_t start_x = 0 ;
    size_t start_y = 0 ;
    size_t line_height = 0 ;

    size_t channel = 0 ;
    for( auto const & bb : internal_glyh_infos )
    {
        stbtt_fontinfo const & font = fonts[bb.id].font ;
        
        float const s = stbtt_ScaleForPixelHeight( &font, ( float_t ) pixel_height );

        int w, h, xo, yo ;
        uchar_ptr_t bitmap = stbtt_GetCodepointBitmap(
            &font, s, s, bb.cp, &w, &h, &xo, &yo ) ;

        if( so_core::is_nullptr( bitmap ) )
            continue ;

        if( start_x + w > iw )
        {
            start_x = 0 ;
            start_y += line_height + 1 ;
            line_height = h ;
        }
        if( start_y + h > ih )
        {
            start_y = 0 ;
            ++channel ;
        }
        if( channel == 4 )
            break ;

        if( channel > ga.get_num_images() )
        {
            ga.add_image( std::move( img ) ) ;
            img = glyph_atlas_t::image_t(iw, ih) ;
        }

        for( size_t y = 0; y < h; ++y )
        {
            for( size_t x = 0; x < w; ++x )
            {
                size_t const cur_y = y + start_y ;
                size_t const cur_x = x + start_x ;

                size_t const tw = cur_y * iw + cur_x ;
                size_t const tr = ( h - 1 - y ) * w + x ;

                img.get_plane()[ tw ] = bitmap[ tr ] ;
            }
        }
        
        {
            glyph_atlas_t::glyph_info_t gi ;
            gi.face_id = bb.face_id ;
            gi.code_point = bb.cp ;
            gi.image_id = channel ;
            gi.start = so_math::vec2f_t( float_t(start_x) / float_t(iw), float_t(start_y) / float_t(ih) ) ;
            gi.dims = so_math::vec2f_t( float_t(w) / float_t(iw), float_t(h) / float_t(ih) ) ;
            
            {
                int adv, bearing ;
                stbtt_GetCodepointHMetrics( &font, bb.cp, &adv, &bearing ) ;
                
                gi.adv = so_math::vec2f_t((float_t(adv-bearing) / float_t(iw))*s, gi.dims.y() ) ;
            }
            gi.bearing = (float_t(bb.voffset) / float_t(iw))*s ;

            ga.add_glyph(gi) ;
        }

        start_x += w + 1;
        line_height = std::max( size_t( h ), line_height ) ;

        stbtt_FreeBitmap( bitmap, nullptr ) ;
    }

    if( so_core::is_not_nullptr( img.get_plane() ) )
    {
        ga.add_image( std::move( img ) ) ;
    }

    for( auto & fd : fonts )
    {
        so_font::memory::dealloc_raw( fd.raw_data ) ;
    }
    

    return glyph_atlas_t::create( std::move( ga ), 
        "[glyph_atlas_creator::create_glyph_atlas] : glyph_atlas" ) ;
}