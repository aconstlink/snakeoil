//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "../framebuffer_begin.h"

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
        class SNAKEOIL_GFX_API gl33_framebuffer_begin_plug : 
            public so_gpx::plug<so_gpu::so_gl::igl_33_api >
        {
            typedef so_gpx::plug<so_gpu::so_gl::igl_33_api > this_base_t ;
            so_this_typedefs( gl33_framebuffer_begin_plug ) ;


        public:

            gl33_framebuffer_begin_plug( framebuffer_begin_t::shared_data_ptr_t,
                so_gpu::so_gl::igl_33_api_ptr_t ) ;
            gl33_framebuffer_begin_plug( this_cref_t ) = delete ;
            gl33_framebuffer_begin_plug( this_rref_t ) ;
            virtual ~gl33_framebuffer_begin_plug( void_t ) ;

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
        so_typedef( gl33_framebuffer_begin_plug ) ;
    }
}