//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "gl33_image_render_2d_plug.h"

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
#include <snakeoil/gpu/image/image_creator.h>

#include <snakeoil/font/glyph_atlas/glyph_atlas.h>

#include <snakeoil/io/global.h>
#include <snakeoil/log/global.h>

#include <snakeoil/math/vector/vector4.hpp>

#include <snakeoil/core/execute_if.hpp>

using namespace so_gfx ;
using namespace so_gfx::so_gl33 ;

//************************************************************************************
gl33_image_render_2d_plug::gl33_image_render_2d_plug(
    so_gfx::image_render_2d_shared_data_ptr_t sd_ptr, so_gpu::so_gl::igl_33_api_ptr_t aptr ) : this_base_t( aptr )
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
gl33_image_render_2d_plug::gl33_image_render_2d_plug( this_rref_t rhv ) : this_base_t( std::move( rhv ) )
{
    so_move_member_ptr( _sd, rhv ) ;
    so_move_member_ptr( _vb, rhv ) ;
    so_move_member_ptr( _ib, rhv ) ;
    so_move_member_ptr( _vs, rhv ) ;
    so_move_member_ptr( _ps, rhv ) ;
    so_move_member_ptr( _prog, rhv ) ;
    so_move_member_ptr( _config, rhv ) ;


    so_move_member_ptr( _image_info_ptr, rhv ) ;

    _image_datas = std::move( rhv._image_datas ) ;
}

//************************************************************************************
gl33_image_render_2d_plug::~gl33_image_render_2d_plug( void_t )
{
    this_t::vb_t::destroy( _vb ) ;
    this_t::ib_t::destroy( _ib ) ;

    so_gpu::vertex_shader_t::destroy( _vs ) ;
    so_gpu::pixel_shader_t::destroy( _ps ) ;
    so_gpu::program_t::destroy( _prog ) ;
    so_gpu::config_t::destroy( _config ) ;

    this_t::data_buffer_t::destroy( _image_info_ptr ) ;

    for( auto & item : _image_datas )
    {
        item.var_set->destroy() ;
        so_gpu::memory::dealloc( item.start_offset ) ;
        item._gpu_img_ptr->destroy() ;
        so_gpu::texture_2d_t::destroy( item._tx ) ;
    }
}

//************************************************************************************
gl33_image_render_2d_plug::this_ptr_t gl33_image_render_2d_plug::create(
    this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_gfx::memory::alloc( std::move( rhv ), p ) ;
}

//************************************************************************************
void_t gl33_image_render_2d_plug::destroy( this_ptr_t ptr )
{
    so_gfx::memory::dealloc( ptr ) ;
}

//************************************************************************************
void_t gl33_image_render_2d_plug::destroy( void_t )
{
    this_t::destroy( this ) ;
}

//*********************************************************************
so_gpx::plug_result gl33_image_render_2d_plug::on_load( void_t )
{
    so_std::string_t const base( so_std::string_t( GFX_SOURCE_DIR ) + "/image/gl33/shader" ) ;

    // text render shader
    {
        so_std::string_t const vs_path = base + so_std::string( "/image_render_2d.vs.glsl" ) ;
        so_std::string_t const ps_path = base + so_std::string( "/image_render_2d.ps.glsl" ) ;

        so_io::load_handle_t vsh = so_io::global::load( so_io::path_t( vs_path ) ) ;
        so_io::load_handle_t psh = so_io::global::load( so_io::path_t( ps_path ) ) ;

        {
            so_std::string_t vs ;
            vsh.wait_for_operation( [&] ( char_cptr_t din, size_t sib, so_io::result res )
            {
                so_log::global::error_and_exit( so_io::no_success( res ),
                    "[gl33_image_render_2d_plug::on_load] : Vertex shader not found : " + vs_path ) ;

                vs = so_std::string_t( din, sib ) ;
            } ) ;
            _vs->set_code( vs ) ;
        }

        {
            so_std::string_t ps ;
            psh.wait_for_operation( [&] ( char_cptr_t din, size_t sib, so_io::result res )
            {
                so_log::global::error_and_exit( so_io::no_success( res ),
                    "[gl33_image_render_2d_plug::on_load] : Pixel shader not found : " + ps_path ) ;

                ps = so_std::string_t( din, sib ) ;
            } ) ;
            _ps->set_code( ps ) ;
        }
    }

    {
        _vb->add_layout_element<so_math::vec3f_t>( so_gpu::vertex_attribute::position ) ;

        size_t const num_quads = _num_quads ;

        for( size_t i = 0; i < num_quads; ++i )
        {
            _vb->add_vertex( vertex_t { so_math::vec3f_t( -0.5f, -0.5f, 0.0f ) } ) ;
            _vb->add_vertex( vertex_t { so_math::vec3f_t( -0.5f, 0.5f, 0.0f ) } ) ;
            _vb->add_vertex( vertex_t { so_math::vec3f_t( 0.5f, 0.5f, 0.0f ) } ) ;
            _vb->add_vertex( vertex_t { so_math::vec3f_t( 0.5f, -0.5f, 0.0f ) } ) ;
        }

        for( size_t i = 0; i < num_quads; ++i )
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

    _image_info_ptr = data_buffer_t::create( data_buffer_t(),
        "[gl33_image_render_2d_plug::on_load] : image_info" ) ;

    return so_gpx::plug_result::ok ;
}

//*********************************************************************
so_gpx::plug_result gl33_image_render_2d_plug::on_unload( void_t ) {
    return so_gpx::plug_result::ok ;
}

//*********************************************************************
so_gpx::plug_result gl33_image_render_2d_plug::on_initialize( so_gpx::iplug_t::init_info_cref_t )
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
        auto const res = this_t::api()->create_buffer( _image_info_ptr ) ;
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

//*********************************************************************
so_gpx::plug_result gl33_image_render_2d_plug::on_release( void_t )
{
    this_t::api()->release_buffer( _vb ) ;
    this_t::api()->release_buffer( _ib ) ;
    this_t::api()->release_shader( _vs ) ;
    this_t::api()->release_shader( _ps ) ;
    this_t::api()->release_program( _prog ) ;
    this_t::api()->release_config( _config ) ;    
    this_t::api()->release_buffer( _image_info_ptr ) ;

    for( auto & item : _image_datas )
    {
        this_t::api()->release_variable( item.var_set ) ;
        this_t::api()->release_image( item._gpu_img_ptr ) ;
        this_t::api()->release_texture( item._tx ) ;
    }

    return so_gpx::plug_result::ok ;
}

//*********************************************************************
so_gpx::plug_result gl33_image_render_2d_plug::on_update( update_info_cref_t )
{
    {
        _image_info_ptr->resize( 0 ) ;
        _draw_calls.clear() ;
    }

    // 1. copy data
    {
        // copy image infos
        {
            for( auto & ii : _sd->image_infos )
            {
                // find image and load
                {
                    bool_t new_image = false ;

                    auto iter = std::find_if( _image_datas.begin(), _image_datas.end(),
                        [&] ( this_t::image_data_cref_t id ) { return ii.img_ptr == id._img_ptr ;  } ) ;

                    auto const res = so_core::execute_if( iter == _image_datas.end(), [&]( void_t )
                    {
                        this_t::image_data_t id ;
                        id._img_ptr = ii.img_ptr ;

                        {
                            id._gpu_img_ptr = so_gpu::image_creator_t( ii.img_ptr,
                                "[gl33_image_render_2d_plug::on_update] : gpu image" ).move_image_2d() ;

                           
                            if( so_core::is_nullptr( id._gpu_img_ptr ) )
                            {
                                so_log::global::error("[gl33_image_render_2d_plug::on_update] : "
                                "Can not create requested gpu image of specified types.") ;
                                return so_core::execute_result::failed ;
                            }
                        }
                        {
                            auto const res = this_t::api()->create_image( id._gpu_img_ptr ) ;
                            so_log::global::error( so_gpu::no_success( res ),
                                "[gl33_image_render_2d_plug::on_update] : failed to create image" ) ;
                        }
                        {
                            id._tx = so_gpu::texture_2d_t::create( so_gpu::texture_2d_t( id._gpu_img_ptr ),
                                "[gl33_image_render_2d_plug::on_update] : texture" ) ;
                        }
                        {
                            auto const res = this_t::api()->create_texture( id._tx ) ;
                            so_log::global::error( so_gpu::no_success( res ),
                                "[gl33_image_render_2d_plug::on_update] : create texture" ) ;
                        }
                        {
                            id.var_set = so_gpu::variable_set_t::create(
                                "[gl33_image_render_2d_plug::on_update] : variable_set" ) ;

                            this_t::api()->create_variable( id.var_set ) ;

                            // configure variable set
                            {
                                // required for rendering
                                id.varset_id = _config->get_num_varsets() ;
                                _config->add_variable_set( id.var_set ) ;

                                id.var_set->bind_buffer<so_math::vec4f_t>( "u_image_info", _image_info_ptr ) ;

                                id.var_set->bind_texture( "u_smp", id._tx ) ;

                                //id.var_set->bind_data<so_math::vec2f_t>( "u_scale", &( _sd->dim_scale ) ) ;

                                {
                                    id.start_offset = so_gpu::memory::alloc<uint32_t>(
                                        "[gl33_image_render_2d_plug::on_update] : start_offset" ) ;
                                    id.var_set->bind_data<uint32_t>( "u_offset", id.start_offset ) ;
                                }
                            }
                        }

                        iter = _image_datas.insert( _image_datas.end(), id ) ;

                        new_image = true ;

                        return so_core::execute_result::ok ;
                    } ) ;
                    
                    bool_t const eres = res == so_core::execute_result::ok || res == so_core::execute_result::cond_false ;

                    if( ii.dirty )
                    {
                        so_gpu::image_creator_t::copy_from( ii.img_ptr, iter->_gpu_img_ptr ) ;
                    }

                    if( ( ii.dirty || new_image ) && eres )
                    {
                        auto const ires = this_t::api()->alloc_image_memory( iter->_gpu_img_ptr,
                            so_gpu::image_alloc_info( true ) ) ;

                        so_log::global::error( so_gpu::no_success( ires ),
                            "[gl33_image_render_2d_plug::on_update] : failed to allocate image" ) ;
                    }
                }
            }

            // check what we have to draw
            // map : shared data's group -> render call data
            {
                size_t offset = 0 ;
                for( auto const & gi : _sd->per_group_infos )
                {
                    // 1. store all image meta info into texture buffer
                    for( size_t i = offset; i < offset+gi.num_images; ++i )
                    {
                        auto const & ii = _sd->image_infos[ i ] ;
                        
                        _image_info_ptr->add_element( so_math::vec4f_t( ii.pos, ii.scale ) ) ;
                        _image_info_ptr->add_element( so_math::vec4f_t( ii.color ) ) ;
                        _image_info_ptr->add_element( so_math::vec4f_t( ii.texcoords ) ) ;
                        _image_info_ptr->add_element( so_math::vec4f_t( ii.rot, so_math::vec3f_t() ) ) ;
                    }

                    // 2. create and store draw call
                    {
                        this_t::draw_call_data_t dcd ;

                        // find image index for retrieving variable data during rendering
                        {
                            auto const iter = std::find_if( _image_datas.begin(), _image_datas.end(),
                                [&] ( image_data_cref_t d ) { return d._img_ptr == _sd->image_infos[ offset ].img_ptr ; } ) ;

                            if( iter == _image_datas.end() )
                            {
                                so_log::global::warning( "[gl33_image_render_2d_plug::on_update] : "
                                    "image not found" ) ;
                                continue ;
                            }

                            dcd.image_data_id = iter - _image_datas.begin() ;
                        }
                        dcd.group_id = gi.group_id ;
                        dcd.num_elements = uint32_t( gi.num_images ) ;
                        dcd.offset = uint32_t( offset ) ;

                        _draw_calls.push_back( dcd ) ;
                    }
                    offset += gi.num_images ;
                }
            }
        }
    }

    // 2. transfer to gpu
    // @todo optimize. Do not always reallocate
    {
        auto const res = this_t::api()->alloc_buffer_memory( _image_info_ptr,
            so_gpu::memory_alloc_info_t( true ) ) ;
    }

    return so_gpx::plug_result::ok ;
}

//*********************************************************************
so_gpx::plug_result gl33_image_render_2d_plug::on_execute( so_gpx::iplug_t::execute_info_cref_t ri )
{
    auto beg = std::lower_bound( _draw_calls.begin(), _draw_calls.end(), ri.rnd_id,
        [&]( draw_call_data_cref_t item, size_t const val ) { return item.group_id < val ; } ) ;

    auto end = std::upper_bound( _draw_calls.begin(), _draw_calls.end(), ri.rnd_id,
        [&]( size_t const val, draw_call_data_cref_t item ) { return item.group_id > val ; } ) ;

    for( auto iter = beg; iter < end; ++iter )
    {
        image_data_ref_t id = _image_datas[ iter->image_data_id ] ;

        // @todo save state

        this_t::api()->enable( so_gpu::render_state::blend ) ;
        this_t::api()->set_state( so_gpu::blend_factor::src_alpha, so_gpu::blend_factor::one ) ;
        {
            (*id.start_offset) = iter->offset ;
            this_t::api()->load_variable( id.var_set ) ;
            this_t::api()->execute( so_gpu::render_config_info( _config, id.varset_id,
                ( iter->num_elements ) * 6 ) ) ;
        }
        this_t::api()->disable( so_gpu::render_state::blend ) ;

        // @todo restore state
    }
    return so_gpx::plug_result::ok ;
}