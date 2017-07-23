//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_IMEX_SYNC_OBJECT_H_
#define _SNAKEOIL_IMEX_SYNC_OBJECT_H_

#include "result.h"
#include "api.h"

#include <snakeoil/thread/primitive/sync_object.hpp>
#include <snakeoil/core/macros/typedef.h>

namespace so_imex
{
    class SNAKEOIL_IMEX_API sync_object
    {
        so_this_typedefs( sync_object ) ;

    private:

        so_imex::result _res = so_imex::invalid ;
        so_thread::sync_object _so ;

    private:

        sync_object( this_cref_t ) {}

    public:

        sync_object( void_t ) ;

        sync_object( this_rref_t rhv ) ;

        ~sync_object( void_t ) ;

    public:

        static this_ptr_t create( so_memory::purpose_cref_t ) ;
        static void_t destroy( this_ptr_t ) ;

        static void_t set_and_signal( this_ptr_t, so_imex::result ) ;
        static void_t set_not_signal( this_ptr_t, so_imex::result ) ;

    public:

        void_t set( so_imex::result res ) ;

        /// should be called by the producer
        void_t set_and_signal( so_imex::result res ) ;

        /// should be called by the consumer
        /// wait until this sync object has been signaled.
        so_imex::result wait( void_t ) ;
    };
    so_typedef( sync_object ) ;
}

#endif
