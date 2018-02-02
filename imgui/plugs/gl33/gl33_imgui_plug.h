//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "../imgui.h"

#include <snakeoil/gpx/plug/plug.h>

#include <snakeoil/gpu/protos.h>
#include <snakeoil/gpu/image/color_image_2d.hpp>
#include <snakeoil/gpu/buffer/vertex_buffer.hpp>
#include <snakeoil/gpu/buffer/index_buffer.hpp>
#include <snakeoil/gpu/buffer/data_buffer.hpp>

#include <snakeoil/math/vector/vector4.hpp>

namespace so_imgui
{
    namespace so_gl33
    {
        class SNAKEOIL_IMGUI_API gl33_imgui_plug :
            public so_gpx::plug<so_gpu::so_gl::igl_33_api >
        {
            typedef so_gpx::plug<so_gpu::so_gl::igl_33_api > this_base_t ;
            so_this_typedefs( gl33_imgui_plug ) ;

        private: // general

            imgui_t::shared_data_ptr_t _sd = nullptr ;

        private: // rendering 

            struct draw_command
            {
                // offset into the big index buffer
                uint_t index_offset ;
                uint_t num_elems ;
                size_t render_data_id ;
                so_math::vec4ui_t scissor ;

            };
            so_typedef( draw_command ) ;
            so_typedefs( so_std::vector< draw_command_t >, draw_commands ) ;
            draw_commands_t _dcs ;

            template< size_t c >
            class deduce_type
            {
            public:
                typedef uint_t type_t ;
            };
            template<>
            class deduce_type<2>
            {
            public:
                typedef ushort_t type_t ;
            };
            typedef this_t::deduce_type<sizeof( ImDrawIdx )> deduced_type_t ;

            struct vertex
            {
                so_math::vec2f_t pos ;
                so_math::vec2f_t uv ;
                so_math::vec4f_t color ;
            };
            so_typedef( vertex ) ;
            so_typedefs( so_gpu::vertex_buffer< vertex_t >, vb ) ;
            so_typedefs( so_gpu::index_buffer<deduced_type_t::type_t>, ib ) ;

            vb_ptr_t _vb ;
            ib_ptr_t _ib ;

            so_gpu::vertex_shader_ptr_t _vs ;
            so_gpu::pixel_shader_ptr_t _ps ;

            so_gpu::program_ptr_t _prog ;
            so_gpu::config_ptr_t _config ;

            so_typedefs( so_gpu::color_image_2d<so_math::vec4uc_t>, color_image ) ;

            struct render_data
            {
                so_gpu::variable_set_ptr_t vars = nullptr ;
                color_image_ptr_t image_ptr = nullptr ;
                so_gpu::texture_2d_ptr_t tx_ptr = nullptr ;
            };
            so_typedef( render_data ) ;
            so_typedefs( so_std::vector< render_data_t >, render_datas ) ;
            render_datas_t _render_datas ;

            so_math::mat4f_t _proj ;

        public:

            gl33_imgui_plug( imgui_t::shared_data_ptr_t,
                so_gpu::so_gl::igl_33_api_ptr_t ) ;
            gl33_imgui_plug( this_cref_t ) = delete ;
            gl33_imgui_plug( this_rref_t ) ;
            virtual ~gl33_imgui_plug( void_t ) ;

        public:

            static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
            static void_t destroy( this_ptr_t ) ;

        public:

            virtual so_gpx::plug_result on_load( void_t ) ;
            virtual so_gpx::plug_result on_unload( void_t ) ;
            virtual so_gpx::plug_result on_initialize( init_info_cref_t ) ;
            virtual so_gpx::plug_result on_release( void_t ) ;
            virtual so_gpx::plug_result on_execute( execute_info_cref_t ) ;
            virtual so_gpx::plug_result on_update( update_info_cref_t ) ;
            virtual void_t destroy( void_t ) ;
        } ;
        so_typedef( gl33_imgui_plug ) ;
    }
}