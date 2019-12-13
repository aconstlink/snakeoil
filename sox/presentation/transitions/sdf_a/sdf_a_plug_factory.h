//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "sdf_a.h"
#include "sdf_a_shared_data.h"

#include <snakeoil/gpx/plug/iplug_factory.h>

namespace sox_presentation
{
    namespace transitions
    {
        class  sdf_a_plug_factory : public so_gpx::iplug_factory
        {
            so_this_typedefs( sdf_a_plug_factory ) ;

        private:

            sox_presentation::transitions::sdf_a_shared_data_ptr_t _sd ;

        public:

            sdf_a_plug_factory( sox_presentation::transitions::sdf_a_shared_data_ptr_t ) ;
            sdf_a_plug_factory( this_cref_t ) = delete ;
            sdf_a_plug_factory( this_rref_t ) ;
            ~sdf_a_plug_factory( void_t ) ;

        public:

            static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
            static void_t destroy( this_ptr_t ) ;

        public:

            virtual so_gpx::iplug_ptr_t create_plug( so_gpu::api_type const, so_gpu::iapi_ptr_t ) ;
            virtual void_t destroy_plug( so_gpx::iplug_ptr_t ) ;

        public:

            virtual void_t destroy( void_t ) ;
        };
        so_typedef( sdf_a_plug_factory ) ;
    }
}