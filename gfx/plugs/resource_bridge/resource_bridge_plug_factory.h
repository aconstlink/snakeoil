#pragma once

#include "resource_bridge.h"

#include <snakeoil/gpx/plug/iplug_factory.h>

namespace so_gfx
{
    class SNAKEOIL_GFX_API resource_bridge_plug_factory : public so_gpx::iplug_factory
    {
        so_this_typedefs( resource_bridge_plug_factory ) ;

    private:

        resource_bridge_t::shared_data_ptr_t _sd ;

    public:

        resource_bridge_plug_factory( resource_bridge_t::shared_data_ptr_t ) ;
        resource_bridge_plug_factory( this_cref_t ) = delete ;
        resource_bridge_plug_factory( this_rref_t ) ;
        ~resource_bridge_plug_factory( void_t ) ;

    public:

        static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
        static void_t destroy( this_ptr_t ) ;

    public:

        virtual so_gpx::iplug_ptr_t create_plug( so_gpu::api_type const, so_gpu::iapi_ptr_t ) ;
        virtual void_t destroy_plug( so_gpx::iplug_ptr_t ) ;

    public:

        virtual void_t destroy( void_t ) ;
        
    } ;
    so_typedef( resource_bridge_plug_factory ) ;
}