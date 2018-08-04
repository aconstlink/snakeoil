//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_IO_TYPEDEFS_H_
#define _SNAKEOIL_IO_TYPEDEFS_H_

#include "result.h"

#include <snakeoil/memory/global.h>

#include <snakeoil/std/string/string.hpp>
#include <snakeoil/std/string/utf8.hpp>
#include <snakeoil/std/filesystem/filesystem.hpp>

#include <snakeoil/core/types.hpp>
#include <snakeoil/core/macros/typedef.h>
#include <snakeoil/core/assert.h>

#include <functional>

namespace so_io
{
    using namespace so_core::so_types ;

    using memory = so_memory::global ;
    so_typedefs( so_std::filesystem::path, path ) ;

    typedef std::function< void_t( char_cptr_t, size_t const, so_io::result const ) > load_completion_funk_t ;
    typedef std::function< void_t( so_io::result const ) > store_completion_funk_t ;

    typedef so_core::void_ptr_t internal_item_ptr_t ;
}

#endif

