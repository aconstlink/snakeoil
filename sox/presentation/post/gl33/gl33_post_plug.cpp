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
    _vs_mix = so_gpu::vertex_shader_t::create( "vertex shader" ) ;
    _ps_mix = so_gpu::pixel_shader_t::create( "pixel shader" ) ;
    _prog_mix = so_gpu::program_t::create( "program" ) ;
    _config_mix = so_gpu::config_t::create( "config" ) ;    
    _vars_mix = so_gpu::variable_set_t::create("[gl33_post_plug::gl33_post_plug] : varset 0") ;

    _vs_blit = so_gpu::vertex_shader_t::create( "vertex shader" ) ;
    _ps_blit = so_gpu::pixel_shader_t::create( "pixel shader" ) ;
    _prog_blit = so_gpu::program_t::create( "program" ) ;
    _config_blit = so_gpu::config_t::create( "config" ) ;
    _vars_blit = so_gpu::variable_set_t::create("[gl33_post_plug::gl33_post_plug] : varset 0") ;

    _sd = sd ;
}

//*************************************************************************************
gl33_post_plug::gl33_post_plug( this_rref_t rhv ) : 
    this_base_t( std::move(rhv) )
{
    so_move_member_ptr( _vb, rhv ) ;
    so_move_member_ptr( _ib, rhv ) ;
    so_move_member_ptr( _vs_mix, rhv ) ;
    so_move_member_ptr( _ps_mix, rhv ) ;
    so_move_member_ptr( _prog_mix, rhv ) ;
    so_move_member_ptr( _config_mix, rhv ) ;
    so_move_member_ptr( _vars_mix, rhv ) ;

    so_move_member_ptr( _vs_blit, rhv ) ;
    so_move_member_ptr( _ps_blit, rhv ) ;
    so_move_member_ptr( _prog_blit, rhv ) ;
    so_move_member_ptr( _config_blit, rhv ) ;
    so_move_member_ptr( _vars_blit, rhv ) ;

    so_move_member_ptr( _sd, rhv ) ;
}

//*************************************************************************************
gl33_post_plug::~gl33_post_plug( void_t )
{
    if( so_core::is_not_nullptr( _vb ) )
        _vb->destroy() ;

    if( so_core::is_not_nullptr( _ib ) )
        _ib->destroy() ;

    if( so_core::is_not_nullptr( _vs_mix ) )
        so_gpu::vertex_shader_t::destroy( _vs_mix ) ;

    if( so_core::is_not_nullptr( _ps_mix ) )
        so_gpu::pixel_shader_t::destroy( _ps_mix ) ;

    if( so_core::is_not_nullptr( _prog_mix ) )
        so_gpu::program_t::destroy( _prog_mix ) ;

    if( so_core::is_not_nullptr( _config_mix ) )
        so_gpu::config_t::destroy( _config_mix ) ;

    if( so_core::is_not_nullptr( _vars_mix ) )
        so_gpu::variable_set_t::destroy( _vars_mix ) ;

    if( so_core::is_not_nullptr( _vs_blit ) )
        so_gpu::vertex_shader_t::destroy( _vs_blit ) ;

    if( so_core::is_not_nullptr( _ps_blit ) )
        so_gpu::pixel_shader_t::destroy( _ps_blit ) ;

    if( so_core::is_not_nullptr( _prog_blit ) )
        so_gpu::program_t::destroy( _prog_blit ) ;

    if( so_core::is_not_nullptr( _config_blit ) )
        so_gpu::config_t::destroy( _config_blit ) ;

    if( so_core::is_not_nullptr( _vars_blit ) )
        so_gpu::variable_set_t::destroy( _vars_blit ) ;

    
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
    auto const base = sox::find_shader_path("/presentation/post/gl33/shader/") ;

    // load shader mix
    {
        auto const vs_path = base / so_io::path_t( "gl33_post_mix.vs.glsl" ) ;
        auto const ps_path = base / so_io::path_t( "gl33_post_mix.ps.glsl" ) ;

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

    // load shader blit
    {
        auto const vs_path = base / so_io::path_t( "gl33_post_blit.vs.glsl" ) ;
        auto const ps_path = base / so_io::path_t( "gl33_post_blit.ps.glsl" ) ;

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
            auto const res = this_t::api()->release_program( _prog_mix ) ;
            if( so_gpu::no_success( res ) )
            {
                so_log::global_t::error( "[gl33_rects_plug::on_initialize] : release program" ) ;
                return so_gpx::plug_result::failed ;
            }
        }
        {
            auto const res = this_t::api()->release_config( _config_mix ) ;
            if( so_gpu::no_success( res ) )
            {
                so_log::global_t::error( "[gl33_rects_plug::on_initialize] : release config" ) ;
                return so_gpx::plug_result::failed ;
            }
        }

        {
            auto const res = this_t::api()->release_program( _prog_blit ) ;
            if( so_gpu::no_success( res ) )
            {
                so_log::global_t::error( "[gl33_rects_plug::on_initialize] : release program" ) ;
                return so_gpx::plug_result::failed ;
            }
        }
        {
            auto const res = this_t::api()->release_config( _config_blit ) ;
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
            auto const res = this_t::api()->create_shader( _vs_mix ) ;
            if( so_gpu::no_success( res ) )
            {
                so_log::global_t::error( "[gl33_rects_plug::on_initialize] : vertex shader creation" ) ;
                return so_gpx::plug_result::failed ;
            }
        }
        {
            auto const res = this_t::api()->create_shader( _ps_mix ) ;
            if( so_gpu::no_success( res ) )
            {
                so_log::global_t::error( "[gl33_rects_plug::on_initialize] : pixel shader creation" ) ;
                return so_gpx::plug_result::failed ;
            }
        }
        {
            auto const res = this_t::api()->create_shader( _vs_blit ) ;
            if( so_gpu::no_success( res ) )
            {
                so_log::global_t::error( "[gl33_rects_plug::on_initialize] : vertex shader creation" ) ;
                return so_gpx::plug_result::failed ;
            }
        }
        {
            auto const res = this_t::api()->create_shader( _ps_blit ) ;
            if( so_gpu::no_success( res ) )
            {
                so_log::global_t::error( "[gl33_rects_plug::on_initialize] : pixel shader creation" ) ;
                return so_gpx::plug_result::failed ;
            }
        }
    }

    {
        auto const res = this_t::api()->create_program( _prog_mix ) ;
        if( so_gpu::no_success( res ) )
        {
            so_log::global_t::error( "[gl33_rects_plug::on_initialize] : create program" ) ;
            return so_gpx::plug_result::failed ;
        }
    }
    {
        auto const res = this_t::api()->create_config( _config_mix ) ;
        if( so_gpu::no_success( res ) )
        {
            so_log::global_t::error( "[gl33_rects_plug::on_initialize] : create config" ) ;
            return so_gpx::plug_result::failed ;
        }
    }
    {
        _prog_mix->set_shader( _vs_mix ) ;
        _prog_mix->set_shader( _ps_mix ) ;
    }
    {
        auto tmp_vs = _vs_mix->get_code() ;
        auto tmp_ps = _ps_mix->get_code() ;

        _vs_mix->set_code( _vss_mix ) ;
        _ps_mix->set_code( _pss_mix ) ;

        if( ii.reinit )
        {
            _vss_mix = tmp_vs ;
            _pss_mix = tmp_ps ;
        }


        bool_t suc = true ;
        {
            auto const res = this_t::api()->compile( _vs_mix )  ;
            suc = suc && so_gpu::success( res ) ;

        }
        {
            auto const res = this_t::api()->compile( _ps_mix )  ;
            suc = suc && so_gpu::success( res ) ;
        }
        {
            auto const res = this_t::api()->link( _prog_mix ) ;
            suc = suc && so_gpu::success( res ) ;
        }

        if( so_core::is_not( suc ) && ii.reinit )
        {
            so_log::global_t::error( "[gl33_rects_plug::on_initialize] : "
                "compile/link shaders - using old shaders" ) ;

            _vs_mix->set_code( _vss_mix ) ;
            _ps_mix->set_code( _pss_mix ) ;
            this_t::api()->compile( _vs_mix )  ;
            this_t::api()->compile( _ps_mix )  ;
            this_t::api()->link( _prog_mix ) ;
        }
        else if( so_core::is_not( suc ) && so_core::is_not( ii.reinit ) )
        {
            so_log::global_t::error( "[gl33_post_plug::on_initialize] : compile/link shaders" ) ;
            return so_gpx::plug_result::failed ;
        }
    }

    {
        _config_mix->set_program( _prog_mix ) ;
        _config_mix->bind( so_gpu::vertex_attribute::position, "in_pos" ) ;
        _config_mix->bind( so_gpu::primitive_type::triangles, _vb, _ib ) ;
        _config_mix->add_variable_set( _vars_mix ) ;
    }
    // fb0 color
    {
        so_gpu::texture_ptr_t color_ptr ;
        so_std::string_t const name = _sd->fb0_name + "_color_0" ;

        so_gpu::gpu_manager::tx2d_manager_t::handle_t hnd ;
        auto const res = ii.mgr->get_tx2d_mgr()->acquire(
            name, "[gl33_post_plug::on_initialize]", hnd ) ;
        if( so_core::is_not( res ) )
        {
            so_log::global::error( "[gl33_post_plug::on_initialize] : "
                "color texture name incorrect" ) ;
            return so_gpx::plug_result::failed ;
        }

        color_ptr = hnd.get_ptr() ;

        so_gpu::iimage_2d_ptr_t ptr = dynamic_cast< so_gpu::iimage_2d_ptr_t >( color_ptr->get_image() );
        if( so_core::is_not_nullptr( ptr ) )
        {
            //_bb_dims = so_math::vec2f_t( float_t( ptr->get_width() ), float_t( ptr->get_height() ) ) ;
            _vars_blit->bind_texture( "u_smp_color", color_ptr ) ;
        }
        else
        {
            so_log::global::error( "[gl33_post_plug::on_initialize] : "
                "color texture is not a 2d image" ) ;
        }

    }

    // fb1 color
    {
        so_gpu::texture_ptr_t color_ptr ;
        so_std::string_t const name = _sd->fb1_name + "_color_0" ;

        so_gpu::gpu_manager::tx2d_manager_t::handle_t hnd ;
        auto const res = ii.mgr->get_tx2d_mgr()->acquire(
            name, "[gl33_post_plug::on_initialize]", hnd ) ;
        if( so_core::is_not( res ) )
        {
            so_log::global::error( "[gl33_post_plug::on_initialize] : "
                "color texture name incorrect" ) ;
            return so_gpx::plug_result::failed ;
        }

        color_ptr = hnd.get_ptr() ;

        so_gpu::iimage_2d_ptr_t ptr = dynamic_cast< so_gpu::iimage_2d_ptr_t >( color_ptr->get_image() );
        if( so_core::is_not_nullptr( ptr ) )
        {
            //_bb_dims = so_math::vec2f_t( float_t( ptr->get_width() ), float_t( ptr->get_height() ) ) ;
        }
        else
        {
            so_log::global::error( "[gl33_post_plug::on_initialize] : "
                "color texture is not a 2d image" ) ;
        }

    }

    // fbx color
    {
        so_gpu::texture_ptr_t color_ptr ;
        so_std::string_t const name = _sd->fbx_name + "_color_0" ;

        so_gpu::gpu_manager::tx2d_manager_t::handle_t hnd ;
        auto const res = ii.mgr->get_tx2d_mgr()->acquire(
            name, "[gl33_post_plug::on_initialize]", hnd ) ;
        if( so_core::is_not( res ) )
        {
            so_log::global::error( "[gl33_post_plug::on_initialize] : "
                "color texture name incorrect" ) ;
            return so_gpx::plug_result::failed ;
        }

        color_ptr = hnd.get_ptr() ;

        so_gpu::iimage_2d_ptr_t ptr = dynamic_cast< so_gpu::iimage_2d_ptr_t >( color_ptr->get_image() );
        if( so_core::is_not_nullptr( ptr ) )
        {
            //_bb_dims = so_math::vec2f_t( float_t( ptr->get_width() ), float_t( ptr->get_height() ) ) ;
            _vars_mix->bind_texture( "u_smp_color_x", color_ptr ) ;
        }
        else
        {
            so_log::global::error( "[gl33_post_plug::on_initialize] : "
                "color texture is not a 2d image" ) ;
        }

    }

    // fbm color
    {
        so_gpu::texture_ptr_t color_ptr ;
        so_std::string_t const name = _sd->fbm_name + "_color_0" ;

        so_gpu::gpu_manager::tx2d_manager_t::handle_t hnd ;
        auto const res = ii.mgr->get_tx2d_mgr()->acquire(
            name, "[gl33_post_plug::on_initialize]", hnd ) ;
        if( so_core::is_not( res ) )
        {
            so_log::global::error( "[gl33_post_plug::on_initialize] : "
                "color texture name incorrect" ) ;
            return so_gpx::plug_result::failed ;
        }

        color_ptr = hnd.get_ptr() ;

        so_gpu::iimage_2d_ptr_t ptr = dynamic_cast< so_gpu::iimage_2d_ptr_t >( color_ptr->get_image() );
        if( so_core::is_not_nullptr( ptr ) )
        {
            //_bb_dims = so_math::vec2f_t( float_t( ptr->get_width() ), float_t( ptr->get_height() ) ) ;
        }
        else
        {
            so_log::global::error( "[gl33_post_plug::on_initialize] : "
                "color texture is not a 2d image" ) ;
        }

    }

    {
        auto const res = this_t::api()->create_variable( _vars_mix ) ;
    }

    // blit

    {
        auto const res = this_t::api()->create_program( _prog_blit ) ;
        if( so_gpu::no_success( res ) )
        {
            so_log::global_t::error( "[gl33_rects_plug::on_initialize] : create program" ) ;
            return so_gpx::plug_result::failed ;
        }
    }
    {
        auto const res = this_t::api()->create_config( _config_blit ) ;
        if( so_gpu::no_success( res ) )
        {
            so_log::global_t::error( "[gl33_rects_plug::on_initialize] : create config" ) ;
            return so_gpx::plug_result::failed ;
        }
    }
    {
        _prog_blit->set_shader( _vs_blit ) ;
        _prog_blit->set_shader( _ps_blit ) ;
    }
    {
        auto tmp_vs = _vs_blit->get_code() ;
        auto tmp_ps = _ps_blit->get_code() ;

        _vs_blit->set_code( _vss_blit ) ;
        _ps_blit->set_code( _pss_blit ) ;

        if( ii.reinit )
        {
            _vss_blit = tmp_vs ;
            _pss_blit = tmp_ps ;
        }


        bool_t suc = true ;
        {
            auto const res = this_t::api()->compile( _vs_blit )  ;
            suc = suc && so_gpu::success( res ) ;

        }
        {
            auto const res = this_t::api()->compile( _ps_blit )  ;
            suc = suc && so_gpu::success( res ) ;
        }
        {
            auto const res = this_t::api()->link( _prog_blit ) ;
            suc = suc && so_gpu::success( res ) ;
        }

        if( so_core::is_not( suc ) && ii.reinit )
        {
            so_log::global_t::error( "[gl33_rects_plug::on_initialize] : "
                "compile/link shaders - using old shaders" ) ;

            _vs_blit->set_code( _vss_blit ) ;
            _ps_blit->set_code( _pss_blit ) ;
            this_t::api()->compile( _vs_blit )  ;
            this_t::api()->compile( _ps_blit )  ;
            this_t::api()->link( _prog_blit ) ;
        }
        else if( so_core::is_not( suc ) && so_core::is_not( ii.reinit ) )
        {
            so_log::global_t::error( "[gl33_post_plug::on_initialize] : compile/link shaders" ) ;
            return so_gpx::plug_result::failed ;
        }
    }

    {
        _config_blit->set_program( _prog_blit ) ;
        _config_blit->bind( so_gpu::vertex_attribute::position, "in_pos" ) ;
        _config_blit->bind( so_gpu::primitive_type::triangles, _vb, _ib ) ;
        _config_blit->add_variable_set( _vars_blit ) ;
    }
    
    {
        so_gpu::texture_ptr_t color_ptr ;
        so_std::string_t const name = _sd->fb0_name + "_color_0" ;

        so_gpu::gpu_manager::tx2d_manager_t::handle_t hnd ;
        auto const res = ii.mgr->get_tx2d_mgr()->acquire(
            name, "[gl33_post_plug::on_initialize]", hnd ) ;
        if( so_core::is_not( res ) )
        {
            so_log::global::error( "[gl33_post_plug::on_initialize] : "
                "color texture name incorrect" ) ;
            return so_gpx::plug_result::failed ;
        }

        color_ptr = hnd.get_ptr() ;

        so_gpu::iimage_2d_ptr_t ptr = dynamic_cast< so_gpu::iimage_2d_ptr_t >( color_ptr->get_image() );
        if( so_core::is_not_nullptr( ptr ) )
        {
            //_bb_dims = so_math::vec2f_t( float_t( ptr->get_width() ), float_t( ptr->get_height() ) ) ;
        }
        else
        {
            so_log::global::error( "[gl33_post_plug::on_initialize] : "
                "color texture is not a 2d image" ) ;
        }

    }


    {
        auto const res = this_t::api()->create_variable( _vars_blit ) ;
    }
   

    return so_gpx::plug_result::ok ;
}

//*************************************************************************************
so_gpx::plug_result gl33_post_plug::on_release( void_t )
{
    this_t::api()->release_buffer( _vb ) ;
    this_t::api()->release_buffer( _ib ) ;

    this_t::api()->release_shader( _vs_mix ) ;
    this_t::api()->release_shader( _ps_mix ) ;
    this_t::api()->release_program( _prog_mix ) ;
    this_t::api()->release_config( _config_mix ) ;
    this_t::api()->release_variable( _vars_mix ) ;

    this_t::api()->release_shader( _vs_blit ) ;
    this_t::api()->release_shader( _ps_blit ) ;
    this_t::api()->release_program( _prog_blit ) ;
    this_t::api()->release_config( _config_blit ) ;
    this_t::api()->release_variable( _vars_blit ) ;

    return so_gpx::plug_result::ok ;
}

//*************************************************************************************
so_gpx::plug_result gl33_post_plug::on_execute( execute_info_cref_t ei )
{
    if( ei.rnd_id == 0 )
    {
        so_gpu::viewport_2d_t fb_dims = so_gpu::viewport_2d_t(0,0,1920,1080) ;// _var_fb_dims->get_data().zw() ;

        this_t::api()->use_framebuffer( nullptr ) ;
        this_t::api()->set_viewport( so_gpu::viewport_2d_t( 0, 0,
            size_t( fb_dims.get_width() ), size_t( fb_dims.get_height() ) ) ) ;

        this_t::api()->load_variable( _vars_blit ) ;
        this_t::api()->execute( so_gpu::render_config_info( _config_blit ) ) ;
    }
    else if( ei.rnd_id == 1 )
    {
        so_gpu::viewport_2d_t fb_dims = so_gpu::viewport_2d_t(0,0,1920,1080) ;// _var_fb_dims->get_data().zw() ;

        this_t::api()->use_framebuffer( nullptr ) ;
        this_t::api()->set_viewport( so_gpu::viewport_2d_t( 0, 0,
            size_t( fb_dims.get_width() ), size_t( fb_dims.get_height() ) ) ) ;

        this_t::api()->load_variable( _vars_mix ) ;
        this_t::api()->execute( so_gpu::render_config_info( _config_mix ) ) ;
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


