//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "../sdf_a.h"
#include "../sdf_a_shared_data.h"

#include "sox_config.h"

#include <snakeoil/gpx/plug/plug.h>
#include <snakeoil/gpx/system/render_system.h>
#include <snakeoil/gpu/protos.h>
#include <snakeoil/gpu/image/color_image_2d.hpp>
#include <snakeoil/gpu/buffer/vertex_buffer.hpp>
#include <snakeoil/gpu/buffer/index_buffer.hpp>
#include <snakeoil/gpu/buffer/data_buffer.hpp>
#include <snakeoil/gpu/viewport/viewport_2d.h>

#include <snakeoil/std/string/string.hpp>
#include <snakeoil/math/vector/vector4.hpp>

namespace sox_presentation
{
    namespace transitions
    {
        namespace gl_33
        {
            class  gl33_sdf_a_plug :
                public so_gpx::plug<so_gpu::so_gl::igl_33_api >
            {
                typedef so_gpx::plug<so_gpu::so_gl::igl_33_api > this_base_t ;
                so_this_typedefs( gl33_sdf_a_plug ) ;

            private:

                struct vertex
                {
                    so_math::vec3f_t pos ;
                };
                so_typedef( vertex ) ;
                so_typedefs( so_gpu::vertex_buffer< vertex_t >, vb ) ;
                so_typedefs( so_gpu::index_buffer<uint_t>, ib ) ;

                so_std::string_t _vss_mix ;
                so_std::string_t _pss_mix ;

                so_std::string_t _vss_blit ;
                so_std::string_t _pss_blit ;

                vb_ptr_t _vb ;
                ib_ptr_t _ib ;

                so_gpu::vertex_shader_ptr_t _vs_color ;
                so_gpu::pixel_shader_ptr_t _ps_color ;

                so_gpu::vertex_shader_ptr_t _vs_mask ;
                so_gpu::pixel_shader_ptr_t _ps_mask ;

                so_gpu::program_ptr_t _prog_color ;
                so_gpu::config_ptr_t _config_color ;

                so_gpu::program_ptr_t _prog_mask ;
                so_gpu::config_ptr_t _config_mask ;

                so_gpu::variable_set_ptr_t _vars_mask ;
                so_gpu::variable_set_ptr_t _vars_color ;

                sox_presentation::transitions::sdf_a_shared_data_ptr_t _sd ;

                so_math::vec2f_t _dims ;

            private:

            public:

                gl33_sdf_a_plug( sox_presentation::transitions::sdf_a_shared_data_ptr_t,
                    so_gpu::so_gl::igl_33_api_ptr_t ) ;
                gl33_sdf_a_plug( this_cref_t ) = delete ;
                gl33_sdf_a_plug( this_rref_t ) ;
                virtual ~gl33_sdf_a_plug( void_t ) ;

            public:

                static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
                static void_t destroy( this_ptr_t ) ;

            public:

                virtual so_gpx::plug_result on_load( load_info_cref_t ) ;
                virtual so_gpx::plug_result on_unload( void_t ) ;
                virtual so_gpx::plug_result on_initialize( init_info_cref_t ) ;
                virtual so_gpx::plug_result on_release( void_t ) ;
                virtual so_gpx::plug_result on_execute( execute_info_cref_t ) ;
                virtual so_gpx::plug_result on_update( update_info_cref_t ) ;
                virtual void_t destroy( void_t ) ;
            } ;
            so_typedef( gl33_sdf_a_plug ) ;
        }
    }
}