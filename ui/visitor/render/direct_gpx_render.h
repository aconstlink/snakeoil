//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_SCENE_VISITOR_DIRECT_GPX_RENDER_H_
#define _SNAKEOIL_SCENE_VISITOR_DIRECT_GPX_RENDER_H_

#include "../visitor.h"

#include <snakeoil/gpx/protos.h>
#include <snakeoil/gfx/camera/ilens.h>

#include <snakeoil/math/utility/3d/transformation.hpp>
#include <snakeoil/std/container/stack.hpp>

namespace so_scene
{
    namespace so_visitor
    {
        class SNAKEOIL_SCENE_API direct_gpx_render : public visitor
        {
            typedef visitor base_t ;
            so_this_typedefs( direct_gpx_render ) ;
        
            so_typedefs( so_math::so_3d::trafof_t, trafo ) ;
            
            typedef so_std::stack<trafo_t,10> __stack_t ;
            so_typedefs( __stack_t, trafo_stack ) ;
            
        private:

            trafo_stack_t _trafo_stack ;
            so_gpx::iuser_system_ptr_t _gpx_system_ptr = nullptr ;

            so_gfx::ilens_ptr_t _lens_ptr = nullptr ;

            so_math::mat4f_t _view ;
            so_math::mat4f_t _proj ;
            so_math::vec4f_t _proj_params ;

            size_t _varset = 0 ;

        public:

            direct_gpx_render( void_t ) ;
            direct_gpx_render( so_gpx::iuser_system_ptr_t ) ;
            direct_gpx_render( this_rref_t ) ;
            virtual ~direct_gpx_render( void_t ) ;

        public:

            static this_ptr_t create( so_memory::purpose_cref_t ) ;
            static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
            static void_t destroy( this_ptr_t ) ;

        public:

            void_t set_view_matrix( so_math::mat4f_cref_t ) ;
            void_t set_proj_matrix( so_math::mat4f_cref_t ) ;
            void_t set_proj_params( so_math::vec4f_cref_t ) ;

            void_t set_lens( so_gfx::ilens_ptr_t ) ;

            void_t use_lens( void_t ) ;

            void_t use_varset( size_t const ) ;
        public:

            virtual so_scene::result visit( so_scene::so_node::transform_3d_ptr_t ) ;
            virtual so_scene::result post_visit( so_scene::so_node::transform_3d_ptr_t ) ;
            virtual so_scene::result visit( so_scene::so_node::render_state_ptr_t ) ;
            virtual so_scene::result post_visit( so_scene::so_node::render_state_ptr_t ) ;

            virtual so_scene::result visit( so_scene::so_node::camera_ptr_t ) ;
            virtual so_scene::result visit( so_scene::so_node::renderable_ptr_t ) ;
            
        public:

            virtual void_t destroy( void_t ) ;

        };
        so_typedef( direct_gpx_render ) ;
    }
}

#endif
