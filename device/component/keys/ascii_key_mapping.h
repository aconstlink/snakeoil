//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "ascii_key.h"
#include "../../../component/keys/key_id.h"

#include <snakeoil/log/log.h>

namespace so_device
{
    /// maps a ascii keyboard device to a key_device key.
    /// it performs a one to one mapping.
    static so_device::so_component::key_id ascii_key_to_key( so_device::ascii_key tb )
    {
        bool_t const cond = size_t( tb ) < size_t( so_device::so_component::key_id::num_keys ) ;
        so_log::log::error_and_exit( so_core::is_not(cond), "[ascii_key_to_key]" ) ;
        
        return so_device::so_component::key_id( size_t( tb ) ) ;
    }

    /// maps a button_device button to a three button.
    static so_device::ascii_key key_to_ascii_key( so_device::so_component::key_id b )
    {
        return ( size_t( b ) < size_t( ascii_key::num_keys ) ) ? ascii_key( size_t( b ) ) : ascii_key::invalid ;
    }
}

