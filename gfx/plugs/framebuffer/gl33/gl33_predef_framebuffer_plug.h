//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "../predef_framebuffer.h"

#include <snakeoil/gpx/system/render_system.h>
#include <snakeoil/std/string/string.hpp>

#include <snakeoil/gpx/plug/plug.h>

#include <snakeoil/gpu/protos.h>
#include <snakeoil/gpu/image/color_image_2d.hpp>
#include <snakeoil/gpu/image/depth_image_2d.hpp>
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
        class SNAKEOIL_GFX_API gl33_predef_framebuffer_plug :
            public so_gpx::plug<so_gpu::so_gl::igl_33_api >
        {
            typedef so_gpx::plug<so_gpu::so_gl::igl_33_api > this_base_t ;
            so_this_typedefs( gl33_predef_framebuffer_plug ) ;

        private:

            so_gpu::framebuffer_2d_ptr_t _fb_ptr = nullptr ;

            so_typedefs( so_gpu::color_image_2d<so_math::vec4uc_t>, color_image ) ;
            color_image_ptr_t _image_ptr = nullptr ;

            so_typedefs( so_gpu::depth_image_2d<float_t>, depth_image ) ;
            depth_image_ptr_t _depth_ptr = nullptr ;

            so_gpu::texture_2d_ptr_t _tx_ptr = nullptr ;
            so_gpu::depth_texture_2d_ptr_t _dtx_ptr = nullptr ;

            so_flow::variable< so_gpu::viewport_2d_t > * _vp_window ;
            so_flow::variable< so_gpu::viewport_2d_t > * _vp_back ;

            so_gfx::predef_framebuffer_type _pfb_type ;
            predef_framebuffer_t::shared_data_ptr_t _sd ;

            so_math::vec4f_t _color ;
            so_gpu::viewport_2d_t _viewport ;

        public:

            gl33_predef_framebuffer_plug( predef_framebuffer_t::shared_data_ptr_t,
                so_gpu::so_gl::igl_33_api_ptr_t ) ;
            gl33_predef_framebuffer_plug( this_cref_t ) = delete ;
            gl33_predef_framebuffer_plug( this_rref_t ) ;
            virtual ~gl33_predef_framebuffer_plug( void_t ) ;

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
        so_typedef( gl33_predef_framebuffer_plug ) ;
    }
}