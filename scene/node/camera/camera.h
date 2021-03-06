//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "../leaf.h"

#include <snakeoil/gfx/camera/icamera.h>
#include <snakeoil/math/utility/3d/transformation.hpp>

namespace so_scene
{
    namespace so_node
    {
        class SNAKEOIL_SCENE_API camera : public leaf
        {
            so_this_typedefs( camera ) ;

        private:

            so_gfx::icamera_ptr_t _cam_ptr = nullptr ;

        public:

            camera( void_t ) noexcept ;
            camera( so_gfx::icamera_ptr_t ) noexcept ;
            camera( this_rref_t ) noexcept ;
            virtual ~camera( void_t ) noexcept ;

        public:

            static this_ptr_t create( so_memory::purpose_cref_t ) ;
            static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
            static void_t destroy( this_ptr_t ) ;

        public:

            void_t transform_by( so_math::so_3d::trafof_cref_t ) ;
            void_t set_transformation( so_math::so_3d::trafof_cref_t ) ;
            so_gfx::icamera_cstr_t get_camera( void_t ) const noexcept ;

        public:

            virtual so_scene::result apply( so_scene::so_visitor::ivisitor_ptr_t ) ;
            virtual void_t destroy( void_t ) ;
        } ;
        so_typedef( camera ) ;
    }
}

