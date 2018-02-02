//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "map_texture.h"

#include <snakeoil/gpx/plug/iplug_factory.h>

namespace so_gfx
{
    class SNAKEOIL_GFX_API map_texture_plug_factory : public so_gpx::iplug_factory
    {
        so_this_typedefs( map_texture_plug_factory ) ;

    private:

        map_texture::shared_data_ptr_t _sd ;

    public:

        map_texture_plug_factory( map_texture::shared_data_ptr_t ) ;
        map_texture_plug_factory( this_cref_t ) = delete ;
        map_texture_plug_factory( this_rref_t ) ;
        ~map_texture_plug_factory( void_t ) ;

    public:

        static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
        static void_t destroy( this_ptr_t ) ;

    public:

        virtual so_gpx::iplug_ptr_t create_plug( so_gpu::api_type const, so_gpu::iapi_ptr_t ) ;
        virtual void_t destroy_plug( so_gpx::iplug_ptr_t ) ;

    public:

        virtual void_t destroy( void_t ) ;
    };
    so_typedef( map_texture_plug_factory ) ;
}