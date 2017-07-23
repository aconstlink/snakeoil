//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_IMEX_MANAGER_CAMERA_MANAGER_H_
#define _SNAKEOIL_IMEX_MANAGER_CAMERA_MANAGER_H_

#include "../api.h"
#include "../protos.h"

#include "base_resource_manager.hpp"

namespace so_imex
{
    class SNAKEOIL_IMEX_API camera_manager : 
        public base_resource_manager<so_imex::icamera>
    {
        so_this_typedefs( camera_manager ) ;
        so_typedefs( base_resource_manager<so_imex::icamera>, base ) ;

    public:

        camera_manager( void_t ) ;
        camera_manager( this_rref_t ) ;
        virtual ~camera_manager( void_t ) ;

    public:

        static this_ptr_t create( so_memory::purpose_cref_t ) ;
        static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
        static void_t destroy( this_ptr_t ) ;

    public:

        virtual void_t destroy( void_t ) ;
    };
    so_typedef( camera_manager ) ;
}

#endif
