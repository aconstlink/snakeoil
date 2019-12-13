//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "gl33_sdf_a_plug.h"

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
using namespace sox_presentation::transitions::gl_33 ;

//*************************************************************************************
gl33_sdf_a_plug::gl33_sdf_a_plug( sox_presentation::transitions::sdf_a_shared_data_ptr_t sd,
    so_gpu::so_gl::igl_33_api_ptr_t aptr ) : this_base_t( aptr )
{
    _vb = vb_t::create( "vertex buffer" ) ;
    _ib = ib_t::create( "index buffer" ) ;
    _vs_color = so_gpu::vertex_shader_t::create( "vertex shader" ) ;
    _ps_color = so_gpu::pixel_shader_t::create( "pixel shader" ) ;
    _prog_color = so_gpu::program_t::create( "program" ) ;
    _config_color = so_gpu::config_t::create( "config" ) ;    
    _vars_color = so_gpu::variable_set_t::create("[gl33_sdf_a_plug::gl33_sdf_a_plug] : varset 0") ;

    _vs_mask = so_gpu::vertex_shader_t::create( "vertex shader" ) ;
    _ps_mask = so_gpu::pixel_shader_t::create( "pixel shader" ) ;
    _prog_mask = so_gpu::program_t::create( "program" ) ;
    _config_mask = so_gpu::config_t::create( "config" ) ;
    _vars_mask = so_gpu::variable_set_t::create("[gl33_sdf_a_plug::gl33_sdf_a_plug] : varset 0") ;

    _sd = sd ;
}

//*************************************************************************************
gl33_sdf_a_plug::gl33_sdf_a_plug( this_rref_t rhv ) : 
    this_base_t( std::move(rhv) )
{
    so_move_member_ptr( _vb, rhv ) ;
    so_move_member_ptr( _ib, rhv ) ;
    so_move_member_ptr( _vs_color, rhv ) ;
    so_move_member_ptr( _ps_color, rhv ) ;
    so_move_member_ptr( _prog_color, rhv ) ;
    so_move_member_ptr( _config_color, rhv ) ;
    so_move_member_ptr( _vars_color, rhv ) ;

    so_move_member_ptr( _vs_mask, rhv ) ;
    so_move_member_ptr( _ps_mask, rhv ) ;
    so_move_member_ptr( _prog_mask, rhv ) ;
    so_move_member_ptr( _config_mask, rhv ) ;
    so_move_member_ptr( _vars_mask, rhv ) ;

    so_move_member_ptr( _sd, rhv ) ;
}

//*************************************************************************************
gl33_sdf_a_plug::~gl33_sdf_a_plug( void_t )
{
    if( so_core::is_not_nullptr( _vb ) )
        _vb->destroy() ;

    if( so_core::is_not_nullptr( _ib ) )
        _ib->destroy() ;

    if( so_core::is_not_nullptr( _vs_color ) )
        so_gpu::vertex_shader_t::destroy( _vs_color ) ;

    if( so_core::is_not_nullptr( _ps_color ) )
        so_gpu::pixel_shader_t::destroy( _ps_color ) ;

    if( so_core::is_not_nullptr( _prog_color ) )
        so_gpu::program_t::destroy( _prog_color ) ;

    if( so_core::is_not_nullptr( _config_color ) )
        so_gpu::config_t::destroy( _config_color ) ;

    if( so_core::is_not_nullptr( _vars_color ) )
        so_gpu::variable_set_t::destroy( _vars_color ) ;

    if( so_core::is_not_nullptr( _vs_mask ) )
        so_gpu::vertex_shader_t::destroy( _vs_mask ) ;

    if( so_core::is_not_nullptr( _ps_mask ) )
        so_gpu::pixel_shader_t::destroy( _ps_mask ) ;

    if( so_core::is_not_nullptr( _prog_mask ) )
        so_gpu::program_t::destroy( _prog_mask ) ;

    if( so_core::is_not_nullptr( _config_mask ) )
        so_gpu::config_t::destroy( _config_mask ) ;

    if( so_core::is_not_nullptr( _vars_mask ) )
        so_gpu::variable_set_t::destroy( _vars_mask ) ;
}

//*************************************************************************************
gl33_sdf_a_plug::this_ptr_t gl33_sdf_a_plug::create( 
    this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_memory::global_t::alloc( std::move( rhv ), p ) ;
}

//*************************************************************************************
void_t gl33_sdf_a_plug::destroy( this_ptr_t ptr )
{
    so_memory::global_t::dealloc( ptr ) ;
}

//*************************************************************************************
so_gpx::plug_result gl33_sdf_a_plug::on_load( load_info_cref_t li )
{
    auto const base = sox::find_shader_path("/presentation/transitions/sdf_a/gl33/shader/") ;

    // load shader color
    {
        auto const vs_path = base / so_io::path_t( "gl33_sdf_a_color.vs.glsl" ) ;
        auto const ps_path = base / so_io::path_t( "gl33_sdf_a_color.ps.glsl" ) ;

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
            _vss_mix = vs ;
        }

        {
            so_std::string_t ps ;
            psh.wait_for_operation( [&] ( char_cptr_t din, size_t sib, so_io::result res )
            {
                so_log::global::error_and_exit( so_io::no_success( res ),
                    "Pixel shader required" ) ;

                ps = so_std::string_t( din, sib ) ;
            } ) ;
            _pss_mix = ps ;
        }
    }

    // load shader mask
    {
        auto const vs_path = base / so_io::path_t( "gl33_sdf_a_mask.vs.glsl" ) ;
        auto const ps_path = base / so_io::path_t( "gl33_sdf_a_mask.ps.glsl" ) ;

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
            _vss_blit = vs ;
        }

        {
            so_std::string_t ps ;
            psh.wait_for_operation( [&] ( char_cptr_t din, size_t sib, so_io::result res )
            {
                so_log::global::error_and_exit( so_io::no_success( res ),
                    "Pixel shader required" ) ;

                ps = so_std::string_t( din, sib ) ;
            } ) ;
            _pss_blit = ps ;
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
so_gpx::plug_result gl33_sdf_a_plug::on_unload( void_t )
{
    return so_gpx::plug_result::ok ;
}

//*************************************************************************************
so_gpx::plug_result gl33_sdf_a_plug::on_initialize( init_info_cref_t ii )
{
    if( ii.reinit )
    {
        {
            auto const res = this_t::api()->release_program( _prog_color ) ;
            if( so_gpu::no_success( res ) )
            {
                so_log::global_t::error( "[gl33_rects_plug::on_initialize] : release program" ) ;
                return so_gpx::plug_result::failed ;
            }
        }
        {
            auto const res = this_t::api()->release_config( _config_color ) ;
            if( so_gpu::no_success( res ) )
            {
                so_log::global_t::error( "[gl33_rects_plug::on_initialize] : release config" ) ;
                return so_gpx::plug_result::failed ;
            }
        }

        {
            auto const res = this_t::api()->release_program( _prog_mask ) ;
            if( so_gpu::no_success( res ) )
            {
                so_log::global_t::error( "[gl33_rects_plug::on_initialize] : release program" ) ;
                return so_gpx::plug_result::failed ;
            }
        }
        {
            auto const res = this_t::api()->release_config( _config_mask ) ;
            if( so_gpu::no_success( res ) )
            {
                so_log::global_t::error( "[gl33_rects_plug::on_initialize] : release config" ) ;
                return so_gpx::plug_result::failed ;
            }
        }
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
            auto const res = this_t::api()->create_shader( _vs_color ) ;
            if( so_gpu::no_success( res ) )
            {
                so_log::global_t::error( "[gl33_rects_plug::on_initialize] : vertex shader creation" ) ;
                return so_gpx::plug_result::failed ;
            }
        }
        {
            auto const res = this_t::api()->create_shader( _ps_color ) ;
            if( so_gpu::no_success( res ) )
            {
                so_log::global_t::error( "[gl33_rects_plug::on_initialize] : pixel shader creation" ) ;
                return so_gpx::plug_result::failed ;
            }
        }
        {
            auto const res = this_t::api()->create_shader( _vs_mask ) ;
            if( so_gpu::no_success( res ) )
            {
                so_log::global_t::error( "[gl33_rects_plug::on_initialize] : vertex shader creation" ) ;
                return so_gpx::plug_result::failed ;
            }
        }
        {
            auto const res = this_t::api()->create_shader( _ps_mask ) ;
            if( so_gpu::no_success( res ) )
            {
                so_log::global_t::error( "[gl33_rects_plug::on_initialize] : pixel shader creation" ) ;
                return so_gpx::plug_result::failed ;
            }
        }
    }

    {
        auto const res = this_t::api()->create_program( _prog_color ) ;
        if( so_gpu::no_success( res ) )
        {
            so_log::global_t::error( "[gl33_rects_plug::on_initialize] : create program" ) ;
            return so_gpx::plug_result::failed ;
        }
    }
    {
        auto const res = this_t::api()->create_config( _config_color ) ;
        if( so_gpu::no_success( res ) )
        {
            so_log::global_t::error( "[gl33_rects_plug::on_initialize] : create config" ) ;
            return so_gpx::plug_result::failed ;
        }
    }
    {
        _prog_color->set_shader( _vs_color ) ;
        _prog_color->set_shader( _ps_color ) ;
    }
    {
        auto tmp_vs = _vs_color->get_code() ;
        auto tmp_ps = _ps_color->get_code() ;

        _vs_color->set_code( _vss_mix ) ;
        _ps_color->set_code( _pss_mix ) ;

        if( ii.reinit )
        {
            _vss_mix = tmp_vs ;
            _pss_mix = tmp_ps ;
        }


        bool_t suc = true ;
        {
            auto const res = this_t::api()->compile( _vs_color )  ;
            suc = suc && so_gpu::success( res ) ;

        }
        {
            auto const res = this_t::api()->compile( _ps_color )  ;
            suc = suc && so_gpu::success( res ) ;
        }
        {
            auto const res = this_t::api()->link( _prog_color ) ;
            suc = suc && so_gpu::success( res ) ;
        }

        if( so_core::is_not( suc ) && ii.reinit )
        {
            so_log::global_t::error( "[gl33_rects_plug::on_initialize] : "
                "compile/link shaders - using old shaders" ) ;

            _vs_color->set_code( _vss_mix ) ;
            _ps_color->set_code( _pss_mix ) ;
            this_t::api()->compile( _vs_color )  ;
            this_t::api()->compile( _ps_color )  ;
            this_t::api()->link( _prog_color ) ;
        }
        else if( so_core::is_not( suc ) && so_core::is_not( ii.reinit ) )
        {
            so_log::global_t::error( "[gl33_sdf_a_plug::on_initialize] : compile/link shaders" ) ;
            return so_gpx::plug_result::failed ;
        }
    }

    {
        _config_color->set_program( _prog_color ) ;
        _config_color->bind( so_gpu::vertex_attribute::position, "in_pos" ) ;
        _config_color->bind( so_gpu::primitive_type::triangles, _vb, _ib ) ;
        _config_color->add_variable_set( _vars_color ) ;
    }
    
    {
        auto const res = this_t::api()->create_variable( _vars_color ) ;
    }

    // blit

    {
        auto const res = this_t::api()->create_program( _prog_mask ) ;
        if( so_gpu::no_success( res ) )
        {
            so_log::global_t::error( "[gl33_rects_plug::on_initialize] : create program" ) ;
            return so_gpx::plug_result::failed ;
        }
    }
    {
        auto const res = this_t::api()->create_config( _config_mask ) ;
        if( so_gpu::no_success( res ) )
        {
            so_log::global_t::error( "[gl33_rects_plug::on_initialize] : create config" ) ;
            return so_gpx::plug_result::failed ;
        }
    }
    {
        _prog_mask->set_shader( _vs_mask ) ;
        _prog_mask->set_shader( _ps_mask ) ;
    }
    {
        auto tmp_vs = _vs_mask->get_code() ;
        auto tmp_ps = _ps_mask->get_code() ;

        _vs_mask->set_code( _vss_blit ) ;
        _ps_mask->set_code( _pss_blit ) ;

        if( ii.reinit )
        {
            _vss_blit = tmp_vs ;
            _pss_blit = tmp_ps ;
        }


        bool_t suc = true ;
        {
            auto const res = this_t::api()->compile( _vs_mask )  ;
            suc = suc && so_gpu::success( res ) ;

        }
        {
            auto const res = this_t::api()->compile( _ps_mask )  ;
            suc = suc && so_gpu::success( res ) ;
        }
        {
            auto const res = this_t::api()->link( _prog_mask ) ;
            suc = suc && so_gpu::success( res ) ;
        }

        if( so_core::is_not( suc ) && ii.reinit )
        {
            so_log::global_t::error( "[gl33_rects_plug::on_initialize] : "
                "compile/link shaders - using old shaders" ) ;

            _vs_mask->set_code( _vss_blit ) ;
            _ps_mask->set_code( _pss_blit ) ;
            this_t::api()->compile( _vs_mask )  ;
            this_t::api()->compile( _ps_mask )  ;
            this_t::api()->link( _prog_mask ) ;
        }
        else if( so_core::is_not( suc ) && so_core::is_not( ii.reinit ) )
        {
            so_log::global_t::error( "[gl33_sdf_a_plug::on_initialize] : compile/link shaders" ) ;
            return so_gpx::plug_result::failed ;
        }
    }

    {
        _config_mask->set_program( _prog_mask ) ;
        _config_mask->bind( so_gpu::vertex_attribute::position, "in_pos" ) ;
        _config_mask->bind( so_gpu::primitive_type::triangles, _vb, _ib ) ;
        _config_mask->add_variable_set( _vars_mask ) ;
    }

    {
        auto const res = this_t::api()->create_variable( _vars_mask ) ;
    }
   

    return so_gpx::plug_result::ok ;
}

//*************************************************************************************
so_gpx::plug_result gl33_sdf_a_plug::on_release( void_t )
{
    this_t::api()->release_buffer( _vb ) ;
    this_t::api()->release_buffer( _ib ) ;

    this_t::api()->release_shader( _vs_color ) ;
    this_t::api()->release_shader( _ps_color ) ;
    this_t::api()->release_program( _prog_color ) ;
    this_t::api()->release_config( _config_color ) ;
    this_t::api()->release_variable( _vars_color ) ;

    this_t::api()->release_shader( _vs_mask ) ;
    this_t::api()->release_shader( _ps_mask ) ;
    this_t::api()->release_program( _prog_mask ) ;
    this_t::api()->release_config( _config_mask ) ;
    this_t::api()->release_variable( _vars_mask ) ;

    return so_gpx::plug_result::ok ;
}

//*************************************************************************************
so_gpx::plug_result gl33_sdf_a_plug::on_execute( execute_info_cref_t ei )
{
    if( ei.rnd_id == 1 )
    {
        so_gpu::viewport_2d_t fb_dims = so_gpu::viewport_2d_t(0,0,1920,1080) ;// _var_fb_dims->get_data().zw() ;
        
        this_t::api()->set_viewport( so_gpu::viewport_2d_t( 0, 0,
            size_t( fb_dims.get_width() ), size_t( fb_dims.get_height() ) ) ) ;
        
        this_t::api()->load_variable( _vars_mask ) ;
        this_t::api()->execute( so_gpu::render_config_info( _config_mask ) ) ;
        

    }
    else if( ei.rnd_id == 0 )
    {
        so_gpu::viewport_2d_t fb_dims = so_gpu::viewport_2d_t(0,0,1920,1080) ;// _var_fb_dims->get_data().zw() ;

        //this_t::api()->use_framebuffer( nullptr ) ;
        this_t::api()->set_viewport( so_gpu::viewport_2d_t( 0, 0,
            size_t( fb_dims.get_width() ), size_t( fb_dims.get_height() ) ) ) ;

        this_t::api()->load_variable( _vars_color ) ;
        this_t::api()->execute( so_gpu::render_config_info( _config_color ) ) ;
    }

    return so_gpx::plug_result::ok ;
}

//*************************************************************************************
so_gpx::plug_result gl33_sdf_a_plug::on_update( update_info_cref_t )
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
void_t gl33_sdf_a_plug::destroy( void_t )
{
    this_t::destroy( this ) ;
}


