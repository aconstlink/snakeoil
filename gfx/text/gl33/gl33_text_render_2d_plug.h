//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GFX_TEXT_GL33_TEXT_RENDER_2D_PLUG_H_
#define _SNAKEOIL_GFX_TEXT_GL33_TEXT_RENDER_2D_PLUG_H_

#include "../../api.h"
#include "../../result.h"
#include "../../protos.h"
#include "../../typedefs.h"

#include "../text_render_2d_shared_data.h"

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
        class SNAKEOIL_GFX_API gl33_text_render_2d_plug : public so_gpx::plug<so_gpu::so_gl::igl_33_api >
        {
            typedef so_gpx::plug<so_gpu::so_gl::igl_33_api> this_base_t ;
            so_this_typedefs( gl33_text_render_2d_plug ) ;

        private:

            so_gfx::text_render_2d_shared_data_ptr_t _sd = nullptr ;

        private: 

            so_typedefs( so_gpu::color_image_2d< so_math::vec4uc_t >, image ) ;

            image_ptr_t _gpu_img_ptr = nullptr ;
            so_gpu::texture_2d_ptr_t _tx = nullptr ;

            size_t _num_quads = 10000 ;

        private: 

            struct vertex
            {
                so_math::vec3f_t pos ;
            };
            so_typedef( vertex ) ;
            so_typedefs( so_gpu::vertex_buffer< vertex_t >, vb ) ;
            so_typedefs( so_gpu::index_buffer<uint_t>, ib ) ;

            so_typedefs( so_gpu::data_buffer< so_math::vec4f_t >, data_buffer ) ;

            vb_ptr_t _vb_text ;
            ib_ptr_t _ib_text ;

            so_gpu::vertex_shader_ptr_t _vs_text ;
            so_gpu::pixel_shader_ptr_t _ps_text ;

            so_gpu::program_ptr_t _prog_text ;
            so_gpu::config_ptr_t _config_text ;
            so_gpu::variable_set_ptr_t _vars_text ;
            so_gpu::framebuffer_2d_ptr_t _fb_ptr ;

            data_buffer_ptr_t _glyph_info_ptr = nullptr ;
            data_buffer_ptr_t _text_info_ptr = nullptr ;

        public:

            gl33_text_render_2d_plug( so_gfx::text_render_2d_shared_data_ptr_t, 
                so_gpu::so_gl::igl_33_api_ptr_t ) ;
            gl33_text_render_2d_plug( this_cref_t ) = delete ;
            gl33_text_render_2d_plug( this_rref_t ) ;
            virtual ~gl33_text_render_2d_plug( void_t ) ;

        public:

            static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
            static void_t destroy( this_ptr_t ) ;

        public:

            virtual so_gpx::plug_result on_load( void_t ) ;
            virtual so_gpx::plug_result on_unload( void_t ) ;

        public:

            virtual so_gpx::plug_result on_initialize( init_info_cref_t ) ;
            virtual so_gpx::plug_result on_release( void_t ) ;
            virtual so_gpx::plug_result on_transfer( void_t ) ;
            virtual so_gpx::plug_result on_execute( execute_info_cref_t ) ;

        public:

            virtual so_gpx::plug_result on_update( void_t ) ;

        public:

            virtual void_t destroy( void_t ) ;
        };
        so_typedef( gl33_text_render_2d_plug ) ;
    }
}

#endif