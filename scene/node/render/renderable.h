//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_SCENE_NODE_RENDER_RENDERABLE_H_
#define _SNAKEOIL_SCENE_NODE_RENDER_RENDERABLE_H_

#include "../leaf.h"

#include <snakeoil/flow/protos.h>
#include <snakeoil/gpu/protos.h>
#include <snakeoil/math/matrix/matrix4.hpp>
#include <snakeoil/math/vector/vector4.hpp>

#include <snakeoil/flow/variable/variable.hpp>

#include <snakeoil/std/container/vector.hpp>

namespace so_scene
{
    namespace so_node
    {
        class SNAKEOIL_SCENE_API renderable : public leaf
        {
            typedef leaf base_t ;
            so_this_typedefs( renderable ) ;

            so_typedefs( so_std::vector< so_gpu::variable_set_ptr_t>, gpu_varsets ) ;
            
            so_typedefs( so_flow::variable< so_math::mat4f_t >, var_mat4f ) ;
            so_typedefs( so_flow::variable< so_math::vec4f_t >, var_vec4f ) ;

        private:

            gpu_varsets_t _gpu_varsets ;
            so_gpu::config_ptr_t _config_ptr = nullptr ;
            
            var_mat4f_ptr_t _object_ptr = nullptr ;
            var_mat4f_ptr_t _world_ptr = nullptr ;
            var_mat4f_ptr_t _view_ptr = nullptr ;
            var_mat4f_ptr_t _proj_ptr = nullptr ;
            var_mat4f_ptr_t _mvp_ptr = nullptr ;

            /// projection parameters
            /// ( near, far, -, - )
            var_vec4f_ptr_t _proj0_ptr = nullptr ;
            
        private:

            // non-copyable
            renderable( this_cref_t ) {}

        public:

            renderable( void_t ) ;
            renderable( this_rref_t ) ;
            renderable( iparent_ptr_t ) ;
            renderable( so_gpu::config_ptr_t ) ;
            renderable( so_gpu::config_ptr_t, iparent_ptr_t ) ;

            virtual ~renderable( void_t ) ;

        public:

            static this_ptr_t create( so_memory::purpose_cref_t ) ;
            static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
            static void_t destroy( this_ptr_t ) ;

        public:

            so_gpu::config_ptr_t get_config( void_t ) ;
            so_gpu::config_cptr_t get_config( void_t ) const ;

            size_t get_num_gpu_varsets( void_t ) const ;
            bool_t renderable::get_varset( size_t const, so_gpu::variable_set_ptr_t & ) ;
                        
            so_gpu::config_ptr_t set_config( so_gpu::config_ptr_t cptr ) ;
            so_scene::result add_variable_set( so_gpu::variable_set_ptr_t ) ;
            

            bool_t set_object_matrix( so_math::mat4f_cref_t ) ;
            bool_t set_world_matrix( so_math::mat4f_cref_t ) ;
            bool_t set_view_matrix( so_math::mat4f_cref_t ) ;
            bool_t set_proj_matrix( so_math::mat4f_cref_t ) ;
            bool_t set_mvp_matrix( so_math::mat4f_cref_t ) ;
            bool_t set_proj_param0( so_math::vec4f_cref_t ) ;

            bool_t bind_object_matrix( so_std::string_cref_t, this_t::var_mat4f_ptr_t ) ;
            bool_t bind_world_matrix( so_std::string_cref_t, this_t::var_mat4f_ptr_t ) ;
            bool_t bind_view_matrix( so_std::string_cref_t, this_t::var_mat4f_ptr_t ) ;
            bool_t bind_proj_matrix( so_std::string_cref_t, this_t::var_mat4f_ptr_t ) ;
            bool_t bind_mvp_matrix( so_std::string_cref_t, this_t::var_mat4f_ptr_t ) ;
            bool_t bind_proj_param0( so_std::string_cref_t, this_t::var_vec4f_ptr_t ) ;

            bool_t get_object_matrix( so_math::mat4f_out_t ) const ;
            bool_t get_world_matrix( so_math::mat4f_out_t ) const ;
            bool_t get_view_matrix( so_math::mat4f_out_t ) const ;
            bool_t get_proj_matrix( so_math::mat4f_out_t ) const ;
            bool_t get_mvp_matrix( so_math::mat4f_out_t ) const ;
            bool_t get_proj_param0( so_math::vec4f_out_t ) const ;

            /// computes the mvp matrix from set projection, 
            /// view, world and object matrix
            bool_t compute_mvp_matrix( void_t ) ;

            bool_t reconnect_flow_to_variable_set( void_t ) ;

        public:

            virtual so_scene::result apply( so_scene::so_visitor::ivisitor_ptr_t ) ;
            virtual void_t destroy( void_t ) ;
        };
        so_typedef( renderable ) ;
    }
}

#endif
