//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "../predef_post.h"
#include "gl33_singlepass_post_plug.h"

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
        class SNAKEOIL_GFX_API gl33_toscreen_post_plug :
            public gl33_singlepass_post_plug
        {
            typedef gl33_singlepass_post_plug this_base_t ;
            so_this_typedefs( gl33_toscreen_post_plug ) ;

        private:

            so_math::vec2f_t _bb_dims ;
            so_math::vec2f_t _fb_dims ;

            so_flow::variable<so_math::vec4f_t> * _var_bb_dims ;
            so_flow::variable<so_math::vec4f_t> * _var_fb_dims ;

        public:

            gl33_toscreen_post_plug( predef_post_t::shared_data_ptr_t,
                so_gpu::so_gl::igl_33_api_ptr_t ) ;
            gl33_toscreen_post_plug( this_cref_t ) = delete ;
            gl33_toscreen_post_plug( this_rref_t ) ;
            virtual ~gl33_toscreen_post_plug( void_t ) ;

        public:

            static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
            static void_t destroy( this_ptr_t ) ;

        public:

            virtual void_t destroy( void_t ) ;

        private:

            virtual void_t reconnect_variables( init_info_cref_t, so_gpu::variable_set_ptr_t ) ;
            virtual void_t before_execute( void_t ) ;
            virtual void_t after_execute( void_t ) ;

        } ;
        so_typedef( gl33_toscreen_post_plug ) ;
    }
}