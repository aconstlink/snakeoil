//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "../visitor.h"

#include <snakeoil/gfx/protos.h>
#include <snakeoil/std/container/vector.hpp>

namespace so_scene
{
    namespace so_visitor
    {
        /// implements the ivisitor interface with default
        /// behavior. The default behavior is to do nothing.
        class SNAKEOIL_SCENE_API camera_collector : public visitor
        {
            so_this_typedefs( camera_collector ) ;

        private:

            so_typedefs( so_std::vector< so_gfx::icamera_cstr_t >, cameras ) ;
            cameras_t _cams ;

        public:

            camera_collector( void_t ) noexcept ;
            camera_collector( this_cref_t ) = delete ;
            camera_collector( this_rref_t ) noexcept ;
            virtual ~camera_collector( void_t ) noexcept ;

        public:

            virtual so_scene::result visit( so_scene::so_node::camera_ptr_t ) ;

        public:

            static this_ptr_t create( so_memory::purpose_cref_t ) ;
            static void_t destroy( this_ptr_t ) ;
            virtual void_t destroy( void_t ) ;

        public:

            size_t get_num_cameras( void_t ) const noexcept { return _cams.size() ; }
            so_gfx::icamera_cptr_t get_camera( size_t const ) const noexcept ;
        };
        so_typedef( camera_collector ) ;
    }
}
