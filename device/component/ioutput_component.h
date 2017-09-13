//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "../typedefs.h"
#include "../protos.h"
#include "../result.h"
#include "../api.h"

namespace so_device
{
    namespace so_output
    {
        struct ioutput_component
        {
            virtual void_t destroy( void_t ) = 0 ;
        };
    }
}
