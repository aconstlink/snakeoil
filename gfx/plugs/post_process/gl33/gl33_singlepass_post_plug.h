//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "../predef_post.h"

#include <snakeoil/gpx/plug/plug.h>

#include <snakeoil/gpu/protos.h>
#include <snakeoil/gpu/image/color_image_2d.hpp>
#include <snakeoil/gpu/buffer/vertex_buffer.hpp>
#include <snakeoil/gpu/buffer/index_buffer.hpp>
#include <snakeoil/gpu/buffer/data_buffer.hpp>
#include <snakeoil/gpu/viewport/viewport_2d.h>

#include <snakeoil/flow/protos.h>
#include <snakeoil/flow/variable/variable.hpp>

#include <snakeoil/math/vector/vector4.hpp>

namespace so_gfx
{
    namespace so_gl33
    {
        class SNAKEOIL_GFX_API gl33_singlepass_post_plug :
            public so_gpx::plug<so_gpu::so_gl::igl_33_api >
        {
            typedef so_gpx::plug<so_gpu::so_gl::igl_33_api > this_base_t ;
            so_this_typedefs( gl33_singlepass_post_plug ) ;

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
            so_gpu::variable_set_ptr_t _vars_0 ;

        protected:

            so_gfx::predef_post_t::shared_data_ptr_t _sd_ptr = nullptr ;       

            so_std::string_t _vs_file_name ;
            so_std::string_t _ps_file_name ;

        public:

            gl33_singlepass_post_plug( predef_post_t::shared_data_ptr_t,
                so_gpu::so_gl::igl_33_api_ptr_t ) ;
            gl33_singlepass_post_plug( this_cref_t ) = delete ;
            gl33_singlepass_post_plug( this_rref_t ) ;
            virtual ~gl33_singlepass_post_plug( void_t ) ;

        public:

            virtual so_gpx::plug_result on_load( load_info_cref_t ) final ;
            virtual so_gpx::plug_result on_unload( void_t ) final;
            virtual so_gpx::plug_result on_initialize( init_info_cref_t ) final;
            virtual so_gpx::plug_result on_release( void_t ) final;
            virtual so_gpx::plug_result on_execute( execute_info_cref_t ) final;
            virtual so_gpx::plug_result on_update( update_info_cref_t ) final;
            
            virtual void_t destroy( void_t ) = 0 ;

        protected:

            virtual void_t reconnect_variables( init_info_cref_t, so_gpu::variable_set_ptr_t ) = 0 ;

            virtual void_t before_execute( void_t ) = 0 ;
            virtual void_t after_execute( void_t ) = 0 ;
        } ;
        so_typedef( gl33_singlepass_post_plug ) ;
    }
}