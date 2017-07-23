//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_NETWORK_SOCKET_SOCKET_FUNK_H_
#define _SNAKEOIL_NETWORK_SOCKET_SOCKET_FUNK_H_

#include "../typedefs.h"

#include <functional>
#include <array>

namespace so_net
{
    struct sender_info{};
    so_typedef( sender_info ) ;

    typedef std::function< size_t ( byte_cptr_t, size_t const, sender_info_cref_t ) > recv_funk_t ;


    typedef std::array<byte_t, 2048> send_array_t ;
    typedef std::function< size_t ( send_array_t &, size_t & ) > send_funk_t ;
}

#endif