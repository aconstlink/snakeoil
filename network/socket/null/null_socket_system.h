//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_NETWORK_SOCKET_NULL_NULL_SOCKET_SYSTEM_H_
#define _SNAKEOIL_NETWORK_SOCKET_NULL_NULL_SOCKET_SYSTEM_H_

#include "../isocket_system.h"

#include <snakeoil/thread/mutex.h>

namespace so_net
{
    class SNAKEOIL_NETWORK_API null_socket_system : public isocket_system
    {
        so_this_typedefs( null_socket_system ) ;

    private:

        so_thread::mutex_t _mtx_udp ;
        udp_socket_id_t _udp_id = udp_socket_id_t( -1 ) ;

    public:

        null_socket_system( void_t )  ;
        null_socket_system( this_cref_t ) = delete ;
        null_socket_system( this_rref_t ) ;
        virtual ~null_socket_system( void_t ) ;

    public:

        static this_ptr_t create( so_memory::purpose_cref_t ) ;
        static void_t destroy( this_ptr_t ) ;

    public:

        virtual so_net::result send( ip4_address_cref_t, uint_t const port, send_funk_t, so_thread::sync_object_ptr_t ) ;
        virtual so_net::result recv( ip4_binding_point_cref_t, recv_funk_t, so_thread::sync_object_ptr_t ) ;

    public:

        virtual void_t destroy( void_t ) ;
    };
    so_typedef( null_socket_system ) ;
}

#endif