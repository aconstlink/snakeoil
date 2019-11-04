//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "../typedefs.h"
#include "protos.h"
#include "post_shared_data.h"

#include <snakeoil/flow/slot/output/output_slot.hpp>
#include <snakeoil/flow/node/variable/variable_node.h>

#include <snakeoil/gpx/system/render_system.h>

#include <functional>

namespace sox_presentation
{
    class post
    {
        so_this_typedefs( post ) ;

    private:

                
    private:
        
        post_plug_factory_ptr_t _fac_ptr = nullptr ;
        post_shared_data_ptr_t _sd_ptr = nullptr ;
        so_gpx::render_system_ptr_t _gpxr = nullptr ;
        
        so_gpx::technique_id_t _tid = so_gpx::technique_id_t( -1 ) ;

    public:

        post( so_gpx::render_system_ptr_t ) ;
        post( this_cref_t ) = delete ;
        post( this_rref_t ) ;
        ~post( void_t ) ;

    public:

        static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
        static void_t destroy( this_ptr_t ) ;

        void_t init( void_t ) ;
        void_t set_view_projection( so_math::mat4f_cref_t view, so_math::mat4f_cref_t proj ) ;

        so_flow::variable_node_ptr_t get_var_node( void ) ;

    public: // render

        void_t reload( void_t ) ;
        void_t render( void_t ) ;
        void_t release( void_t ) ;

    };
    so_typedef( post ) ;
}