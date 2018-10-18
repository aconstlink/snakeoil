//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "gl33_imgui_plug.h"

#include "../../system/system.h"

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

using namespace so_imgui ;
using namespace so_imgui::so_gl33 ;

//*************************************************************************************
gl33_imgui_plug::gl33_imgui_plug( imgui_t::shared_data_ptr_t sd,
    so_gpu::so_gl::igl_33_api_ptr_t aptr ) : this_base_t( aptr )
{
    _sd = sd ;

    _vb = vb_t::create( "vertex buffer" ) ;
    _ib = ib_t::create( "index buffer" ) ;
    _vs = so_gpu::vertex_shader_t::create( "vertex shader" ) ;
    _ps = so_gpu::pixel_shader_t::create( "pixel shader" ) ;
    _prog = so_gpu::program_t::create( "program" ) ;
    _config = so_gpu::config_t::create( "config" ) ;
}

//*************************************************************************************
gl33_imgui_plug::gl33_imgui_plug( this_rref_t rhv ) :
    this_base_t( std::move( rhv ) )
{
    so_move_member_ptr( _sd, rhv ) ;

    so_move_member_ptr( _vb, rhv ) ;
    so_move_member_ptr( _ib, rhv ) ;
    so_move_member_ptr( _vs, rhv ) ;
    so_move_member_ptr( _ps, rhv ) ;
    so_move_member_ptr( _prog, rhv ) ;
    so_move_member_ptr( _config, rhv ) ;

    _render_datas = std::move( rhv._render_datas ) ;
    _tname_to_id = std::move( rhv._tname_to_id ) ;
}

//*************************************************************************************
gl33_imgui_plug::~gl33_imgui_plug( void_t )
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

    for( auto & d : _render_datas )
    {
        if( so_core::is_not_nullptr( d.vars ) )
            d.vars->destroy() ;

        // at the moment, if the image is nullptr, the
        // texture is a reference
        if( so_core::is_not_nullptr( d.image_ptr ) )
        {
            if( so_core::is_not_nullptr( d.image_ptr ) )
                d.image_ptr->destroy() ;
            if( so_core::is_not_nullptr( d.tx_ptr ) )
                d.tx_ptr->destroy() ;
        }
    }
}

//*************************************************************************************
gl33_imgui_plug::this_ptr_t gl33_imgui_plug::create(
    this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_imgui::memory::alloc( std::move( rhv ), p ) ;
}

//*************************************************************************************
void_t gl33_imgui_plug::destroy( this_ptr_t ptr )
{
    so_imgui::memory::dealloc( ptr ) ;
}

//*************************************************************************************
so_gpx::plug_result gl33_imgui_plug::on_load( load_info_cref_t li )
{
    if( li.reload )
    {
        so_log::global_t::warning( "[gl33_imgui_plug::on_load] : reload not supported" ) ;
        return so_gpx::plug_result::failed ;
    }

    {
        const so_std::string_t vs = so_std::string_t(
            "#version 150\n"
            "uniform mat4 u_proj;\n"
            "in vec2 in_pos;\n"
            "in vec2 in_uv;\n"
            "in vec4 in_color;\n"
            "out vec2 var_fraguv;\n"
            "out vec4 var_color;\n"
            "void main()\n"
            "{\n"
            "	var_fraguv = in_uv;\n"
            "	var_color = in_color;\n"
            "	gl_Position = u_proj * vec4(in_pos.xy,0,1);\n"
            "}\n" );

        _vs->set_code( vs ) ;
    }

    {
        const so_std::string_t ps = so_std::string_t(
            "#version 150\n"
            "uniform sampler2D u_tex;\n"
            "in vec2 var_fraguv;\n"
            "in vec4 var_color;\n"
            "out vec4 out_color;\n"
            "void main()\n"
            "{\n"
            "	out_color = var_color * texture( u_tex, var_fraguv.st);\n"
            "	//out_color = vec4(var_fraguv.st, 0.0,1.0);\n"
            "}\n" ) ;
        _ps->set_code( ps ) ;
    }


    {
        _vb->add_layout_element<so_math::vec2f_t>( so_gpu::vertex_attribute::position ) ;
        _vb->add_layout_element<so_math::vec2f_t>( so_gpu::vertex_attribute::texcoord0 ) ;
        _vb->add_layout_element<so_math::vec4f_t>( so_gpu::vertex_attribute::color0 ) ;
        _vb->resize( 100 ) ;
    }
    {
        _ib->resize( 100 ) ;
    }
    

    return so_gpx::plug_result::ok ;
}

//*************************************************************************************
so_gpx::plug_result gl33_imgui_plug::on_unload( void_t )
{
    return so_gpx::plug_result::ok ;
}

//*************************************************************************************
so_gpx::plug_result gl33_imgui_plug::on_initialize( init_info_cref_t ii )
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
        _config->bind( so_gpu::vertex_attribute::texcoord0, "in_uv" ) ;
        _config->bind( so_gpu::vertex_attribute::color0, "in_color" ) ;

        _config->bind( so_gpu::primitive_type::triangles, _vb, _ib ) ;
    }
#if 0
    {
        so_gpu::texture_ptr_t color_ptr ;

        /*{
            so_gpu::gpu_manager::tx2d_manager_t::handle_t hnd ;
            auto const res = ii.mgr->get_tx2d_mgr()->acquire(
                _ud_ptr->color_name, "", hnd ) ;
            so_log::global::error_and_exit( so_core::is_not( res ), "][ : color texture name incorrect" ) ;
            color_ptr = hnd.get_ptr() ;
        }*/

        auto const res = this_t::api()->create_variable( _vars_0 ) ;
        _vars_0->bind_texture( "u_tex", color_ptr ) ;
    }
#endif

    // do first variable set for glyph rendering
    {
        this_t::render_data_t rd ;
        
        // 1. create stuff
        {
            int_t const w = int_t(_sd->width) ;
            int_t const h = int_t(_sd->height) ;
            
            color_image_t img( w, h ) ;

            for( int_t y = 0; y <h; ++y )
            {
                for( int_t x = 0; x <w ; ++x )
                {
                    int_t const wi = y * w + x ;
                    int_t const ri = wi << 2 ;

                    uchar_t const r = _sd->pixels[ ri + 0 ] ;
                    uchar_t const g = _sd->pixels[ ri + 1 ] ;
                    uchar_t const b = _sd->pixels[ ri + 2 ] ;
                    uchar_t const a = _sd->pixels[ ri + 3 ] ;

                    img.set_pixel( wi, so_math::vec4uc_t( r, g, b, a ) )  ;
                }
            }

            rd.image_ptr = color_image_t::create( std::move( img ),
                "[gl33_imgui_plug] : glyph image" ) ;

            this_t::api()->create_image( rd.image_ptr ) ;
            {
                this_t::api()->alloc_image_memory( rd.image_ptr,
                    so_gpu::image_alloc_info_t( true ) ) ;
            }

            rd.tx_ptr = so_gpu::texture_2d_t::create( so_gpu::texture_2d_t( rd.image_ptr ),
                "[gl33_imgui_plug] : glyph image texture" ) ;

            this_t::api()->create_texture( rd.tx_ptr ) ;

        }
       
        // 2. create vars and bind
        {
            rd.vars = so_gpu::variable_set_t::create( "[gl33_imgui_plug] : varset" ) ;
            this_t::api()->create_variable( rd.vars ) ;
            rd.vars->bind_data<so_math::mat4f_t>( "u_proj", &_proj/*&_sd->proj*/ ) ;
            rd.vars->bind_texture( "u_tex", rd.tx_ptr ) ;

            _config->add_variable_set( rd.vars ) ;
        }

        // insert framebuffers and fb textures
        {
            ii.mgr->get_tx2d_mgr()->insert( "imgui_texture", rd.tx_ptr ) ;
        }

        _render_datas.push_back( rd ) ;
    }

    _gpu_mgr = ii.mgr ;

    return so_gpx::plug_result::ok ;
}

//*************************************************************************************
so_gpx::plug_result gl33_imgui_plug::on_release( void_t )
{
    this_t::api()->release_buffer( _vb ) ;
    this_t::api()->release_buffer( _ib ) ;
    this_t::api()->release_shader( _vs ) ;
    this_t::api()->release_shader( _ps ) ;
    this_t::api()->release_program( _prog ) ;
    this_t::api()->release_config( _config ) ;

    for( auto & r : _render_datas )
    {
        this_t::api()->release_variable( r.vars ) ;
        this_t::api()->release_image( r.image_ptr ) ;
        this_t::api()->release_texture( r.tx_ptr ) ;
    }

    return so_gpx::plug_result::ok ;
}

//*************************************************************************************
so_gpx::plug_result gl33_imgui_plug::on_execute( execute_info_cref_t )
{
    this_t::api()->enable( so_gpu::render_state::blend ) ;
    this_t::api()->set_state( so_gpu::blend_factor::src_alpha, so_gpu::blend_factor::one_minus_src_alpha ) ;
    this_t::api()->enable( so_gpu::render_state::scissor_test ) ;

    for( auto & dc : _dcs )
    {
        auto & rd = _render_datas[ dc.render_data_id ] ;

        this_t::api()->load_variable( rd.vars ) ;
        this_t::api()->set_scissor( dc.scissor.x(), dc.scissor.y() , dc.scissor.z() , dc.scissor.w() ) ;
        this_t::api()->execute( so_gpu::render_config_info( _config, dc.render_data_id,
            dc.index_offset, dc.num_elems ) ) ;
    }
    this_t::api()->disable( so_gpu::render_state::scissor_test ) ;
    this_t::api()->disable( so_gpu::render_state::blend ) ;
    return so_gpx::plug_result::ok ;
}

//*************************************************************************************
so_gpx::plug_result gl33_imgui_plug::on_update( update_info_cref_t )
{
    ImGuiIO& io = ImGui::GetIO();

    // make projection matrix
    {
        // from imgui example code
        so_math::vec4f_t vcX( 2.0f / io.DisplaySize.x, 0.0f, 0.0f, 0.0f ) ;
        so_math::vec4f_t vcY( 0.0f, 2.0f / -io.DisplaySize.y, 0.0f, 0.0f ) ;
        so_math::vec4f_t vcZ( 0.0f, 0.0f, -1.0f, 0.0f ) ;
        so_math::vec4f_t vcW( -1.0f, 1.0f, 0.0f, 1.0f ) ;

        _proj.set_column( 0, vcX ) ;
        _proj.set_column( 1, vcY ) ;
        _proj.set_column( 2, vcZ ) ;
        _proj.set_column( 3, vcW ) ;
    }

    _dcs.clear() ;

    ImDrawData * dd = _sd->dd ;
    if( so_core::is_nullptr( dd ) )
        return so_gpx::plug_result::ok ;

    
    
    int fb_width = ( int ) ( io.DisplaySize.x * io.DisplayFramebufferScale.x );
    int fb_height = ( int ) ( io.DisplaySize.y * io.DisplayFramebufferScale.y );
    if( fb_width == 0 || fb_height == 0 ) return so_gpx::plug_result::ok ;

    dd->ScaleClipRects( io.DisplayFramebufferScale );
    
    bool_t vb_gpu_alloc = true ;
    bool_t ib_gpu_alloc = true ;

    // copy data
    {
        size_t num_vertices = 0 ;
        size_t num_indices = 0 ;
        size_t num_drawc = 0 ;
        
        // 1. determine size
        { 
            for( size_t n = 0; n < size_t( dd->CmdListsCount ); n++ )
            {
                const ImDrawList* cmd_list = dd->CmdLists[ n ];
                num_vertices += size_t( cmd_list->VtxBuffer.Size ) ;
                num_indices += size_t( cmd_list->IdxBuffer.Size ) ;
                num_drawc += size_t( cmd_list->CmdBuffer.Size ) ;
            }

            vb_gpu_alloc = _vb->get_num_elements() < num_vertices ;
            ib_gpu_alloc = _ib->get_num_elements() < num_indices ;

            _dcs.resize( num_drawc ) ;
            _vb->resize( num_vertices ) ;
            _ib->resize( num_indices ) ;
        }

        // 2. copy data
        {
            size_t voff = 0 ;
            size_t ioff = 0 ;

            for( size_t n = 0; n < size_t(dd->CmdListsCount); n++ )
            {
                const ImDrawList* cmd_list = dd->CmdLists[ n ];
                for( size_t i = 0; i < size_t( cmd_list->VtxBuffer.Size ); ++i )
                {
                    ImDrawVert & iv = cmd_list->VtxBuffer.Data[ i ] ;
                    auto const color = ImColor( iv.col ).Value ;

                    this_t::vertex v ;
                    v.pos = so_math::vec2f_t(iv.pos.x, iv.pos.y) ;
                    v.uv = so_math::vec2f_t(iv.uv.x, iv.uv.y) ; 
                    v.color = so_math::vec4f_t( color.x, color.y, color.z, color.w ) ;
                    _vb->set_vertex( voff + i, v ) ;
                }
                for( size_t i = 0; i < size_t( cmd_list->IdxBuffer.Size ) ; ++i )
                {
                    _ib->set_index( ioff + i, deduced_type_t::type_t(voff) + 
                        deduced_type_t::type_t(cmd_list->IdxBuffer.Data[i]) ) ;
                }
                voff += size_t( cmd_list->VtxBuffer.Size ) ;
                ioff += size_t( cmd_list->IdxBuffer.Size ) ;
            }
        }

        // 3. store actual draw commands
        {
            uint_t offset = 0 ;
            size_t dc_index = 0 ;
            for( size_t n = 0; n < size_t( dd->CmdListsCount ); n++ )
            {
                const ImDrawList* cmd_list = dd->CmdLists[ n ];
                for( int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++ )
                {
                    const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[ cmd_i ];

                    if( size_t( pcmd->TextureId ) == 1 )
                    {
                        this_t::draw_command_t dc ;
                        dc.index_offset = offset  ;
                        dc.num_elems = pcmd->ElemCount  ;
                        dc.render_data_id = 0 ;
                        dc.scissor = so_math::vec4ui_t( 
                            uint_t(pcmd->ClipRect.x), uint_t( fb_height - pcmd->ClipRect.w ),
                            uint_t( pcmd->ClipRect.z - pcmd->ClipRect.x ),
                            uint_t( pcmd->ClipRect.w - pcmd->ClipRect.y ) ) ;
                        _dcs[ dc_index] = dc ;
                        ++dc_index ;
                    }
                    else
                    {
                        size_t rd_id = 0 ;
                        so_imgui::system_t::imgui_texture_data_ptr_t tdp =
                            so_imgui::system_t::imgui_texture_data_ptr_t(pcmd->TextureId) ;
                        {
                            auto const iter = _tname_to_id.find( tdp->name ) ;
                            if( iter == _tname_to_id.end() )
                            {
                                so_resource::handle<so_gpu::texture_2d_t> hnd ;
                                bool_t tvalid = _gpu_mgr->get_tx2d_mgr()->acquire( tdp->name,
                                    "[gl33_imgui_plug::on_update]", hnd ) ;

                                if( tvalid )
                                {
                                    this_t::render_data_t rd = _render_datas[ 0 ] ;
                                    if( tvalid )
                                    {
                                        rd.image_ptr = nullptr ;
                                        rd.tx_ptr = hnd.get_ptr() ;
                                        rd.tx_ptr->set_texture_filter(
                                            so_gpu::texture_filter_type::nearest, 
                                            so_gpu::texture_filter_type::nearest ) ;

                                        rd.vars = so_gpu::variable_set_t::create(
                                            "[gl33_imgui_plug] : varset" ) ;
                                        this_t::api()->create_variable( rd.vars ) ;
                                        rd.vars->bind_data<so_math::mat4f_t>( "u_proj", &_proj/*&_sd->proj*/ ) ;
                                        rd.vars->bind_texture( "u_tex", rd.tx_ptr ) ;

                                        _config->add_variable_set( rd.vars ) ;
                                    }

                                    so_log::global_t::error( so_core::is_not( tvalid ),
                                        "[gl33_imgui_plug::on_update] : "
                                        "texture name does not exist: " + tdp->name ) ;

                                    _render_datas.push_back( rd ) ;
                                    _tname_to_id[ tdp->name ] = _render_datas.size() - 1 ;

                                    rd_id = _render_datas.size() - 1 ;
                                }
                            }
                            else
                            {
                                rd_id = iter->second ;
                            }
                        }

                        this_t::draw_command_t dc ;
                        dc.index_offset = offset  ;
                        dc.num_elems = pcmd->ElemCount  ;
                        dc.render_data_id = rd_id ;
                        dc.scissor = so_math::vec4ui_t(
                            uint_t( pcmd->ClipRect.x ), uint_t( fb_height - pcmd->ClipRect.w ),
                            uint_t( pcmd->ClipRect.z - pcmd->ClipRect.x ),
                            uint_t( pcmd->ClipRect.w - pcmd->ClipRect.y ) ) ;
                        _dcs[ dc_index ] = dc ;
                        ++dc_index ;
                    }
                    
                    offset += pcmd->ElemCount ;
                }
            }
            if( dc_index != _dcs.size() )
                _dcs.resize( dc_index ) ;
        }
    }

    if( vb_gpu_alloc )
    {
        auto const res = this_t::api()->alloc_buffer_memory( _vb,
            so_gpu::memory_alloc_info( true ) ) ;
    }
    else
    {
        this_t::api()->load_buffer_memory( _vb, 
            so_gpu::memory_load_info() ) ;
    }

    if( ib_gpu_alloc )
    {
        auto const res = this_t::api()->alloc_buffer_memory( _ib,
            so_gpu::memory_alloc_info( true ) ) ;
    }
    else
    {
        this_t::api()->load_buffer_memory( _ib,
            so_gpu::memory_load_info() ) ;
    }

    // copy changed gpu buffer data if required here
    return so_gpx::plug_result::need_transfer ;
}

//*************************************************************************************
void_t gl33_imgui_plug::destroy( void_t )
{
    this_t::destroy( this ) ;
}

//*************************************************************************************