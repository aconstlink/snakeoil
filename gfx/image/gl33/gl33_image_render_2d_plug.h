//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "../../api.h"
#include "../../result.h"
#include "../../protos.h"
#include "../../typedefs.h"

#include "../image_render_2d_shared_data.h"

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
        class SNAKEOIL_GFX_API gl33_image_render_2d_plug : public so_gpx::plug<so_gpu::so_gl::igl_33_api >
        {
            typedef so_gpx::plug<so_gpu::so_gl::igl_33_api> this_base_t ;
            so_this_typedefs( gl33_image_render_2d_plug ) ;

        private:

            so_gfx::image_render_2d_shared_data_ptr_t _sd = nullptr ;

            size_t _num_quads = 10000 ;

        private:

            struct image_data
            {
                so_imex::image_cptr_t _img_ptr = nullptr ;
                so_gpu::iimage_2d_ptr_t _gpu_img_ptr = nullptr ;
                so_gpu::texture_2d_ptr_t _tx = nullptr ;

                uint32_ptr_t start_offset = nullptr ;

                /// used for rendering
                size_t varset_id ;
                /// used for configuration and deletion
                so_gpu::variable_set_ptr_t var_set ;
            };
            so_typedef( image_data ) ;
            so_typedefs( so_std::vector< image_data_t >, image_datas ) ;

            image_datas_t _image_datas ;

            struct draw_call_data
            {
                size_t group_id ;
                /// where to start in the image info buffer
                /// needs to be set right before rendering
                uint32_t offset ;
                /// the number of elements 
                uint32_t num_elements ;

                /// index into the image data array
                size_t image_data_id ;
            };
            so_typedef( draw_call_data ) ;

            so_typedefs( so_std::vector< draw_call_data_t >, draw_calls ) ;
            draw_calls_t _draw_calls ;

            
        private:

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

            data_buffer_ptr_t _image_info_ptr = nullptr ;

            so_math::mat4f_t _view ;
            so_math::mat4f_t _proj ;

        public:

            gl33_image_render_2d_plug( so_gfx::image_render_2d_shared_data_ptr_t,
                so_gpu::so_gl::igl_33_api_ptr_t ) ;
            gl33_image_render_2d_plug( this_cref_t ) = delete ;
            gl33_image_render_2d_plug( this_rref_t ) ;
            virtual ~gl33_image_render_2d_plug( void_t ) ;

        public:

            static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
            static void_t destroy( this_ptr_t ) ;

        public:

            virtual so_gpx::plug_result on_load( void_t ) ;
            virtual so_gpx::plug_result on_unload( void_t ) ;

        public:

            virtual so_gpx::plug_result on_initialize( init_info_cref_t ) ;
            virtual so_gpx::plug_result on_release( void_t ) ;
            virtual so_gpx::plug_result on_execute( execute_info_cref_t ) ;

        public:

            virtual so_gpx::plug_result on_update( update_info_cref_t ) ;

        public:

            virtual void_t destroy( void_t ) ;
        };
        so_typedef( gl33_image_render_2d_plug ) ;
    }
}