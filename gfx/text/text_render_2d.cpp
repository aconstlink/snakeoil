//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "text_render_2d.h"
#include "text_render_2d_plug_factory.h"

#include <snakeoil/gpx/system/render_system.h>

#include <snakeoil/font/glyph_atlas/glyph_atlas.h>
#include <snakeoil/font/glyph_atlas/stb/stb_glyph_atlas_creator.h>

#include <snakeoil/log/log.h>

using namespace so_gfx ;

//************************************************************************************
text_render_2d::text_render_2d( canvas_info_cref_t ci, so_gpx::render_system_ptr_t rsptr )
{
    _ci = ci ;
    _gpxr = rsptr ;

    _sd_ptr = so_gfx::memory::alloc( so_gfx::text_render_2d_shared_data_t(), 
        "[text_render_2d::text_render_2d] : shared data" ) ;

    
    _fac_ptr = so_gfx::text_render_2d_plug_factory_t::create( 
        so_gfx::text_render_2d_plug_factory_t( _sd_ptr ),
        "[text_render_2d::text_render_2d] : plug factory" ) ;
}

//************************************************************************************
text_render_2d::text_render_2d( this_rref_t rhv )
{
    _ci = rhv._ci ;
    
    so_move_member_ptr( _gaptr, rhv ) ;
    so_move_member_ptr( _gpxr, rhv ) ;
    so_move_member_ptr( _sd_ptr, rhv ) ;
    so_move_member_ptr( _fac_ptr, rhv ) ;
}

//************************************************************************************
text_render_2d::~text_render_2d( void_t )
{
    if( _t_rnd != so_gpx::technique_id_t( -1 ) )
    {
        _gpxr->unregister_technique( _t_rnd ) ;
    }

    so_gfx::text_render_2d_plug_factory_t::destroy( _fac_ptr ) ;

    if( so_core::is_not_nullptr( _sd_ptr ) )
        so_gfx::memory::dealloc( _sd_ptr ) ;

    so_font::glyph_atlas_t::destroy( _gaptr ) ;
}

//************************************************************************************
text_render_2d::this_ptr_t text_render_2d::create( this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_gfx::memory::alloc( std::move( rhv ), p ) ;
}

//************************************************************************************
void_t text_render_2d::destroy( this_ptr_t ptr )
{
    so_gfx::memory::dealloc( ptr ) ;
}

//************************************************************************************
void_t text_render_2d::init_fonts( std::vector< so_io::path_t > const & paths )
{
    so_font::so_stb::glyph_atlas_creator_t::face_infos_t fis ;
    {
        size_t i = 0 ;
        for( auto const & p : paths )
        {
            so_font::so_stb::glyph_atlas_creator_t::face_info fi ;
            fi.id = i++ ;
            fi.path = p ;

            fis.push_back( fi ) ;
        }
    }

    so_std::vector<so_font::utf32_t> cps ;
    for( uint32_t i = 32; i < 127; ++i )
        cps.push_back( i ) ;
    
    for( uchar_t ch : "הצü" )
    {
        cps.push_back( so_font::utf32_t( ch ) ) ;
    }

    size_t const atlas_width = 1024 ;
    size_t const atlas_height = 1024 ;

    _gaptr = so_font::so_stb::glyph_atlas_creator_t::create_glyph_atlas( fis, 20,
        cps, atlas_width, atlas_height ) ;

    _t_rnd = _gpxr->register_technique( _fac_ptr ) ;

    _sd_ptr->ga = _gaptr ;
}

//************************************************************************************
so_gfx::result text_render_2d::draw_text( size_t const layer, size_t const font_id, size_t const point_size,
    so_math::vec2ui_cref_t spos, so_std::string_cref_t text )
{
    // 1. transform text to glyphs
    // 2. add text to buffer

    layer_infos_t::iterator cur_layer_iter ;
    {
        so_thread::lock_guard_t lk( _mtx_lis ) ;
        auto iter = std::find_if( _lis.begin(), _lis.end(), [&]( this_t::layer_info_cref_t li )
        { 
            return  li.layer_id == layer ;
        } ) ;
        cur_layer_iter = iter ;

        if( iter == _lis.end() )
        {
            layer_info_t li ;
            li.layer_id = layer ;
            
            auto const lower_iter = std::lower_bound( _lis.begin(), _lis.end(), layer,
                [&]( this_t::layer_info_cref_t li, size_t const val  )
            { 
                return li.layer_id < val;
            } ) ;
            cur_layer_iter = _lis.insert( lower_iter, li ) ;
        }
    }

    so_math::vec2f_t start_pos( -0.8f, -0.8f ) ;

    so_std::vector< glyph_info_t > gis ;
    for( char const & c : text )
    {
        so_font::glyph_atlas_t::glyph_info_t gi ;
        size_t buffer_offset = 0 ;

        // 1. find glyph
        auto const res = _gaptr->find_glyph( font_id, so_font::utf32_t(c),
            buffer_offset, gi ) ;

        // 1.1 find alternative glyph
        if( so_core::is_not( res ) )
        {
            so_log::log::warning( "[text_render_2d::draw_text] : glyph not found for : " + c ) ;
            
            auto const ires = _gaptr->find_glyph( font_id, so_font::utf32_t( '?' ),
                buffer_offset, gi ) ;

            so_log::log::error_and_exit( so_core::is_not( ires), 
                "[text_render_2d::draw_text] : glyph ? must be included" ) ;
        }

        // 2. compute position
        so_math::vec2f_t const pos = start_pos + so_math::vec2f_t( 0.1f, 0.1f ) ;
        
        // 3. store
        this_t::glyph_info_t this_gi ;
        this_gi.pos = pos ;
        this_gi.offset = buffer_offset ;
        this_gi.color = so_math::vec3f_t( 1.0f ) ;
        gis.push_back( this_gi ) ;

        start_pos = pos ;
    }

    {
        layer_info_ref_t li = *cur_layer_iter ;

        so_thread::lock_guard_t lk( li.mtx ) ;
        

        for( auto const & gi : gis )
            li.glyph_infos.push_back( gi ) ;
    }

    return so_gfx::ok ;
}

//************************************************************************************
so_gfx::result text_render_2d::render( void_t )
{
    // 1. clear out the shared data
    {
        _sd_ptr->glyph_infos.resize( 0 ) ;
        _sd_ptr->per_layer_infos.resize( 0 ) ;
    }

    // 2. refill glyph shared data buffers
    // 3. clear the layer's glyph draw buffers
    {
        size_t lid = 0 ;
        for( auto & p : _lis )
        {
            layer_info_ref_t li = p ;

            so_thread::lock_guard_t lk( li.mtx ) ;

            // store glyphs in shared data buffer
            for( auto const gi : li.glyph_infos )
            {
                text_render_2d_shared_data_t::glyph_info_t igi ;
                igi.color = gi.color ;
                igi.offset = gi.offset ;
                igi.pos = gi.pos ;
                _sd_ptr->glyph_infos.push_back( igi ) ;
            }
            
            // do per layer info
            {
                text_render_2d_shared_data_t::per_layer_info_t pli ;
                pli.layer_id = lid++ ;
                pli.num_glyphs = li.glyph_infos.size() ;

                _sd_ptr->per_layer_infos.push_back( pli ) ;
            }

            li.glyph_infos.resize( 0 ) ;
        }
    }

    // 4. schedule techniques
    {
        _gpxr->schedule( _t_rnd ) ;
    }

    // 1. transform glyph infos and store in gpu buffer
    // 2. schedule technique for layer
    // 3. empty local glyph buffers

    return so_gfx::ok ;
}

//************************************************************************************