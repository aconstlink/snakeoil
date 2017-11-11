//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "../../api.h"
#include "../../result.h"
#include "../../protos.h"
#include "../../typedefs.h"

#include "../rect_render_2d_shared_data.h"

#include <snakeoil/gpx/plug/plug.h>

#include <snakeoil/gpu/protos.h>
#include <snakeoil/gpu/image/color_image_2d.hpp>
#include <snakeoil/gpu/buffer/vertex_buffer.hpp>
#include <snakeoil/gpu/buffer/index_buffer.hpp>
#include <snakeoil/gpu/buffer/data_buffer.hpp>

#include <snakeoil/math/vector/vector4.hpp>

namespace so_gfx
{
    namespace so_gl33
    {
        class SNAKEOIL_GFX_API gl33_rect_render_2d_plug : public so_gpx::plug<so_gpu::so_gl::igl_33_api >
        {
            typedef so_gpx::plug<so_gpu::so_gl::igl_33_api > this_base_t ;
            so_this_typedefs( gl33_rect_render_2d_plug ) ;

        private:

            so_gfx::rect_render_2d_shared_data_ptr_t _sd = nullptr ;

        private: // group data stuff

            struct per_group_data
            {
                size_t group_id ;
                size_t varset_id ;

                uint32_t num_elements ;

                uint32_ptr_t start_offset ;
                so_math::mat4f_ptr_t proj ;
                so_math::mat4f_ptr_t view ;
            };
            so_typedef( per_group_data ) ;

            so_typedefs( so_std::vector< per_group_data >, per_group_datas ) ;

            per_group_datas_t _per_group_datas ;

        private: // rendering stuff

            struct vertex
            {
                so_math::vec3f_t pos ;
            };
            so_typedef( vertex ) ;
            so_typedefs( so_gpu::vertex_buffer< vertex_t >, vb ) ;
            so_typedefs( so_gpu::index_buffer<uint_t>, ib ) ;

            so_typedefs( so_gpu::data_buffer< so_math::vec4f_t >, data_buffer ) ;

            vb_ptr_t _vb ;
            ib_ptr_t _ib ;

            so_gpu::vertex_shader_ptr_t _vs ;
            so_gpu::pixel_shader_ptr_t _ps ;

            so_gpu::program_ptr_t _prog ;
            so_gpu::config_ptr_t _config ;
            so_gpu::framebuffer_2d_ptr_t _fb ;

            data_buffer_ptr_t _rect_info_ptr = nullptr ;

            /// the number of lines in the vertex buffer
            size_t _num_rects = 10000 ;

            so_typedefs( so_std::vector<so_gpu::variable_set_ptr_t>, var_sets ) ;

            /// per render call variable set
            /// generally, one var set per group but
            /// if a group requires rendering of too many 
            /// lines, a group could have 
            var_sets_t _var_sets ;

        public:

            gl33_rect_render_2d_plug( so_gfx::rect_render_2d_shared_data_ptr_t,
                so_gpu::so_gl::igl_33_api_ptr_t ) ;
            gl33_rect_render_2d_plug( this_cref_t ) = delete ;
            gl33_rect_render_2d_plug( this_rref_t ) ;
            virtual ~gl33_rect_render_2d_plug( void_t ) ;

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

        };
        so_typedef( gl33_rect_render_2d_plug ) ;
    }
}