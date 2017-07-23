//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GEOMETRY_TYPEDEFS_H_
#define _SNAKEOIL_GEOMETRY_TYPEDEFS_H_

#include <snakeoil/io/typedefs.h>

#include <snakeoil/memory/memory.h>
#include <snakeoil/memory/allocator.hpp>

#include <snakeoil/std/container/vector.hpp>
#include <snakeoil/std/string/string.hpp>

#include <snakeoil/core/assert.h>
#include <snakeoil/core/types.hpp>
#include <snakeoil/core/macros/typedef.h>

namespace so_geo
{
    using namespace so_core::so_types ;

    so_typedefs( so_std::string_t, string ) ;
    so_typedefs( so_std::string_t, key ) ;
    

    using memory = so_memory::memory ;


    so_typedefs( so_std::vector< uint_t >, uints ) ;
    so_typedefs( so_std::vector< uints_t >, more_uints ) ;
    
    so_typedefs( so_std::vector< float_t >, floats ) ;
    so_typedefs( so_std::vector< floats_t >, more_floats ) ;    
}

#endif

