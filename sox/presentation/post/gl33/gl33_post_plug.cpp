//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "gl33_post_plug.h"

#include <snakeoil/gpu/api/gl/igl_33_api.h>
#include <snakeoil/gpu/api/gl/igl_33_api.h>
#include <snakeoil/gpu/framebuffer/framebuffer_2d.h>
#include <snakeoil/gpu/image/color_image_2d.hpp>
#include <snakeoil/gpu/image/depth_image_2d.hpp>
#include <snakeoil/gpu/texture/texture_2d.h>
#include <snakeoil/gpu/texture/depth_texture_2d.h>
#include <snakeoil/gpu/manager/gpu_manager.h>
#include <snakeoil/gpu/image/image_creator.h>
#include <snakeoil/gpu/viewport/viewport_2d.h>
#include <snakeoil/gpu/buffer/vertex_buffer.hpp>
#include <snakeoil/gpu/buffer/index_buffer.hpp>
#include <snakeoil/gpu/shader/vertex_shader.h>
#include <snakeoil/gpu/shader/pixel_shader.h>
#include <snakeoil/gpu/program/program.h>
#include <snakeoil/gpu/program/config.h>
#include <snakeoil/gpu/variable/variable_set.h>

#include <snakeoil/io/global.h>
#include <snakeoil/log/global.h>

using namespace sox_presentation ;
using namespace sox_presentation::gl_33 ;

//*************************************************************************************
gl33_post_plug::gl33_post_plug( sox_presentation::post_shared_data_ptr_t sd,
    so_gpu::so_gl::igl_33_api_ptr_t aptr ) : this_base_t( aptr )
{
    _vb = vb_t::create( "vertex buffer" ) ;
    _ib = ib_t::create( "index buffer" ) ;
    _vs = so_gpu::vertex_shader_t::create( "vertex shader" ) ;
    _ps = so_gpu::pixel_shader_t::create( "pixel shader" ) ;
    _prog = so_gpu::program_t::create( "program" ) ;
    _config = so_gpu::config_t::create( "config" ) ;    

    _sd = sd ;
}

//*************************************************************************************
gl33_post_plug::gl33_post_plug( this_rref_t rhv ) : 
    this_base_t( std::move(rhv) )
{
    so_move_member_ptr( _vb, rhv ) ;
    so_move_member_ptr( _ib, rhv ) ;
    so_move_member_ptr( _vs, rhv ) ;
    so_move_member_ptr( _ps, rhv ) ;
    so_move_member_ptr( _prog, rhv ) ;
    so_move_member_ptr( _config, rhv ) ;

    so_move_member_ptr( _sd, rhv ) ;

    _bar_datas = std::move( rhv._bar_datas ) ;
}

//*************************************************************************************
gl33_post_plug::~gl33_post_plug( void_t )
{
    if( so_core::is_not_nullptr( _vb ) )
        _vb->destroy() ;

    if( so_core::is_not_nullptr( _ib ) )
        _ib->destroy() ;

    if( so_core::is_not_nullptr( _vs ) )
        so_gpu::vertex_shader_t::destroy( _vs ) ;

    if( so_core::is_not_nullptr( _ps ) )
        so_gpu::pixel_shader_t::destroy( _ps ) ;

    if( so_core::is_not_nullptr( _prog ) )
        so_gpu::program_t::destroy( _prog ) ;

    if( so_core::is_not_nullptr( _config ) )
        so_gpu::config_t::destroy( _config ) ;

    for( auto * item : _bar_datas )
    {
        if( so_core::is_not_nullptr( item->vs ) )
            item->vs->destroy() ;
    }
    _bar_datas.clear() ;
}

//*************************************************************************************
gl33_post_plug::this_ptr_t gl33_post_plug::create( 
    this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_memory::global_t::alloc( std::move( rhv ), p ) ;
}

//*************************************************************************************
void_t gl33_post_plug::destroy( this_ptr_t ptr )
{
    so_memory::global_t::dealloc( ptr ) ;
}

//*************************************************************************************
so_gpx::plug_result gl33_post_plug::on_load( load_info_cref_t li )
{
    auto const base = "";// SHADER_BASE ;

    // load shader
    {
        auto const vs_path = base / so_io::path_t( "render/post/gl33/shader/post.vs" ) ;
        auto const ps_path = base / so_io::path_t( "render/post/gl33/shader/post.ps" ) ;

        so_io::load_handle_t vsh = so_io::global::load( so_io::path_t( vs_path ) ) ;
        so_io::load_handle_t psh = so_io::global::load( so_io::path_t( ps_path ) ) ;

        {
            so_std::string_t vs ;

            vsh.wait_for_operation( [&] ( char_cptr_t din, size_t sib, so_io::result res )
            {
                so_log::global::error_and_exit( so_io::no_success( res ),
                    "Vertex shader required" ) ;

                vs = so_std::string_t( din, sib ) ;
            } ) ;
            _vss = vs ;
        }

        {
            so_std::string_t ps ;
            psh.wait_for_operation( [&] ( char_cptr_t din, size_t sib, so_io::result res )
            {
                so_log::global::error_and_exit( so_io::no_success( res ),
                    "Pixel shader required" ) ;

                ps = so_std::string_t( din, sib ) ;
            } ) ;
            _pss = ps ;
        }
    }

    if( li.reload )
        return so_gpx::plug_result::ok ;

    {
        _vb->add_layout_element<so_math::vec3f_t>( so_gpu::vertex_attribute::position ) ;
        _vb->add_vertex( vertex_t { so_math::vec3f_t( -0.5f, -0.5f, 0.0f ) } ) ;
        _vb->add_vertex( vertex_t { so_math::vec3f_t( -0.5f, 0.5f, 0.0f ) } ) ;
        _vb->add_vertex( vertex_t { so_math::vec3f_t( 0.5f, 0.5f, 0.0f ) } ) ;
        _vb->add_vertex( vertex_t { so_math::vec3f_t( 0.5f, -0.5f, 0.0f ) } ) ;
    }
    {
        _ib->add_index( 0 ) ;
        _ib->add_index( 1 ) ;
        _ib->add_index( 2 ) ;
        _ib->add_index( 0 ) ;
        _ib->add_index( 2 ) ;
        _ib->add_index( 3 ) ;
    }

    return so_gpx::plug_result::ok ;
}

//*************************************************************************************
so_gpx::plug_result gl33_post_plug::on_unload( void_t )
{
    return so_gpx::plug_result::ok ;
}

//*************************************************************************************
so_gpx::plug_result gl33_post_plug::on_initialize( init_info_cref_t ii )
{
    if( ii.reinit )
    {
        {
            auto const res = this_t::api()->release_program( _prog ) ;
            if( so_gpu::no_success( res ) )
            {
                so_log::global_t::error( "[gl33_rects_plug::on_initialize] : release program" ) ;
                return so_gpx::plug_result::failed ;
            }
        }
        {
            auto const res = this_t::api()->release_config( _config ) ;
            if( so_gpu::no_success( res ) )
            {
                so_log::global_t::error( "[gl33_rects_plug::on_initialize] : release config" ) ;
                return so_gpx::plug_result::failed ;
            }
        }

        // the variable sets do not need to be destroyed and removed from
        // the config. Its just easier for now and we have another use-case.
        // the variable sets could also just be released by gpu layer and 
        // recreated by gpu layer
        for( auto * item : _bar_datas )
        {
            auto const res = this_t::api()->release_variable( item->vs ) ;
            if( so_gpu::no_success( res ) )
            {
                so_log::global_t::error( "[gl33_rects_plug::on_initialize] : release variable set" ) ;
                return so_gpx::plug_result::failed ;
            }
            _config->remove_variable_set( item->vs ) ;
            item->vs->destroy() ;
        }
        
        _bar_datas.clear() ;
        _num_bars = 0 ;
    }
    else
    {
        {
            auto const res = this_t::api()->create_buffer( _vb ) ;
            if( so_gpu::no_success( res ) )
            {
                so_log::global_t::error( "[gl33_rects_plug::on_initialize] : buffer creation" ) ;
                return so_gpx::plug_result::failed ;
            }
        }
        {
            auto const res = this_t::api()->create_buffer( _ib ) ;
            if( so_gpu::no_success( res ) )
            {
                so_log::global_t::error( "[gl33_rects_plug::on_initialize] : buffer creation" ) ;
                return so_gpx::plug_result::failed ;
            }
        }
        {
            auto const res = this_t::api()->create_shader( _vs ) ;
            if( so_gpu::no_success( res ) )
            {
                so_log::global_t::error( "[gl33_rects_plug::on_initialize] : vertex shader creation" ) ;
                return so_gpx::plug_result::failed ;
            }
        }
        {
            auto const res = this_t::api()->create_shader( _ps ) ;
            if( so_gpu::no_success( res ) )
            {
                so_log::global_t::error( "[gl33_rects_plug::on_initialize] : pixel shader creation" ) ;
                return so_gpx::plug_result::failed ;
            }
        }
    }

    {
        auto const res = this_t::api()->create_program( _prog ) ;
        if( so_gpu::no_success( res ) )
        {
            so_log::global_t::error( "[gl33_rects_plug::on_initialize] : create program" ) ;
            return so_gpx::plug_result::failed ;
        }
    }
    {
        auto const res = this_t::api()->create_config( _config ) ;
        if( so_gpu::no_success( res ) )
        {
            so_log::global_t::error( "[gl33_rects_plug::on_initialize] : create config" ) ;
            return so_gpx::plug_result::failed ;
        }
    }
    {
        _prog->set_shader( _vs ) ;
        _prog->set_shader( _ps ) ;
    }
    {
        auto tmp_vs = _vs->get_code() ;
        auto tmp_ps = _ps->get_code() ;

        _vs->set_code( _vss ) ;
        _ps->set_code( _pss ) ;

        if( ii.reinit )
        {
            _vss = tmp_vs ;
            _pss = tmp_ps ;
        }


        bool_t suc = true ;
        {
            auto const res = this_t::api()->compile( _vs )  ;
            suc = suc && so_gpu::success( res ) ;

        }
        {
            auto const res = this_t::api()->compile( _ps )  ;
            suc = suc && so_gpu::success( res ) ;
        }
        {
            auto const res = this_t::api()->link( _prog ) ;
            suc = suc && so_gpu::success( res ) ;
        }

        if( so_core::is_not( suc ) && ii.reinit )
        {
            so_log::global_t::error( "[gl33_rects_plug::on_initialize] : "
                "compile/link shaders - using old shaders" ) ;

            _vs->set_code( _vss ) ;
            _ps->set_code( _pss ) ;
            this_t::api()->compile( _vs )  ;
            this_t::api()->compile( _ps )  ;
            this_t::api()->link( _prog ) ;
        }
        else if( so_core::is_not( suc ) && so_core::is_not( ii.reinit ) )
        {
            so_log::global_t::error( "[gl33_post_plug::on_initialize] : compile/link shaders" ) ;
            return so_gpx::plug_result::failed ;
        }
    }

    {
        _config->set_program( _prog ) ;
        _config->bind( so_gpu::vertex_attribute::position, "in_pos" ) ;
        _config->bind( so_gpu::primitive_type::triangles, _vb, _ib ) ;
    }
    
    

    return so_gpx::plug_result::ok ;
}

//*************************************************************************************
void_t gl33_post_plug::create_bar_data( size_t const num_bars ) 
{
    for( size_t i=_num_bars; i<num_bars; ++i )
    {
        this_t::bar_data_ptr_t bd = so_memory::global_t::alloc( 
            this_t::bar_data_t(), "[bar_data]" ) ;

        so_gpu::variable_set_t vs ;
        auto const res = this_t::api()->create_variable( &vs ) ;

        vs.bind_data< so_math::mat4f_t >( "u_view", &_sd->view ) ;
        vs.bind_data< so_math::mat4f_t >( "u_proj", &_sd->proj ) ;
        vs.bind_data< so_math::vec4f_t >( "u_pos_wh", &bd->pos_wh ) ;
        
        auto * new_ptr = so_gpu::variable_set_t::create( std::move( vs ),
            "[gl33_post_plug::create_bar_data] : variable_set" );

        bd->vs = new_ptr ;

        _config->add_variable_set( new_ptr ) ;
        _bar_datas.push_back( bd ) ;
    }

    _num_bars = num_bars ;
}

//*************************************************************************************
so_gpx::plug_result gl33_post_plug::on_release( void_t )
{
    this_t::api()->release_buffer( _vb ) ;
    this_t::api()->release_buffer( _ib ) ;
    this_t::api()->release_shader( _vs ) ;
    this_t::api()->release_shader( _ps ) ;
    this_t::api()->release_program( _prog ) ;
    this_t::api()->release_config( _config ) ;

    for( auto * bd : _bar_datas )
    {
        this_t::api()->release_variable( bd->vs ) ;
        //so_memory::global_t::dealloc( bd ) ;
    }

    return so_gpx::plug_result::ok ;
}

//*************************************************************************************
so_gpx::plug_result gl33_post_plug::on_execute( execute_info_cref_t ei )
{
    size_t i = 0; 
    for( auto * bd : _bar_datas )
    {
        this_t::api()->load_variable( bd->vs ) ;
        this_t::api()->execute( so_gpu::render_config_info( _config, i++ ) ) ;
    }

    return so_gpx::plug_result::ok ;
}

//*************************************************************************************
so_gpx::plug_result gl33_post_plug::on_update( update_info_cref_t )
{
    {
        so_gpu::viewport_2d_t vp = _sd->win_vp->get_data() ;        
        _dims = vp.get_width_height<float_t>() ;
    }

    {
        auto const res = this_t::api()->alloc_buffer_memory( _vb,
            so_gpu::memory_alloc_info( true ) ) ;
    }
    {
        auto const res = this_t::api()->alloc_buffer_memory( _ib,
            so_gpu::memory_alloc_info( true ) ) ;
    }

    // copy changed gpu buffer data if required here
    return so_gpx::plug_result::need_transfer ;
}

//*************************************************************************************
void_t gl33_post_plug::destroy( void_t )
{
    this_t::destroy( this ) ;
}


