//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "../protos.h"
#include "../result.h"
#include "../typedefs.h"
#include "../api.h"

#include "image_render_2d_shared_data.h"

#include <snakeoil/gpx/plug/iplug_factory.h>

namespace so_gfx
{
    class image_render_2d_plug_factory : public so_gpx::iplug_factory
    {
        so_this_typedefs( image_render_2d_plug_factory ) ;

    private:

        so_gfx::image_render_2d_shared_data_ptr_t _sd ;

    public:

        image_render_2d_plug_factory( so_gfx::image_render_2d_shared_data_ptr_t ) ;
        image_render_2d_plug_factory( this_cref_t ) = delete ;
        image_render_2d_plug_factory( this_rref_t ) ;
        virtual ~image_render_2d_plug_factory( void_t ) ;

    public:

        static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
        static void_t destroy( this_ptr_t ) ;

    public:

        virtual so_gpx::iplug_ptr_t create_plug( so_gpu::api_type const, so_gpu::iapi_ptr_t ) ;
        virtual void_t destroy_plug( so_gpx::iplug_ptr_t ) ;

    public:

        virtual void_t destroy( void_t ) ;
    };
    so_typedef( image_render_2d_plug_factory ) ;
}