//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "api.h"
#include "protos.h"
#include "typedefs.h"

#include <snakeoil/thread/mutex.h>

namespace so_imex
{
    class SNAKEOIL_IMEX_API global
    {
        so_this_typedefs( global ) ;

    private:

        so_imex::system_ptr_t _system_ptr = nullptr ;

    private:

        static so_thread::mutex_t _mtx ;
        static this_ptr_t _ptr ;

    public:

        global( void_t ) ;
        global( this_cref_t ) = delete ;
        global( this_rref_t ) ;
        ~global( void_t ) ;

    public:

        static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
        static void_t destroy( this_ptr_t ) ;

    public:

        static this_ptr_t init( void_t ) ;
        static void_t deinit( void_t ) ;
        
        static this_ptr_t get( void_t ) ;


        static so_imex::isystem_ptr_t system( void_t ) ;
        static so_imex::module_registry_ptr_t module_registry( void_t ) ;
        static so_imex::manager_registry_ptr_t manager_registry( void_t ) ;
    };
    so_typedef( global ) ;
}