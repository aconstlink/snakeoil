//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_NOISE_TYPEDEFS_H_
#define _SNAKEOIL_NOISE_TYPEDEFS_H_

#include <snakeoil/memory/global.h>
#include <snakeoil/memory/allocator.hpp>

#include <snakeoil/core/macros/typedef.h>
#include <snakeoil/core/types.hpp>

namespace so_noise
{
    using namespace so_core::so_types ;

    using memory = so_memory::global ;

    template< typename T >
    using allocator = so_memory::allocator< T, so_noise::memory > ;
}

#endif

