//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_AUDIO_TYPEDEFS_H_
#define _SNAKEOIL_AUDIO_TYPEDEFS_H_

#include <snakeoil/std/string/string.hpp>

#include <snakeoil/memory/global.h>

#include <snakeoil/core/boolean.h>
#include <snakeoil/core/checks.h>
#include <snakeoil/core/types.hpp>
#include <snakeoil/core/macros/typedef.h>
#include <snakeoil/core/macros/move.h>

namespace so_audio
{
    using namespace so_core::so_types ;
    using memory = so_memory::global ;

    so_typedefs( so_std::string_t, key ) ;

    typedef std::pair< size_t, size_t > __range_t ;
    so_typedefs( __range_t, range ) ;
}

#endif

