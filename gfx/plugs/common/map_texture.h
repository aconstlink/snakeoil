//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "../../api.h"
#include "../../protos.h"
#include "../../typedefs.h"

#include <snakeoil/gpx/system/render_system.h>
#include <snakeoil/std/string/string.hpp>

namespace so_gfx
{
    class SNAKEOIL_GFX_API map_texture
    {
        so_this_typedefs( map_texture ) ;

    private:

    public:

        struct shared_data
        {
            so_std::string_t name ;
        };
        so_typedef( shared_data ) ;
        shared_data_ptr_t _sd ;

        so_gpx::technique_id_t _tid ;
        so_gpx::render_system_ptr_t _rs ;

    public:

        map_texture( so_std::string_cref_t, so_gpx::render_system_ptr_t ) ;
        map_texture( this_cref_t ) = delete ;
        map_texture( this_rref_t ) ;
        ~map_texture( void_t ) ;

    public:

        static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
        static void_t destroy( this_ptr_t ) ;

    public:

        void_t schedule( void_t ) ;
    };
    so_typedef( map_texture ) ;
}