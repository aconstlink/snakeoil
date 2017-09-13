//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "../../typedefs.h"

namespace so_device
{
    enum class key_state
    {
        none,
        pressed,
        pressing,
        released
    };

    static bool_t is_valid( key_state ks ) { return ks != key_state::none ; }
    static bool_t is_invalid( key_state ks ) { return ks == key_state::none ; }
}