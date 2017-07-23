//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_IMEX_MANAGER_MATERIAL_MANAGER_H_
#define _SNAKEOIL_IMEX_MANAGER_MATERIAL_MANAGER_H_

#include "../../api.h"

#include "../base_resource_manager.hpp"
#include <snakeoil/shade/protos.h>

namespace so_imex
{
    class SNAKEOIL_IMEX_API material_manager : 
        public base_resource_manager<so_shade::so_material::imaterial>
    {
        so_this_typedefs( material_manager ) ;
        so_typedefs( base_resource_manager<so_shade::so_material::imaterial>, base ) ;

    public:

        material_manager( void_t ) ;
        material_manager( this_rref_t ) ;
        virtual ~material_manager( void_t ) ;

    public:

        static this_ptr_t create( so_memory::purpose_cref_t ) ;
        static void_t destroy( this_ptr_t ) ;

    public:

        virtual void_t destroy( void_t ) ;
        
    } ;
    so_typedef( material_manager ) ;
}

#endif
