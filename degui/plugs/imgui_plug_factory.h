//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "imgui.h"

#include <snakeoil/gpx/plug/iplug_factory.h>

namespace so_imgui
{
    class SNAKEOIL_DEGUI_API imgui_plug_factory : public so_gpx::iplug_factory
    {
        so_this_typedefs( imgui_plug_factory ) ;

    private:

        imgui::shared_data_ptr_t _sd ;

    public:

        imgui_plug_factory( imgui::shared_data_ptr_t ) ;
        imgui_plug_factory( this_cref_t ) = delete ;
        imgui_plug_factory( this_rref_t ) ;
        ~imgui_plug_factory( void_t ) ;

    public:

        static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
        static void_t destroy( this_ptr_t ) ;

    public:

        virtual so_gpx::iplug_ptr_t create_plug( so_gpu::api_type const, so_gpu::iapi_ptr_t ) ;
        virtual void_t destroy_plug( so_gpx::iplug_ptr_t ) ;

    public:

        virtual void_t destroy( void_t ) ;
    };
    so_typedef( imgui_plug_factory ) ;
}