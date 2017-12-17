//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_API_GL_IGL_33_API_H_
#define _SNAKEOIL_API_GL_IGL_33_API_H_

#include "igl_api.h"
#include "../info_structs.h"

#include "../../state_set.h"
#include "../../viewport/viewport_2d.h"

#include "../../enums/render_states.h"
#include "../../enums/clear_modes.h"
#include "../../enums/attribute_state.h"
#include "../../enums/color_targets.h"
#include "../../enums/cull_modes.h"
#include "../../enums/front_faces.h"
#include "../../enums/blend_factors.h"
#include "../../enums/blend_functions.h"
#include "../../enums/fill_modes.h"

namespace so_gpu
{
    namespace so_gl
    {
        class SNAKEOIL_GPU_API igl_33_api : public igl_api
        {
        public:

            virtual so_gpu::result initialize( void_t ) = 0 ;

        public:

            virtual so_gpu::result create_shader( so_gpu::vertex_shader_ptr_t ptr ) = 0 ;
            virtual so_gpu::result release_shader( so_gpu::vertex_shader_ptr_t ptr ) = 0 ;

            virtual so_gpu::result create_shader( so_gpu::geometry_shader_ptr_t ptr ) = 0 ;
            virtual so_gpu::result release_shader( so_gpu::geometry_shader_ptr_t ptr ) = 0 ;

            virtual so_gpu::result create_shader( so_gpu::pixel_shader_ptr_t ptr ) = 0 ;
            virtual so_gpu::result release_shader( so_gpu::pixel_shader_ptr_t ptr ) = 0 ;

            virtual so_gpu::result create_program( so_gpu::program_ptr_t ptr ) = 0 ;
            virtual so_gpu::result release_program( so_gpu::program_ptr_t  ptr ) = 0 ;

            virtual so_gpu::result create_config( so_gpu::config_ptr_t ptr ) = 0 ;
            virtual so_gpu::result release_config( so_gpu::config_ptr_t ptr ) = 0 ;

            virtual so_gpu::result create_buffer( so_gpu::ivertex_buffer_ptr_t ptr ) = 0 ;
            virtual so_gpu::result release_buffer( so_gpu::ivertex_buffer_ptr_t ptr ) = 0 ;

            virtual so_gpu::result create_buffer( so_gpu::iindex_buffer_ptr_t ptr ) = 0 ;
            virtual so_gpu::result release_buffer( so_gpu::iindex_buffer_ptr_t ptr ) = 0 ;

            virtual so_gpu::result create_buffer( so_gpu::idata_buffer_ptr_t ptr ) = 0 ;
            virtual so_gpu::result release_buffer( so_gpu::idata_buffer_ptr_t  ptr ) = 0 ;

            virtual so_gpu::result create_buffer( so_gpu::feedback_buffer_ptr_t ptr ) = 0 ;
            virtual so_gpu::result release_buffer( so_gpu::feedback_buffer_ptr_t  ptr ) = 0 ;

            virtual so_gpu::result create_image( so_gpu::image_ptr_t ptr ) = 0 ;
            virtual so_gpu::result release_image( so_gpu::image_ptr_t ptr ) = 0 ;

            virtual so_gpu::result create_texture( so_gpu::texture_ptr_t ptr ) = 0 ;
            virtual so_gpu::result release_texture( so_gpu::texture_ptr_t ptr ) = 0 ;

            virtual so_gpu::result create_framebuffer( so_gpu::framebuffer_2d_ptr_t ptr ) = 0 ;
            virtual so_gpu::result release_framebuffer( so_gpu::framebuffer_2d_ptr_t  ptr ) = 0 ;

            virtual so_gpu::result validate( so_gpu::framebuffer_2d_ptr_t ptr ) = 0 ;

            virtual so_gpu::result create_variable( so_gpu::variable_set_ptr_t varset ) = 0 ;
            virtual so_gpu::result release_variable( so_gpu::variable_set_ptr_t varset ) = 0 ;

            virtual so_gpu::result create_variable( so_gpu::variable_ptr_t ) = 0 ;
            virtual so_gpu::result release_variable( so_gpu::variable_ptr_t ) = 0 ;

            virtual so_gpu::result validate_variable( so_gpu::variable_set_ptr_t ) = 0 ;

            virtual so_gpu::result create_sync( so_gpu::sync_ptr_t ptr ) = 0 ;
            virtual so_gpu::result release_sync( so_gpu::sync_ptr_t ptr ) = 0 ;

        public: // compile/link

            virtual so_gpu::result compile( so_gpu::vertex_shader_ptr_t ptr ) = 0 ;
            virtual so_gpu::result compile( so_gpu::geometry_shader_ptr_t ptr ) = 0 ;
            virtual so_gpu::result compile( so_gpu::pixel_shader_ptr_t ptr ) = 0 ;
            virtual so_gpu::result link( so_gpu::program_ptr_t ptr ) = 0 ;

        public: // load/save

            virtual so_gpu::result alloc_image_memory( so_gpu::iimage_2d_ptr_t ptr, image_alloc_info_cref_t ) = 0 ;
            virtual so_gpu::result save_image_memory( so_gpu::iimage_2d_ptr_t ptr, image_save_info_cref_t ) = 0 ;

            virtual so_gpu::result alloc_buffer_memory( so_gpu::idata_buffer_ptr_t ptr, memory_alloc_info_cref_t ) = 0 ;
            virtual so_gpu::result alloc_buffer_memory( so_gpu::ivertex_buffer_ptr_t ptr, memory_alloc_info_cref_t ) = 0 ;
            virtual so_gpu::result alloc_buffer_memory( so_gpu::iindex_buffer_ptr_t ptr, memory_alloc_info_cref_t ) = 0 ;

            virtual void_ptr_t map_buffer_memory( so_gpu::ivertex_buffer_ptr_t, memory_mapping_info_cref_t ) = 0 ;
            virtual so_gpu::result unmap_buffer_memory( so_gpu::ivertex_buffer_ptr_t ) = 0 ;

            virtual void_ptr_t map_buffer_memory( so_gpu::iindex_buffer_ptr_t, memory_mapping_info_cref_t ) = 0 ;
            virtual so_gpu::result unmap_buffer_memory( so_gpu::iindex_buffer_ptr_t ) = 0 ;

            virtual so_gpu::result load_buffer_memory( so_gpu::ivertex_buffer_ptr_t, memory_load_info_cref_t ) = 0 ;
            virtual so_gpu::result load_buffer_memory( so_gpu::iindex_buffer_ptr_t, memory_load_info_cref_t ) = 0 ;
            virtual so_gpu::result load_buffer_memory( so_gpu::idata_buffer_ptr_t, memory_load_info_cref_t ) = 0 ;

            virtual so_gpu::result load_image_memory( so_gpu::iimage_2d_ptr_t, image_load_info_cref_t ) = 0 ;

            virtual so_gpu::result load_variable( so_gpu::variable_set_ptr_t ptr ) = 0 ;

        public: // execution

            virtual so_gpu::result set_viewport( so_gpu::viewport_2d_cref_t viewport ) = 0 ;
            virtual so_gpu::result set_clear_color( so_math::vec4f_t const & color ) = 0 ;
            virtual so_gpu::result clear( bool_t color, bool_t depth, bool_t stencil ) = 0 ;

            virtual so_gpu::result use_framebuffer( so_gpu::framebuffer_2d_ptr_t ) = 0 ;
            virtual so_gpu::result use_framebuffer( so_gpu::framebuffer_2d_ptr_t, so_gpu::color_target ct ) = 0 ;

            virtual so_gpu::result use_feedback_buffer( so_gpu::feedback_buffer_ptr_t ) = 0 ;
            virtual so_gpu::result query_primitives_written( so_gpu::feedback_buffer_ptr_t, bool_t wait_for_result ) = 0 ;

            virtual so_gpu::result execute( render_config_info_cref_t ) = 0 ;
            virtual so_gpu::result execute( so_gpu::feedback_buffer_ptr_t, render_config_info_cref_t ) = 0 ;

            virtual so_gpu::result flush( void_t ) = 0 ;
            virtual so_gpu::result finish( void_t ) = 0 ;

            virtual so_gpu::result sync_fence( sync_ptr_t ) = 0 ;

        public: // state

            virtual so_gpu::result enable( render_state rs ) = 0 ;
            virtual so_gpu::result disable( render_state rs ) = 0 ;

            virtual so_gpu::result set_state( cull_mode cm ) = 0 ;
            virtual so_gpu::result set_state( front_face ff ) = 0 ;
            virtual so_gpu::result set_state( cull_mode cm, fill_mode fm ) = 0 ;

            virtual so_gpu::result set_state( blend_factor src, blend_factor dst ) = 0 ;
            virtual so_gpu::result set_state( blend_factor src_color, blend_factor dst_color, blend_factor src_alpha, blend_factor dst_alpha ) = 0 ;
            virtual so_gpu::result set_state( blend_function func ) = 0 ;
            virtual so_gpu::result set_state( blend_function func_color, blend_function func_alpha ) = 0 ;

            virtual so_gpu::result push_state( attribute_state_field_cref_t ps ) = 0 ;
            virtual so_gpu::result pop_state( void_t ) = 0 ;

            virtual so_gpu::result change_states( attribute_state_field_cref_t, state_sets_cptr_t ) = 0 ;

            virtual float_t line_width( float_t const w ) = 0 ;
        };
    }
}

#endif