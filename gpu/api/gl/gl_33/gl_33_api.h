//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_API_GL_GL_33_GL_33_API_H_
#define _SNAKEOIL_GPU_API_GL_GL_33_GL_33_API_H_

#include "../igl_33_api.h"

#include "../gl_any/gl_context.h"
#include "../gl_any/objects/variable.h"
#include "../gl_any/objects/feedback_buffer.h"

#include "../../../enums/primitive_types.h"
#include "../../../enums/shader_variable_types.h"

#include "../../../typedefs.h"

#include <snakeoil/std/container/stack.hpp>

namespace so_gpu
{
    namespace so_gl
    {
        class SNAKEOIL_GPU_API gl_33_api : public igl_33_api
        {
            typedef igl_33_api base_t ;
            so_this_typedefs( gl_33_api ) ;

            typedef so_std::stack< attribute_state_field, 10 > attrib_stack_t ;
            typedef so_std::stack< state_sets, 10 > state_sets_stack_t ;

        private:

            size_t _global_id = 0 ;
            gl_context * _context = nullptr ;

        private: // render states

            attrib_stack_t _attrib_stack ;
            state_sets_stack_t _state_sets ;

            state_sets _the_state ;

            GLuint _last_vao = GLuint( -1 ) ;
            GLuint _last_po = GLuint( -1 ) ;
            GLuint _last_framebuffer = GLuint( -1 ) ;
            so_gl::feedback_buffer * _last_feedback_buffer = nullptr ;
            bool_t _raster_discarded = false ;

            /// does not work out with glActiveTexture
            //GLuint _last_image = GLuint(-1) ;

        public:

            gl_33_api( void_t ) ;
            gl_33_api( this_cref_t ) = delete ;
            gl_33_api( this_rref_t rhv ) ;
            gl_33_api( gl_context_ptr_t ctx ) ;
            virtual ~gl_33_api( void_t ) ;

        public:  // static 

            static this_ptr_t create( this_rref_t, std::string const & purpose ) ;
            static void_t destroy( this_ptr_t ) ;

            virtual api_type get_type( void_t ) const ;
            virtual void_t destroy( void_t ) ;

        public: // initialization

            virtual so_gpu::result initialize( void_t ) ;

        public: // create/release

            virtual so_gpu::result create_shader( so_gpu::vertex_shader_ptr_t ptr ) ;
            virtual so_gpu::result release_shader( so_gpu::vertex_shader_ptr_t ptr ) ;

            virtual so_gpu::result create_shader( so_gpu::geometry_shader_ptr_t ptr ) ;
            virtual so_gpu::result release_shader( so_gpu::geometry_shader_ptr_t ptr ) ;

            virtual so_gpu::result create_shader( so_gpu::pixel_shader_ptr_t ptr ) ;
            virtual so_gpu::result release_shader( so_gpu::pixel_shader_ptr_t ptr ) ;

            virtual so_gpu::result create_program( so_gpu::program_ptr_t ptr ) ;
            virtual so_gpu::result release_program( so_gpu::program_ptr_t  ptr ) ;

            virtual so_gpu::result create_config( so_gpu::config_ptr_t ptr ) ;
            virtual so_gpu::result release_config( so_gpu::config_ptr_t ptr ) ;

            virtual so_gpu::result create_buffer( so_gpu::ivertex_buffer_ptr_t ptr ) ;
            virtual so_gpu::result release_buffer( so_gpu::ivertex_buffer_ptr_t ptr ) ;

            virtual so_gpu::result create_buffer( so_gpu::iindex_buffer_ptr_t ptr ) ;
            virtual so_gpu::result release_buffer( so_gpu::iindex_buffer_ptr_t ptr ) ;

            virtual so_gpu::result create_buffer( so_gpu::idata_buffer_ptr_t ptr ) ;
            virtual so_gpu::result release_buffer( so_gpu::idata_buffer_ptr_t  ptr ) ;

            virtual so_gpu::result create_buffer( so_gpu::feedback_buffer_ptr_t ptr ) ;
            virtual so_gpu::result release_buffer( so_gpu::feedback_buffer_ptr_t  ptr ) ;

            virtual so_gpu::result create_image( so_gpu::image_ptr_t ptr ) ;
            virtual so_gpu::result release_image( so_gpu::image_ptr_t ptr ) ;

            virtual so_gpu::result create_texture( so_gpu::texture_ptr_t ptr ) ;
            virtual so_gpu::result release_texture( so_gpu::texture_ptr_t ptr ) ;

            virtual so_gpu::result create_framebuffer( so_gpu::framebuffer_2d_ptr_t ptr ) ;
            virtual so_gpu::result release_framebuffer( so_gpu::framebuffer_2d_ptr_t  ptr ) ;

            virtual so_gpu::result validate( so_gpu::framebuffer_2d_ptr_t ptr ) ;

            virtual so_gpu::result create_variable( so_gpu::variable_set_ptr_t varset ) ;
            virtual so_gpu::result release_variable( so_gpu::variable_set_ptr_t varset ) ;

            virtual so_gpu::result create_variable( so_gpu::variable_ptr_t ) ;
            virtual so_gpu::result release_variable( so_gpu::variable_ptr_t ) ;

            virtual so_gpu::result validate_variable( so_gpu::variable_set_ptr_t ) ;

            virtual so_gpu::result create_sync( so_gpu::sync_ptr_t ptr )  ;
            virtual so_gpu::result release_sync( so_gpu::sync_ptr_t ptr )  ;

        public: // compile/link

            virtual so_gpu::result compile( so_gpu::vertex_shader_ptr_t ptr ) ;
            virtual so_gpu::result compile( so_gpu::geometry_shader_ptr_t ptr ) ;
            virtual so_gpu::result compile( so_gpu::pixel_shader_ptr_t ptr ) ;

            virtual so_gpu::result link( so_gpu::program_ptr_t ptr ) ;

        public: // load/save

            virtual so_gpu::result alloc_image_memory( so_gpu::iimage_2d_ptr_t ptr, image_alloc_info_cref_t ) ;
            virtual so_gpu::result save_image_memory( so_gpu::iimage_2d_ptr_t ptr, image_save_info_cref_t ) ;

            virtual so_gpu::result alloc_buffer_memory( so_gpu::idata_buffer_ptr_t ptr, memory_alloc_info_cref_t ) ;
            virtual so_gpu::result alloc_buffer_memory( so_gpu::ivertex_buffer_ptr_t ptr, memory_alloc_info_cref_t ) ;
            virtual so_gpu::result alloc_buffer_memory( so_gpu::iindex_buffer_ptr_t ptr, memory_alloc_info_cref_t ) ;

            virtual void_ptr_t map_buffer_memory( so_gpu::ivertex_buffer_ptr_t, memory_mapping_info_cref_t ) ;
            virtual so_gpu::result unmap_buffer_memory( so_gpu::ivertex_buffer_ptr_t ) ;

            virtual void_ptr_t map_buffer_memory( so_gpu::iindex_buffer_ptr_t, memory_mapping_info_cref_t ) ;
            virtual so_gpu::result unmap_buffer_memory( so_gpu::iindex_buffer_ptr_t ) ;

            virtual so_gpu::result load_buffer_memory( so_gpu::ivertex_buffer_ptr_t, memory_load_info_cref_t ) ;
            virtual so_gpu::result load_buffer_memory( so_gpu::iindex_buffer_ptr_t, memory_load_info_cref_t ) ;
            virtual so_gpu::result load_buffer_memory( so_gpu::idata_buffer_ptr_t, memory_load_info_cref_t ) ;

            virtual so_gpu::result load_image_memory( so_gpu::iimage_2d_ptr_t, image_load_info_cref_t ) ;

            virtual so_gpu::result load_variable( so_gpu::variable_set_ptr_t ptr ) ;

        public: // execution

            virtual so_gpu::result set_viewport( so_gpu::viewport_2d_cref_t viewport ) ;
            virtual so_gpu::result set_clear_color( so_math::vec4f_t const & color ) ;
            virtual so_gpu::result clear( bool_t color, bool_t depth, bool_t stencil ) ;

            virtual so_gpu::result use_framebuffer( so_gpu::framebuffer_2d_ptr_t ) ;
            virtual so_gpu::result use_framebuffer( so_gpu::framebuffer_2d_ptr_t, so_gpu::color_target ct ) ;

            virtual so_gpu::result use_feedback_buffer( so_gpu::feedback_buffer_ptr_t ) ;
            virtual so_gpu::result query_primitives_written( so_gpu::feedback_buffer_ptr_t, bool_t ) ;

            virtual so_gpu::result execute( render_config_info_cref_t ) ;
            virtual so_gpu::result execute( so_gpu::feedback_buffer_ptr_t, render_config_info_cref_t ) ;

            virtual so_gpu::result flush( void_t ) ;
            virtual so_gpu::result finish( void_t ) ;

            virtual so_gpu::result sync_fence( sync_ptr_t ) ;

        public: // state

            virtual so_gpu::result enable( render_state rs ) ;
            virtual so_gpu::result disable( render_state rs ) ;

            virtual so_gpu::result set_state( cull_mode cm ) ;
            virtual so_gpu::result set_state( front_face ff ) ;
            virtual so_gpu::result set_state( cull_mode cm, fill_mode fm ) ;

            virtual so_gpu::result set_state( blend_factor src, blend_factor dst ) ;
            virtual so_gpu::result set_state( blend_factor src_color, blend_factor dst_color, blend_factor src_alpha, blend_factor dst_alpha ) ;
            virtual so_gpu::result set_state( blend_function func ) ;
            virtual so_gpu::result set_state( blend_function func_color, blend_function func_alpha ) ;

            virtual so_gpu::result push_state( attribute_state_field const & ps ) ;
            virtual so_gpu::result pop_state( void_t ) ;

            virtual so_gpu::result change_states( attribute_state_field_cref_t, state_sets_cptr_t ) ;

            virtual float_t line_width( float_t const w ) ;
            virtual so_gpu::result set_scissor( size_t const, size_t const, size_t const, size_t const ) ;

        public: // other

        private:

            so_gpu::result prepare_config( GLuint id, config_ptr_t, size_t ) ;
            so_gpu::result use_vao( GLuint id, config_ptr_t ) ;
            so_gpu::result null_vao( void_t ) ;
            so_gpu::result use_program( GLuint id, program_ptr_t ) ;
            so_gpu::result use_texture( GLuint tu, GLuint sid, texture_ptr_t ) ;

            so_gpu::result check_buffer_size( bool_t check_index_buffer ) ;

            /// requires the pt from the draw command. the tf pt is deduced from that.
            so_gpu::result begin_transform_feedback( so_gpu::primitive_type pt, program_ptr_t ) ;
            so_gpu::result end_transform_feedback( void_t ) ;

            /// bind the texture image for the target.
            so_gpu::result use_image( GLuint id, GLenum target ) ;

            /// bind the targeted texture image to the passed texture unit
            so_gpu::result bind_image( GLuint id, GLuint tex_unit, GLenum target ) ;

            so_gpu::result bind_attributes( GLuint id, config_ptr_t ) ;
            so_gpu::result bind_uniforms( config_ptr_t, size_t ) ;
            so_gpu::result bind_textures( config_ptr_t, size_t ) ;
            so_gpu::result commit_uniforms( config_ptr_t, size_t ) ;
            so_gpu::result commit_textures( config_ptr_t, size_t, GLuint & ) ;

            so_gpu::result detatach_shaders( GLuint program_id ) ;
            so_gpu::result compile( GLuint shader_id, so_gpu::shader_ptr_t shd_ptr ) ;
            so_gpu::result prepare_vertex_shader( GLuint program_id, so_gpu::program_ptr_t program_ptr ) ;
            so_gpu::result prepare_geometry_shader( GLuint program_id, so_gpu::program_ptr_t program_ptr ) ;
            so_gpu::result prepare_pixel_shader( GLuint program_id, so_gpu::program_ptr_t program_ptr ) ;
            so_gpu::result pre_link_transform_feedback( GLuint program_id, so_gpu::program_ptr_t program_ptr ) ;
            so_gpu::result post_link_attributes( GLuint program_id, so_gpu::program_ptr_t program_ptr ) ;
            so_gpu::result post_link_uniforms( GLuint program_id, so_gpu::program_ptr_t program_ptr ) ;

            shader_variable_ptr_t create_shader_variable( GLuint location, GLenum type, std::string const & name ) ;
            void_t delete_all_shader_variables( so_gpu::program_ptr_t prog_ptr ) ;
            void_t delete_shader_variable( shader_variable_ptr_t var_ptr ) ;

            so_gpu::result check_link( GLuint program_id ) ;

            void_ptr_t alloc_variable_memory( shader_variable_type ) ;
            void_t free_variable_memory( void_ptr_t ) ;

            so_gpu::so_gl::variable::copy_funk_t make_copy_funk( shader_variable_type ) ;
            so_gpu::so_gl::variable::uniform_funk_t make_uniform_funk( shader_variable_type ) ;

            so_gpu::result use_framebuffer( GLuint ) ;
        };
        so_typedef( gl_33_api ) ;
    }
    
}

#endif

