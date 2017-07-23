//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_IO_IO_H_
#define _SNAKEOIL_IO_IO_H_

#include "api.h"
#include "protos.h"
#include "result.h"

#include "handle/handle.h"

namespace so_io
{
    // singleton io interface for easy file loading and saving.
    class SNAKEOIL_IO_API io
    {
    private:

        // system
        static so_io::system_ptr_t _ios ;

    public:

        static void_t init( void_t ) ;
        static void_t deinit( void_t ) ;
        static so_io::load_handle_t load( so_io::path_cref_t ) ;
        static so_io::store_handle_t store( so_io::path_cref_t, char_cptr_t, size_t ) ;
    };
}

#endif