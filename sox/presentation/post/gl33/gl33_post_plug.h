//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "../post.h"
#include "../post_shared_data.h"

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
    namespace gl_33
    {
        class  gl33_post_plug :
            public so_gpx::plug<so_gpu::so_gl::igl_33_api >
        {
            typedef so_gpx::plug<so_gpu::so_gl::igl_33_api > this_base_t ;
            so_this_typedefs( gl33_post_plug ) ;

        private:

            struct vertex
            {
                so_math::vec3f_t pos ; 
            };
            so_typedef( vertex ) ;
            so_typedefs( so_gpu::vertex_buffer< vertex_t >, vb ) ;
            so_typedefs( so_gpu::index_buffer<uint_t>, ib ) ;

            so_std::string_t _vss ;
            so_std::string_t _pss ;

            vb_ptr_t _vb ;
            ib_ptr_t _ib ;

            so_gpu::vertex_shader_ptr_t _vs ;
            so_gpu::pixel_shader_ptr_t _ps ;

            so_gpu::program_ptr_t _prog ;
            so_gpu::config_ptr_t _config ;

            struct bar_data
            {
                so_math::vec4f_t pos_wh;
                so_gpu::variable_set_ptr_t vs ;
            };
            so_typedef( bar_data ) ;

            sox_presentation::post_shared_data_ptr_t _sd ;
           
            so_math::vec2f_t _dims ;

        private:

        public:

            gl33_post_plug( sox_presentation::post_shared_data_ptr_t,
                so_gpu::so_gl::igl_33_api_ptr_t ) ;
            gl33_post_plug( this_cref_t ) = delete ;
            gl33_post_plug( this_rref_t ) ;
            virtual ~gl33_post_plug( void_t ) ;

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
        so_typedef( gl33_post_plug ) ;
    }
}