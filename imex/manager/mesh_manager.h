//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_IMEX_MANAGER_MESH_MANAGER_H_
#define _SNAKEOIL_IMEX_MANAGER_MESH_MANAGER_H_

#include "../api.h"

#include "base_resource_manager.hpp"
#include <snakeoil/geometry/protos.h>

namespace so_imex
{
    /// managing geometry assets
    class SNAKEOIL_IMEX_API mesh_manager : 
        public base_resource_manager<so_geo::imesh>
    {
        so_this_typedefs( mesh_manager ) ;
        so_typedefs( base_resource_manager<so_geo::imesh>, base ) ;

    public:

        mesh_manager( void_t ) ;
        mesh_manager( this_rref_t ) ;
        virtual ~mesh_manager( void_t ) ;

    public:

        static this_ptr_t create( so_memory::purpose_cref_t ) ;
        static void_t destroy( this_ptr_t ) ;

    public:

        virtual void_t destroy( void_t ) ;
       
    };
    so_typedef( mesh_manager ) ;

}

#endif
