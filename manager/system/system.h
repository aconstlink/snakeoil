//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_MANAGER_SYSTEM_SYSTEM_H_
#define _SNAKEOIL_MANAGER_SYSTEM_SYSTEM_H_

#include "../typedefs.h"
#include "../protos.h"
#include "../api.h"
#include "../result.h"

#include <snakeoil/thread/mutex.h>

#include <snakeoil/std/string/string.hpp>
#include <snakeoil/std/container/map.hpp>

namespace so_manager
{
	class SNAKEOIL_MANAGER_API system
    {
        so_this_typedefs( system ) ;

    private:

        typedef so_std::map< so_std::string_t, so_manager::manager_registry_ptr_t > __registries_t ;
        so_typedefs( __registries_t, registries ) ;

    private:

        so_thread::mutex_t _mtx ;
        registries_t _regs ;

    public:

        system( void_t ) ;
        system( this_rref_t ) ;
        ~system( void_t ) ;

    public:

        static this_ptr_t create( so_memory::purpose_cref_t ) ;
        static void_t destroy( this_ptr_t ) ;

    public:

        so_manager::result register_registry( so_std::string_cref_t, 
            so_manager::manager_registry_ptr_t ) ;

        so_manager::manager_registry_ptr_t find_registry( so_std::string_cref_t ) ;

        so_manager::result shutdown( void_t ) ;

    };
    so_typedef( system ) ;
}

#endif
