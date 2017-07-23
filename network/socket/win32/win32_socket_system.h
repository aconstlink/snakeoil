//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_NETWORK_SOCKET_WIN32_SOCKET_SYSTEM_H_
#define _SNAKEOIL_NETWORK_SOCKET_WIN32_SOCKET_SYSTEM_H_

#include "../isocket_system.h"

#include <snakeoil/std/container/map.hpp>
#include <snakeoil/thread/mutex.h>

#include <WinSock2.h>

namespace so_net
{
    namespace so_win32
    {
        class SNAKEOIL_NETWORK_API win32_socket_system : public isocket_system
        {
            so_this_typedefs( win32_socket_system ) ;

        private:

            struct socket_data
            {
                so_this_typedefs( socket_data ) ;

                socket_data( void_t ){}
                socket_data( this_cref_t ) = delete ;
                socket_data( this_rref_t rhv ){
                    s = rhv.s ;
                    rhv.s = NULL ;
                }
                ~socket_data( void_t ){}

                SOCKET s ;
            };
            so_typedef( socket_data ) ;

            typedef so_std::map< udp_socket_id_t, socket_data_ptr_t > __udp_sockets_t ;
            so_typedefs( __udp_sockets_t, udp_sockets ) ;

        private:

            so_thread::mutex_t _mtx_udps ;
            udp_sockets_t _udps ;
            udp_socket_id_t _udp_id = udp_socket_id_t( 0 ) ;

            bool_t _is_init = false ;

        public:

            win32_socket_system( void_t ) ;
            win32_socket_system( this_cref_t ) = delete ;
            win32_socket_system( this_rref_t ) ;
            virtual ~win32_socket_system( void_t ) ;

        public:

            static this_ptr_t create( so_memory::purpose_cref_t ) ;
            static void_t destroy( this_ptr_t ) ;

        public:

            virtual so_net::result send( ip4_address_cref_t, uint_t const port, send_funk_t, so_thread::sync_object_ptr_t ) ;
            virtual so_net::result recv( ip4_binding_point_cref_t, recv_funk_t, so_thread::sync_object_ptr_t ) ;

        public:

            virtual void_t destroy( void_t ) ;

        private:

            udp_socket_id_t create_udp_socket_id( void_t ) ;
        };
        so_typedef( win32_socket_system ) ;
    }
}

#endif