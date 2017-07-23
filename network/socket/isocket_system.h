//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_NETWORK_SOCKET_ISOCKET_SYSTEM_H_
#define _SNAKEOIL_NETWORK_SOCKET_ISOCKET_SYSTEM_H_

#include "../api.h"
#include "../typedefs.h"
#include "../protos.h"
#include "../result.h"

#include "../address/ip4_address.h"
#include "../binding/ip4_binding_point.h"

#include "socket_id.h"
#include "socket_funk.h"

#include <snakeoil/thread/primitive/sync_object.hpp>
#include <snakeoil/std/string/string.hpp>

namespace so_net
{
    class SNAKEOIL_NETWORK_API isocket_system
    {
    public:

        virtual so_net::result send( ip4_address_cref_t, uint_t const port, send_funk_t, so_thread::sync_object_ptr_t ) = 0 ;
        virtual so_net::result recv( ip4_binding_point_cref_t, recv_funk_t, so_thread::sync_object_ptr_t ) = 0 ;

    public:

        virtual void_t destroy( void_t ) = 0 ;
    };
    so_typedef( isocket_system ) ;
}

#endif