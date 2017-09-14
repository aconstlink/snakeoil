//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_IMEX_TYPEDEFS_H_
#define _SNAKEOIL_IMEX_TYPEDEFS_H_

#include <snakeoil/io/typedefs.h>

#include <snakeoil/std/string/string.hpp>
#include <snakeoil/std/string/utf8.hpp>
#include <snakeoil/std/container/list.hpp>
#include <snakeoil/std/container/vector.hpp>

#include <snakeoil/memory/global.h>

#include <snakeoil/core/assert.h>
#include <snakeoil/core/types.hpp>
#include <snakeoil/core/macros/typedef.h>
#include <snakeoil/core/macros/move.h>
#include <snakeoil/core/boolean.h>
#include <snakeoil/core/checks.h>

#include <algorithm>

namespace so_imex
{
    using namespace so_core::so_types ;

    so_typedefs( so_std::string, string ) ;
    so_typedefs( so_std::string, key ) ;
    so_typedefs( so_std::list<key_t>, key_list ) ;
    so_typedefs( so_std::vector<so_std::string_t>, file_extension_names ) ;

    using memory = so_memory::global ;
}

#endif

