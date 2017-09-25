//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "api.h"
#include "protos.h"
#include "typedefs.h"

#include "handle/handle.h"

#include <snakeoil/thread/mutex.h>

namespace so_io
{
    class SNAKEOIL_IO_API global
    {
        so_this_typedefs( global ) ;

    private:

        so_io::system_ptr_t _ios = nullptr ;

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
        static system_ptr_t io_system( void_t ) ;

        static so_io::load_handle_t load( so_io::path_cref_t ) ;
        static so_io::store_handle_t store( so_io::path_cref_t, char_cptr_t, size_t ) ;
    };
    so_typedef( global ) ;
}