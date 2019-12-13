//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "../../typedefs.h"
#include "protos.h"
#include "../../content/itransition.h"

#include <snakeoil/flow/slot/output/output_slot.hpp>
#include <snakeoil/flow/node/variable/variable_node.h>

#include <snakeoil/gpx/system/render_system.h>

#include <functional>

namespace sox_presentation
{
    namespace transitions
    {
        class SNAKEOILX_PRESENTATION_API sdf_a
        {
            so_this_typedefs( sdf_a ) ;

        private:


        private:

            sdf_a_plug_factory_ptr_t _fac_ptr = nullptr ;
            sdf_a_shared_data_ptr_t _sd_ptr = nullptr ;
            so_gpx::render_system_ptr_t _gpxr = nullptr ;

            so_gpx::technique_id_t _tid = so_gpx::technique_id_t( -1 ) ;

        public:

            sdf_a( so_gpx::render_system_ptr_t ) noexcept ;
            sdf_a( this_cref_t ) = delete ;
            sdf_a( this_rref_t ) noexcept ;
            ~sdf_a( void_t ) noexcept ;

        public:

            static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
            static void_t destroy( this_ptr_t ) ;

            void_t init( void_t ) ;

            void_t set_view_projection( so_math::mat4f_cref_t view, so_math::mat4f_cref_t proj ) ;

            so_flow::variable_node_ptr_t get_var_node( void ) ;

            void_t update( std::chrono::milliseconds const t ) ;

        public: // render

            void_t reload( void_t ) ;
            void_t render( sox_presentation::itransition_t::render_type const ) ;
            void_t release( void_t ) ;

        };
        so_typedef( sdf_a ) ;
    }
}