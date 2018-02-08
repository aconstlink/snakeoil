//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "gl33_rect_render_2d_plug.h"

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

#include <snakeoil/io/global.h>
#include <snakeoil/log/global.h>

#include <snakeoil/math/vector/vector4.hpp>

using namespace so_gfx ;
using namespace so_gfx::so_gl33 ;

//************************************************************************************
gl33_rect_render_2d_plug::gl33_rect_render_2d_plug( so_gfx::rect_render_2d_shared_data_ptr_t sd_ptr,
    so_gpu::so_gl::igl_33_api_ptr_t aptr ) : this_base_t( aptr )
{
    _sd = sd_ptr ;

    _vb = vb_t::create( "vertex buffer" ) ;
    _ib = ib_t::create( "index buffer" ) ;
    _vs = so_gpu::vertex_shader_t::create( "vertex shader" ) ;
    _ps = so_gpu::pixel_shader_t::create( "pixel shader" ) ;
    _prog = so_gpu::program_t::create( "program" ) ;
    _config = so_gpu::config_t::create( "config" ) ;
}

//************************************************************************************
gl33_rect_render_2d_plug::gl33_rect_render_2d_plug( this_rref_t rhv ) : this_base_t( std::move( rhv ) )
{
    so_move_member_ptr( _sd, rhv ) ;

    so_move_member_ptr( _vb, rhv ) ;
    so_move_member_ptr( _ib, rhv ) ;
    so_move_member_ptr( _vs, rhv ) ;
    so_move_member_ptr( _ps, rhv ) ;
    so_move_member_ptr( _prog, rhv ) ;
    so_move_member_ptr( _config, rhv ) ;

    so_move_member_ptr( _rect_info_ptr, rhv ) ;

    _per_group_datas = std::move( rhv._per_group_datas ) ;
}

//************************************************************************************
gl33_rect_render_2d_plug::~gl33_rect_render_2d_plug( void_t )
{
    this_t::vb_t::destroy( _vb ) ;
    this_t::ib_t::destroy( _ib ) ;
    this_t::data_buffer_t::destroy( _rect_info_ptr ) ;

    so_gpu::vertex_shader_t::destroy( _vs ) ;
    so_gpu::pixel_shader_t::destroy( _ps ) ;
    so_gpu::program_t::destroy( _prog ) ;
    so_gpu::config_t::destroy( _config ) ;

    for( auto * var_set : _var_sets )
    {
        so_gpu::variable_set_t::destroy( var_set ) ;
    }

    for( auto & item : _per_group_datas )
    {
        so_gpu::memory::dealloc( item.start_offset ) ;
        so_gpu::memory::dealloc( item.proj ) ;
        so_gpu::memory::dealloc( item.view ) ;
    }
}

//************************************************************************************
gl33_rect_render_2d_plug::this_ptr_t gl33_rect_render_2d_plug::create( this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_gfx::memory::alloc( std::move( rhv ), p ) ;
}

//************************************************************************************
void_t gl33_rect_render_2d_plug::destroy( this_ptr_t ptr )
{
    so_gfx::memory::dealloc( ptr ) ;
}

//************************************************************************************
so_gpx::plug_result gl33_rect_render_2d_plug::on_load( load_info_cref_t li )
{
    if( li.reload )
    {
        so_log::global_t::warning( "[gl33_rect_render_2d_plug::on_load] : reload not supported" ) ;
        return so_gpx::plug_result::failed ;
    }

    so_std::string_t const base( so_std::string_t( GFX_SOURCE_DIR ) + "/rect/gl33/shader" ) ;

    // text render shader
    {
        so_std::string_t const vs_path = base + so_std::string( "/rect_render_2d.vs.glsl" ) ;
        so_std::string_t const ps_path = base + so_std::string( "/rect_render_2d.ps.glsl" ) ;

        so_io::load_handle_t vsh = so_io::global::load( so_io::path_t( vs_path ) ) ;
        so_io::load_handle_t psh = so_io::global::load( so_io::path_t( ps_path ) ) ;

        {
            so_std::string_t vs ;
            vsh.wait_for_operation( [&] ( char_cptr_t din, size_t sib, so_io::result res )
            {
                so_log::global::error_and_exit( so_io::no_success( res ),
                    "[gl33_rect_render_2d_plug::on_load] : Vertex shader not found : " + vs_path ) ;

                vs = so_std::string_t( din, sib ) ;
            } ) ;
            _vs->set_code( vs ) ;
        }

        {
            so_std::string_t ps ;
            psh.wait_for_operation( [&] ( char_cptr_t din, size_t sib, so_io::result res )
            {
                so_log::global::error_and_exit( so_io::no_success( res ),
                    "[gl33_rect_render_2d_plug::on_load] : Pixel shader not found : " + ps_path ) ;

                ps = so_std::string_t( din, sib ) ;
            } ) ;
            _ps->set_code( ps ) ;
        }
    }

    {
        _vb->add_layout_element<so_math::vec3f_t>( so_gpu::vertex_attribute::position ) ;

        size_t const num_rects = _num_rects ;

        for( size_t i = 0; i < num_rects; ++i )
        {
            _vb->add_vertex( vertex_t { so_math::vec3f_t( -0.5f, -0.5f, 0.0f ) } ) ;
            _vb->add_vertex( vertex_t { so_math::vec3f_t( -0.5f, 0.5f, 0.0f ) } ) ;
            _vb->add_vertex( vertex_t { so_math::vec3f_t( 0.5f, 0.5f, 0.0f ) } ) ;
            _vb->add_vertex( vertex_t { so_math::vec3f_t( 0.5f, -0.5f, 0.0f ) } ) ;
        }

        for( size_t i = 0; i < num_rects; ++i )
        {
            uint_t const index = uint_t( i ) * 4 ;

            _ib->add_index( index + 0 ) ;
            _ib->add_index( index + 1 ) ;
            _ib->add_index( index + 2 ) ;

            _ib->add_index( index + 0 ) ;
            _ib->add_index( index + 2 ) ;
            _ib->add_index( index + 3 ) ;
        }
    }

    {
        data_buffer_t db ;
        _rect_info_ptr = data_buffer_t::create( std::move( db ),
            "[gl33_rect_render_2d_plug::on_load] : rect_info" ) ;
    }

    return so_gpx::plug_result::ok ;
}

//************************************************************************************
so_gpx::plug_result gl33_rect_render_2d_plug::on_unload( void_t )
{
    return so_gpx::plug_result::ok ;
}

//************************************************************************************
so_gpx::plug_result gl33_rect_render_2d_plug::on_initialize( init_info_cref_t )
{
    {
        {
            auto const res = this_t::api()->create_buffer( _vb ) ;
        }
        {
            auto const res = this_t::api()->create_buffer( _ib ) ;
        }
        {
            auto const res = this_t::api()->create_shader( _vs ) ;
        }
        {
            auto const res = this_t::api()->compile( _vs )  ;
        }

        {
            auto const res = this_t::api()->create_shader( _ps ) ;
        }
        {
            auto const res = this_t::api()->compile( _ps )  ;
        }
        {
            auto const res = this_t::api()->create_program( _prog ) ;
        }
        {
            _prog->set_shader( _vs ) ;
            _prog->set_shader( _ps ) ;
        }
        {
            auto const res = this_t::api()->link( _prog ) ;
        }
        {
            auto const res = this_t::api()->create_config( _config ) ;
        }
        {
            _config->set_program( _prog ) ;
            _config->bind( so_gpu::vertex_attribute::position, "in_pos" ) ;
            _config->bind( so_gpu::primitive_type::triangles, _vb, _ib ) ;
        }
    }

    {
        auto const res = this_t::api()->create_buffer( _rect_info_ptr ) ;
    }

    //
    // memory allocation
    //
    {
        auto const res = this_t::api()->alloc_buffer_memory( _vb,
            so_gpu::memory_alloc_info( true ) ) ;
    }
    {
        auto const res = this_t::api()->alloc_buffer_memory( _ib,
            so_gpu::memory_alloc_info( true ) ) ;
    }

    return so_gpx::plug_result::ok ;
}

//************************************************************************************
so_gpx::plug_result gl33_rect_render_2d_plug::on_release( void_t )
{
    this_t::api()->release_buffer( _vb ) ;
    this_t::api()->release_buffer( _ib ) ;
    this_t::api()->release_shader( _vs ) ;
    this_t::api()->release_shader( _ps ) ;
    this_t::api()->release_program( _prog ) ;
    this_t::api()->release_config( _config ) ;

    this_t::api()->release_buffer( _rect_info_ptr ) ;

    for( auto * var_set : _var_sets )
    {
        this_t::api()->release_variable( var_set ) ;
    }

    return so_gpx::plug_result::ok ;
}

//************************************************************************************
so_gpx::plug_result gl33_rect_render_2d_plug::on_update( update_info_cref_t )
{
    // 1. copy the data
    {
        _rect_info_ptr->resize( 0 ) ;

        // copy all glyph data
        {
            for( auto const & gi : _sd->rect_infos )
            {
                so_math::vec4f_t const color = gi.color ;
                so_math::vec2f_t const pos = gi.pos ;
                so_math::vec2f_t const scale = gi.scale ;
                so_math::vec2f_t const pivot = gi.pivot ;

                _rect_info_ptr->add_element( so_math::vec4f_t( pos.x(), pos.y(), scale.x(), scale.y() ) ) ;
                _rect_info_ptr->add_element( so_math::vec4f_t( pivot, so_math::vec2f_t() ) ) ;
                _rect_info_ptr->add_element( color ) ;
            }
        }

        // copy global data
        {
            _proj = _sd->proj ;
            _view = _sd->view ;
        }

        // 1. copy all per group data
        // 2. calc start offset
        {
            uint32_t offset = 0 ;
            for( auto const & gi : _sd->per_group_infos )
            {
                auto iter = std::find_if( _per_group_datas.begin(), _per_group_datas.end(),
                    [&] ( this_t::per_group_data_cref_t pgd )
                {
                    return pgd.group_id == gi.group_id ;
                } ) ;

                if( iter == _per_group_datas.end() )
                {
                    this_t::per_group_data_t pgd ;
                    pgd.group_id = gi.group_id ;

                    auto * var_set = so_gpu::variable_set_t::create(
                        "[gl33_text_render_2d_plug::on_update] : variable_set" ) ;

                    this_t::api()->create_variable( var_set ) ;

                    // configure variable set
                    {
                        // required for rendering
                        pgd.varset_id = _config->get_num_varsets() ;
                        _config->add_variable_set( var_set ) ;

                        var_set->bind_buffer<so_math::vec4f_t>( "u_rect_info", _rect_info_ptr ) ;

                        {
                            pgd.proj = so_gpu::memory::alloc<so_math::mat4f_t>(
                                "[gl33_text_render_2d_plug::on_update] : projection matrix" ) ;
                            var_set->bind_data<so_math::mat4f_t>( "u_proj", pgd.proj ) ;
                        }

                        {
                            pgd.view = so_gpu::memory::alloc<so_math::mat4f_t>(
                                "[gl33_text_render_2d_plug::on_update] : view matrix" ) ;
                            var_set->bind_data<so_math::mat4f_t>( "u_view", pgd.view ) ;
                        }

                        {
                            pgd.start_offset = so_gpu::memory::alloc<uint32_t>(
                                "[gl33_text_render_2d_plug::on_update] : start_offset" ) ;
                            var_set->bind_data<uint32_t>( "u_offset", pgd.start_offset ) ;
                        }
                    }

                    pgd.varset_id = _var_sets.size() ;
                    _var_sets.push_back( var_set ) ;

                    iter = _per_group_datas.insert( _per_group_datas.end(), pgd ) ;
                }

                // need to be updated every call
                {
                    iter->num_elements = uint32_t( gi.num_rects ) ;
                    ( *iter->start_offset ) = offset ;
                    ( *iter->proj ) = gi.proj ;
                    ( *iter->view ) = gi.view ;
                    offset += iter->num_elements ;

                    // for now, overwrite with global matrices
                    ( *iter->proj ) = _proj ;
                    ( *iter->view ) = _view ;
                }
            }
        }
    }

    // 2. transfer to gpu
    // @todo optimize. Do not always reallocate
    {
        auto const res = this_t::api()->alloc_buffer_memory( _rect_info_ptr,
            so_gpu::memory_alloc_info_t( true ) ) ;
    }

    return so_gpx::plug_result::ok ;
}

//************************************************************************************
so_gpx::plug_result gl33_rect_render_2d_plug::on_execute( execute_info_cref_t ri )
{
    auto iter = std::find_if( _per_group_datas.begin(), _per_group_datas.end(),
        [&] ( this_t::per_group_data_cref_t pgd ) { return pgd.group_id == ri.rnd_id ; } ) ;

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
        this_t::api()->load_variable( _var_sets[ iter->varset_id ] ) ;
        this_t::api()->execute( so_gpu::render_config_info( _config, iter->varset_id, 0,
            ( iter->num_elements ) * 6 ) ) ;
    }
    this_t::api()->disable( so_gpu::render_state::blend ) ;

    // @todo restore state

    return so_gpx::plug_result::ok ;
}

//************************************************************************************
void_t gl33_rect_render_2d_plug::destroy( void_t )
{
    this_t::destroy( this ) ;
}

//************************************************************************************