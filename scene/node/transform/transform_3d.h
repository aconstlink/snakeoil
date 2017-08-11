//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_SCENE_NODE_TRANSFORM_TRANSFORM_3D_H_
#define _SNAKEOIL_SCENE_NODE_TRANSFORM_TRANSFORM_3D_H_

#include "../decorator.h"

#include <snakeoil/flow/variable/variable.hpp>
#include <snakeoil/math/utility/3d/transformation.hpp>

namespace so_scene
{
    namespace so_node
    {
        class SNAKEOIL_SCENE_API transform_3d : public decorator
        {
            typedef decorator base_t ;
            so_this_typedefs( transform_3d ) ;

            so_typedefs( so_math::so_3d::trafof_t, trafo ) ;
            so_typedefs( so_flow::variable< so_math::mat4f_t >, var_mat4f ) ;

        private:

            trafo_t _trafo ;
            var_mat4f_ptr_t _var_trafo ;
            
        public:

            transform_3d( void_t ) ;
            transform_3d( this_rref_t ) ;
            transform_3d( this_cref_t ) ;
            transform_3d( so_math::so_3d::trafof_cref_t ) ;
            transform_3d( iparent_ptr_t ) ;
            transform_3d( node_ptr_t, iparent_ptr_t=nullptr ) ;
            virtual ~transform_3d( void_t ) ;

        private:

            void_t ctor_init( void_t ) ;

        public:

            static this_ptr_t create( so_memory::purpose_cref_t ) ;
            static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
            static void_t destroy( this_ptr_t ) ;

        public:

            this_ref_t set_decorated( node_ptr_t ) ;

        public:

            void_t set_transformation( trafo_cref_t ) ;
            trafo_cref_t get_transformation( void_t ) const ;
            trafo_t compute_trafo( void_t ) const ;

        public:

            virtual so_scene::result apply( so_scene::so_visitor::ivisitor_ptr_t ptr ) ;

        public:

            virtual void_t destroy( void_t ) ;
        };
        so_typedef( transform_3d ) ;
    }
}

#endif
