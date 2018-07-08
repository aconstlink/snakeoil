//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "../resource_bridge.h"

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
        class SNAKEOIL_GFX_API gl33_resource_bridge_plug :
            public so_gpx::plug<so_gpu::so_gl::igl_33_api >
        {
            typedef so_gpx::plug<so_gpu::so_gl::igl_33_api > this_base_t ;
            so_this_typedefs( gl33_resource_bridge_plug ) ;


        protected:

            so_gfx::resource_bridge_t::shared_data_ptr_t _sd_ptr = nullptr ;

        public:

            gl33_resource_bridge_plug( resource_bridge_t::shared_data_ptr_t,
                so_gpu::so_gl::igl_33_api_ptr_t ) ;
            gl33_resource_bridge_plug( this_cref_t ) = delete ;
            gl33_resource_bridge_plug( this_rref_t ) ;
            virtual ~gl33_resource_bridge_plug( void_t ) ;

            static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
            static void_t destroy( this_ptr_t ) ;

        public:

            virtual so_gpx::plug_result on_load( load_info_cref_t ) final ;
            virtual so_gpx::plug_result on_unload( void_t ) final;
            virtual so_gpx::plug_result on_initialize( init_info_cref_t ) final;
            virtual so_gpx::plug_result on_release( void_t ) final;
            virtual so_gpx::plug_result on_execute( execute_info_cref_t ) final;
            virtual so_gpx::plug_result on_update( update_info_cref_t ) final;

            virtual void_t destroy( void_t ) ;

        private:

            void_t upload_image( so_std::string_cref_t imex_name, so_std::string_cref_t gpu_name,
                execute_info_cref_t) ;
        } ;
        so_typedef( gl33_resource_bridge_plug ) ;
    }
}