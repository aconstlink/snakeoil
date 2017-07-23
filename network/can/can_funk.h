//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_NETWORK_CAN_CAN_FUNK_H_
#define _SNAKEOIL_NETWORK_CAN_CAN_FUNK_H_

#include "../typedefs.h"

#include <functional>
#include <array>

namespace so_net
{
    namespace so_can
    {
        typedef std::array< byte_t, 8 > __can_data_t ;
        so_typedefs( __can_data_t, can_data ) ;

        typedef std::function< size_t ( can_data_cref_t ) > can_recv_funk_t ;

        typedef std::function< size_t ( can_data_ref_t ) > can_send_funk_t ;
    }
}

#endif