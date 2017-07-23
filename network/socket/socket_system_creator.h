//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_NETWORK_SOCKET_SOCKET_SYSTEM_CREATOR_H_
#define _SNAKEOIL_NETWORK_SOCKET_SOCKET_SYSTEM_CREATOR_H_

#include "../api.h"
#include "isocket_system.h"

namespace so_net
{
    struct SNAKEOIL_NETWORK_API socket_system_creator
    {
        static isocket_system_ptr_t create_null_system( void_t ) ;
        static isocket_system_ptr_t create_os_system( void_t ) ;
    };
    so_typedef( socket_system_creator ) ;
}

#endif