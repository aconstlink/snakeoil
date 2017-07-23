//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_NETWORK_BINDING_IP4_BINDING_POINT_H_
#define _SNAKEOIL_NETWORK_BINDING_IP4_BINDING_POINT_H_

#include "../address/ip4_address.h"

namespace so_net
{
    struct ip4_binding_point
    {
        ip4_address_t address ;
        uint_t port ;
    };
    so_typedef( ip4_binding_point ) ;
}

#endif