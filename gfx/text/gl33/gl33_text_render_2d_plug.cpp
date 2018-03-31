//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "gl33_text_render_2d_plug.h"
#include "gfx_config.h"

#include <snakeoil/gpu/api/gl/igl_33_api.h>
#include <snakeoil/gpu/buffer/vertex_buffer.hpp>
#include <snakeoil/gpu/buffer/index_buffer.hpp>
#include <snakeoil/gpu/buffer/data_buffer.hpp>
#include <snakeoil/gpu/shader/pixel_shader.h>
#include <snakeoil/gpu/shader/vertex_shader.h>
#include <snakeoil/gpu/program/program.h>
#include <snakeoil/gpu/program/config.h>
#include <snakeoil/gpu/variable/variable_set.h>
#include <snakeoil/gpu/framebuffer/framebuffer_2d.h>
#include <snakeoil/gpu/image/color_image_2d.hpp>
#include <snakeoil/gpu/texture/texture_2d.h>
#include <snakeoil/gpu/texture/depth_texture_2d.h>

#include <snakeoil/font/glyph_atlas/glyph_atlas.h>

#include <snakeoil/io/global.h>
#include <snakeoil/log/global.h>

#include <snakeoil/math/vector/vector4.hpp>

using namespace so_gfx ;
using namespace so_gfx::so_gl33 ;

//************************************************************************************
gl33_text_render_2d_plug::gl33_text_render_2d_plug( 
    so_gfx::text_render_2d_shared_data_ptr_t sd_ptr, so_gpu::so_gl::igl_33_api_ptr_t aptr ) : this_base_t( aptr )
{
    _sd = sd_ptr ;

    _vb_text = vb_t::create( "vertex buffer" ) ;
    _ib_text = ib_t::create( "index buffer" ) ;
    _vs_text = so_gpu::vertex_shader_t::create( "vertex shader" ) ;
    _ps_text = so_gpu::pixel_shader_t::create( "pixel shader" ) ;
    _prog_text = so_gpu::program_t::create( "program" ) ;
    _config_text = so_gpu::config_t::create( "config" ) ;
}

//************************************************************************************
gl33_text_render_2d_plug::gl33_text_render_2d_plug( this_rref_t rhv ) : this_base_t( std::move( rhv ) )
{
    so_move_member_ptr( _sd, rhv ) ;

    so_move_member_ptr( _gpu_img_ptr, rhv ) ;
    so_move_member_ptr( _tx, rhv ) ;

    so_move_member_ptr( _vb_text, rhv ) ;
    so_move_member_ptr( _ib_text, rhv ) ;
    so_move_member_ptr( _vs_text, rhv ) ;
    so_move_member_ptr( _ps_text, rhv ) ;
    so_move_member_ptr( _prog_text, rhv ) ;
    so_move_member_ptr( _config_text, rhv ) ;

    so_move_member_ptr( _glyph_info_ptr, rhv ) ;
    so_move_member_ptr( _text_info_ptr, rhv ) ;

    _per_group_datas = std::move( rhv._per_group_datas ) ;
}

//************************************************************************************
gl33_text_render_2d_plug::~gl33_text_render_2d_plug( void_t )
{
    if( so_core::is_not_nullptr( _vb_text ) )
        _vb_text->destroy() ;

    if( so_core::is_not_nullptr( _ib_text ) )
        _ib_text->destroy() ;

    if( so_core::is_not_nullptr( _vs_text ) )
        so_gpu::vertex_shader_t::destroy( _vs_text ) ;

    if( so_core::is_not_nullptr( _ps_text ) )
        so_gpu::pixel_shader_t::destroy( _ps_text ) ;

    if( so_core::is_not_nullptr( _prog_text ) )
        so_gpu::program_t::destroy( _prog_text ) ;

    if( so_core::is_not_nullptr( _config_text ) )
        so_gpu::config_t::destroy( _config_text ) ;

    if( so_core::is_not_nullptr( _gpu_img_ptr ) )
        _gpu_img_ptr->destroy() ;

    if( so_core::is_not_nullptr( _tx ) )
        _tx->destroy() ;

    if( so_core::is_not_nullptr( _glyph_info_ptr ) )
        _glyph_info_ptr->destroy() ;

    if( so_core::is_not_nullptr( _text_info_ptr ) )
        _text_info_ptr->destroy() ;

    for( auto & item : _per_group_datas )
    {
        item.var_set->destroy() ;
        so_gpu::memory::dealloc( item.start_offset ) ;
        so_gpu::memory::dealloc( item.proj ) ;
        so_gpu::memory::dealloc( item.view ) ;
    }
}

//************************************************************************************
gl33_text_render_2d_plug::this_ptr_t gl33_text_render_2d_plug::create( 
    this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_gfx::memory::alloc( std::move( rhv ), p ) ;
}

//************************************************************************************
void_t gl33_text_render_2d_plug::destroy( this_ptr_t ptr )
{
    so_gfx::memory::dealloc( ptr ) ;
}

//************************************************************************************
void_t gl33_text_render_2d_plug::destroy( void_t )
{
    this_t::destroy( this ) ;
}

//*********************************************************************
so_gpx::plug_result gl33_text_render_2d_plug::on_load( load_info_cref_t li )
{
    if( li.reload )
    {
        so_log::global_t::warning( "[gl33_text_render_2d_plug::on_load] : reload not supported" ) ;
        return so_gpx::plug_result::failed ;
    }

    auto const base = so_gfx::find_shader_path( "/text/gl33/shader" ) ;

    // text render shader
    {
        auto const vs_path = base / so_io::path_t( "/text_render_2d.vs.glsl" ) ;
        auto const ps_path = base / so_io::path_t( "/text_render_2d.ps.glsl" ) ;

        so_io::load_handle_t vsh = so_io::global::load( so_io::path_t( vs_path ) ) ;
        so_io::load_handle_t psh = so_io::global::load( so_io::path_t( ps_path ) ) ;

        {
            so_std::string_t vs ;
            vsh.wait_for_operation( [&] ( char_cptr_t din, size_t sib, so_io::result res )
            {
                so_log::global::error_and_exit( so_io::no_success( res ),
                    "[gl33_text_render_2d_plug::on_load] : Vertex shader not found : " + vs_path.string() ) ;

                vs = so_std::string_t( din, sib ) ;
            } ) ;
            _vs_text->set_code( vs ) ;
        }

        {
            so_std::string_t ps ;
            psh.wait_for_operation( [&] ( char_cptr_t din, size_t sib, so_io::result res )
            {
                so_log::global::error_and_exit( so_io::no_success( res ),
                    "[gl33_text_render_2d_plug::on_load] : Pixel shader not found : " + ps_path.string() ) ;

                ps = so_std::string_t( din, sib ) ;
            } ) ;
            _ps_text->set_code( ps ) ;
        }
    }

    {
        _vb_text->add_layout_element<so_math::vec3f_t>( so_gpu::vertex_attribute::position ) ;

        size_t const num_glyph_quads = _num_quads ;

        for( size_t i = 0; i<num_glyph_quads; ++i )
        {
            _vb_text->add_vertex( vertex_t { so_math::vec3f_t( -0.5f, -0.5f, 0.0f ) } ) ;
            _vb_text->add_vertex( vertex_t { so_math::vec3f_t( -0.5f, 0.5f, 0.0f ) } ) ;
            _vb_text->add_vertex( vertex_t { so_math::vec3f_t( 0.5f, 0.5f, 0.0f ) } ) ;
            _vb_text->add_vertex( vertex_t { so_math::vec3f_t( 0.5f, -0.5f, 0.0f ) } ) ;
        }

        for( size_t i = 0; i<num_glyph_quads; ++i )
        {
            uint_t const index = uint_t( i ) * 4 ;

            _ib_text->add_index( index + 0 ) ;
            _ib_text->add_index( index + 1 ) ;
            _ib_text->add_index( index + 2 ) ;

            _ib_text->add_index( index + 0 ) ;
            _ib_text->add_index( index + 2 ) ;
            _ib_text->add_index( index + 3 ) ;
        }
    }

    // copy glyph image to gpu image
    {
        size_t const width = _sd->ga->get_width() ;
        size_t const height = _sd->ga->get_height() ;

        this_t::image_t gpu_img( width, height ) ;

        for( size_t i = 0; i < _sd->ga->get_num_images(); ++i )
        {
            auto * img_ptr = _sd->ga->get_image( i ) ;

            so_math::vec4uc_ptr_t wimg_ptr = ( so_math::vec4uc_ptr_t )gpu_img.get_raw_pointer() ;
            uint8_cptr_t rimg_ptr = img_ptr->get_plane() ;

            for( int y = 0; y < height; ++y )
            {
                for( int x = 0; x < width; ++x )
                {
                    size_t const index = y * width + x ;
                    wimg_ptr[ index ][ i ] = rimg_ptr[ index ] ;
                }
            }
        }

        _gpu_img_ptr = this_t::image_t::create( std::move( gpu_img ),
            "[my_gl_33_plug] : gpu img plane" ) ;
    }

    // copy glyph info
    {
        data_buffer_t db( _sd->ga->get_num_glyphs() << 1 ) ;


        for( size_t i = 0; i < _sd->ga->get_num_glyphs(); ++i )
        {
            so_font::glyph_atlas_t::glyph_info_t gi ;
            auto const res = _sd->ga->get_glyph_info( i, gi ) ;
            if( so_core::is_not( res ) )
                continue ;

            auto const v1 = so_math::vec4f_t( gi.start, gi.dims ) ;
            auto const v2 = so_math::vec4f_t( float_t( gi.image_id ), gi.bearing, 0.0f, 0.0f ) ;

            size_t const index = i << 1 ;

            db.set_element( index + 0, v1 ) ;
            db.set_element( index + 1, v2 ) ;

        }

        _glyph_info_ptr = data_buffer_t::create( std::move( db ),
            "[my_gl_33_plug] : glyph_info" ) ;
    }

    data_buffer_t db_text ;
    _text_info_ptr = data_buffer_t::create( std::move( db_text ),
        "[my_gl_33_plug] : text info" ) ;

    return so_gpx::plug_result::ok ;
}

//*********************************************************************
so_gpx::plug_result gl33_text_render_2d_plug::on_unload( void_t ) {
    return so_gpx::plug_result::ok ;
}

//*********************************************************************
so_gpx::plug_result gl33_text_render_2d_plug::on_initialize( so_gpx::iplug_t::init_info_cref_t )
{
    {
        {
            auto const res = this_t::api()->create_buffer( _vb_text ) ;
        }
        {
            auto const res = this_t::api()->create_buffer( _ib_text ) ;
        }
        {
            auto const res = this_t::api()->create_shader( _vs_text ) ;
        }
        {
            auto const res = this_t::api()->compile( _vs_text )  ;
        }

        {
            auto const res = this_t::api()->create_shader( _ps_text ) ;
        }
        {
            auto const res = this_t::api()->compile( _ps_text )  ;
        }
        {
            auto const res = this_t::api()->create_program( _prog_text ) ;
        }
        {
            _prog_text->set_shader( _vs_text ) ;
            _prog_text->set_shader( _ps_text ) ;
        }
        {
            auto const res = this_t::api()->link( _prog_text ) ;
        }
        {
            auto const res = this_t::api()->create_config( _config_text ) ;
        }
        {
            _config_text->set_program( _prog_text ) ;
            _config_text->bind( so_gpu::vertex_attribute::position, "in_pos" ) ;
            _config_text->bind( so_gpu::primitive_type::triangles, _vb_text, _ib_text ) ;
        }
    }

    {
        auto const res = this_t::api()->create_image( _gpu_img_ptr ) ;
    }

    {
        _tx = so_gpu::texture_2d_t::create( so_gpu::texture_2d_t( _gpu_img_ptr ),
            "glyph texture" ) ;

        auto const res = this_t::api()->create_texture( _tx ) ;
    }

    {
        auto const res = this_t::api()->create_buffer( _glyph_info_ptr ) ;
    }

    {
        auto const res = this_t::api()->create_buffer( _text_info_ptr ) ;
    }

    //
    // memory allocation
    //
    {
        auto const res = this_t::api()->alloc_buffer_memory( _vb_text,
            so_gpu::memory_alloc_info( true ) ) ;
    }
    {
        auto const res = this_t::api()->alloc_buffer_memory( _ib_text,
            so_gpu::memory_alloc_info( true ) ) ;
    }

    // glyph image and glyph info buffer do not change if the 
    // atlas is not recreated.
    {
        auto const res = this_t::api()->alloc_image_memory( _gpu_img_ptr,
            so_gpu::image_alloc_info( true ) ) ;
    }
    {
        auto const res = this_t::api()->alloc_buffer_memory( _glyph_info_ptr,
            so_gpu::memory_alloc_info_t( true ) ) ;
    }

    return so_gpx::plug_result::ok ;
}

//*********************************************************************
so_gpx::plug_result gl33_text_render_2d_plug::on_release( void_t )
{
    this_t::api()->release_buffer( _vb_text ) ;
    this_t::api()->release_buffer( _ib_text ) ;
    this_t::api()->release_shader( _vs_text ) ;
    this_t::api()->release_shader( _ps_text ) ;
    this_t::api()->release_program( _prog_text ) ;
    this_t::api()->release_config( _config_text ) ;
    //this_t::api()->release_variable( _vars_text ) ;

    this_t::api()->release_image( _gpu_img_ptr ) ;
    this_t::api()->release_texture( _tx ) ;
    this_t::api()->release_buffer( _glyph_info_ptr ) ;
    this_t::api()->release_buffer( _text_info_ptr ) ;

    for( auto & item : _per_group_datas )
    {
        this_t::api()->release_variable( item.var_set ) ;
    }

    return so_gpx::plug_result::ok ;
}

//*********************************************************************
so_gpx::plug_result gl33_text_render_2d_plug::on_update( update_info_cref_t )
{
    // 1. copy the data
    {
        _text_info_ptr->resize( 0 ) ;

        // copy all glyph data
        {
            for( auto const & gi : _sd->glyph_infos )
            {
                float_t const offset = float_t( gi.offset ) ;
                so_math::vec3f_t const color = gi.color ;
                so_math::vec2f_t const pos = gi.pos ;
                float_t const pss = gi.point_size_scale ;

                _text_info_ptr->add_element( so_math::vec4f_t( offset, pos.x(), pos.y(), pss ) ) ;
                _text_info_ptr->add_element( so_math::vec4f_t( color, 0.0f ) ) ;
            }
        }

        // 1. copy all per group data
        // 2. calc start offset
        {
            uint32_t offset = 0 ;
            for( auto const & gi : _sd->per_group_infos )
            {
                auto iter = std::find_if( _per_group_datas.begin(), _per_group_datas.end(), 
                    [&]( this_t::per_group_data_cref_t pgd ) 
                { 
                    return pgd.group_id == gi.group_id ;
                } ) ;
                
                if( iter == _per_group_datas.end() )
                {
                    this_t::per_group_data_t pgd ;
                    pgd.group_id = gi.group_id ;

                    pgd.var_set = so_gpu::variable_set_t::create(
                        "[gl33_text_render_2d_plug::on_update] : variable_set") ;

                    this_t::api()->create_variable( pgd.var_set ) ;

                    // configure variable set
                    {
                        // required for rendering
                        pgd.varset_id = _config_text->get_num_varsets() ;
                        _config_text->add_variable_set( pgd.var_set ) ;

                        pgd.var_set->bind_buffer<so_math::vec4f_t>( "u_glyph_info", _glyph_info_ptr ) ;
                        pgd.var_set->bind_buffer<so_math::vec4f_t>( "u_text_info", _text_info_ptr ) ;
                        
                        pgd.var_set->bind_texture( "u_smp_glyph", _tx ) ;
                        
                        pgd.var_set->bind_data<so_math::vec2f_t>( "u_scale", &( _sd->dim_scale ) ) ;
                        
                        {
                            pgd.proj = so_gpu::memory::alloc<so_math::mat4f_t>( gi.proj,
                                "[gl33_text_render_2d_plug::on_update] : projection matrix" ) ;
                            pgd.var_set->bind_data<so_math::mat4f_t>( "u_proj", pgd.proj ) ;
                        }

                        {
                            pgd.view = so_gpu::memory::alloc<so_math::mat4f_t>( gi.view,
                                "[gl33_text_render_2d_plug::on_update] : view matrix" ) ;
                            pgd.var_set->bind_data<so_math::mat4f_t>( "u_view", pgd.view ) ;
                        }

                        {
                            pgd.start_offset = so_gpu::memory::alloc<uint32_t>(
                                "[gl33_text_render_2d_plug::on_update] : start_offset") ;
                            pgd.var_set->bind_data<uint32_t>( "u_offset", pgd.start_offset ) ;
                        }
                    }

                    iter = _per_group_datas.insert( _per_group_datas.end(), pgd ) ;
                }

                // need to be updated every call
                {
                    iter->num_elements = uint32_t( gi.num_glyphs ) ;
                    ( *iter->start_offset ) = offset ;
                    ( *iter->proj ) = gi.proj ;
                    ( *iter->view ) = gi.view ;
                    offset += iter->num_elements ;
                }
            }
        }
    }

    // 2. transfer to gpu
    // @todo optimize. Do not always reallocate
    {
        auto const res = this_t::api()->alloc_buffer_memory( _text_info_ptr,
            so_gpu::memory_alloc_info_t( true ) ) ;
    }

    return so_gpx::plug_result::ok ;
}

//*********************************************************************
so_gpx::plug_result gl33_text_render_2d_plug::on_execute( so_gpx::iplug_t::execute_info_cref_t ri )
{
    auto iter = std::find_if( _per_group_datas.begin(), _per_group_datas.end(), 
        [&]( this_t::per_group_data_cref_t pgd ){ return pgd.group_id == ri.rnd_id ; } ) ;

    // nothing to do. Could be an error
    if( iter == _per_group_datas.end() )
    {
        so_log::global::warning( "[gl33_text_render_2d_plug::on_execute] : "
            "rendering an unused group id is inefficient" ) ;
        return so_gpx::plug_result::ok ;
    }

    if( iter->num_elements == 0 )
    {
        // basically also inefficient
        return so_gpx::plug_result::ok ;
    }

    // @todo save state

    this_t::api()->enable( so_gpu::render_state::blend ) ;
    this_t::api()->set_state( so_gpu::blend_factor::one, so_gpu::blend_factor::one_minus_src_alpha ) ;
    {
        this_t::api()->load_variable( iter->var_set ) ;
        this_t::api()->execute( so_gpu::render_config_info( _config_text, iter->varset_id, 0,
            (iter->num_elements) * 6 ) ) ;
    }
    this_t::api()->disable( so_gpu::render_state::blend ) ;

    // @todo restore state

    return so_gpx::plug_result::ok ;
}