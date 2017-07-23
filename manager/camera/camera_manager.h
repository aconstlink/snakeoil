//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_MANAGER_CAMERA_CAMERA_MANAGER_H_
#define _SNAKEOIL_MANAGER_CAMERA_CAMERA_MANAGER_H_

#include "../base_manager.hpp"

#include <snakeoil/gfx/protos.h>

#include <snakeoil/thread/mutex.h>
#include <snakeoil/std/container/map.hpp>

namespace so_manager
{
    class SNAKEOIL_MANAGER_API camera_manager : 
        public so_manager::base_manager<so_gfx::icamera>
    {

        so_this_typedefs( camera_manager ) ;
        so_typedefs( base_manager<so_gfx::icamera>, base ) ;

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