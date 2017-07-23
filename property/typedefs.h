//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_PROPERTY_TYPEDEFS_H_
#define _SNAKEOIL_PROPERTY_TYPEDEFS_H_

#include <snakeoil/std/container/map.hpp> 
#include <snakeoil/std/string/string.hpp>
#include <snakeoil/std/string/utf8.hpp>

#include <snakeoil/memory/memory.h>
#include <snakeoil/memory/allocator.hpp>

#include <snakeoil/log/typedefs.h>

#include <snakeoil/core/macros/typedef.h>
#include <snakeoil/core/macros/move.h>
#include <snakeoil/core/types.hpp>
#include <snakeoil/core/checks.h>
#include <snakeoil/core/boolean.h>

#include "protos.h"

namespace so_property
{
    using namespace so_core::so_types ;

    using memory = so_memory::memory ;

    template< typename T >
    using allocator = so_memory::allocator< T, so_property::memory > ;

    typedef so_std::map< so_std::string_t, so_property::iproperty_ptr_t > __property_map_t ;
    so_typedefs( __property_map_t, property_map ) ;
}

#endif

