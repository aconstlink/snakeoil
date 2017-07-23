//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_IMEX_MANAGER_IMAGE_MANAGER_H_
#define _SNAKEOIL_IMEX_MANAGER_IMAGE_MANAGER_H_

#include "../api.h"
#include "../protos.h"

#include "base_resource_manager.hpp"

namespace so_imex
{
    /// managing image assets
    class SNAKEOIL_IMEX_API image_manager : 
        public base_resource_manager<so_imex::iimage>
    {
        so_this_typedefs( image_manager ) ;

        so_typedefs( base_resource_manager<so_imex::iimage>, base ) ;

    public:

        image_manager( void_t ) ;
        image_manager( this_rref_t ) ;
        virtual ~image_manager( void_t ) ;

    public:

        static this_ptr_t create( so_memory::purpose_cref_t ) ;
        static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
        static void_t destroy( this_ptr_t ) ;

    public:

        virtual void_t destroy( void_t ) ;
    };
    so_typedef( image_manager ) ;
}

#endif
