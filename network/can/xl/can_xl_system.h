//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_NETWORK_CAN_CAN_XL_SYSTEM_H_
#define _SNAKEOIL_NETWORK_CAN_CAN_XL_SYSTEM_H_

#include "../../typedefs.h"
#include "../../protos.h"
#include "../../api.h"
#include "../../result.h"

#include "../can_funk.h"

#include <snakeoil/thread/primitive/sync_object.hpp>
#include <snakeoil/std/container/vector.hpp>

#define  STRICT
#include <windows.h>
#define RECEIVE_EVENT_SIZE 1                // DO NOT EDIT! Currently 1 is supported only
#define RX_QUEUE_SIZE      4096             // internal driver queue size in CAN events
#include <vxlapi.h>

namespace so_net
{
    namespace so_can
    {
        class SNAKEOIL_NETWORK_API can_xl_system
        {
            so_this_typedefs( can_xl_system ) ;

        private:

            struct recv_data
            {
                so_this_typedefs( recv_data ) ;

            public:

                XLportHandle hnd_port ;
                bool_t fd_support = false ;

            public:

                recv_data( void_t ){}
                recv_data( this_cref_t ) = delete ;
                recv_data( this_rref_t rhv ){
                    hnd_port = rhv.hnd_port ;
                    rhv.hnd_port = XL_INVALID_PORTHANDLE ;
                    fd_support = rhv.fd_support ;
                    rhv.fd_support = false ;
                }
                ~recv_data( void_t ){}

            };
            so_typedef( recv_data ) ;

            so_typedefs( so_std::vector<recv_data_ptr_t>, recvs ) ;
            recvs_t _recvs ;

        private:

            struct send_data
            {
                so_this_typedefs( send_data ) ;

            public:

                XLportHandle hnd_port ;
                bool_t fd_support = false ;

            public:

                send_data( void_t ){}
                send_data( this_cref_t ) = delete ;
                send_data( this_rref_t rhv ){
                    hnd_port = rhv.hnd_port ;
                    rhv.hnd_port = XL_INVALID_PORTHANDLE ;
                    fd_support = rhv.fd_support ;
                    rhv.fd_support = false ;
                }
                ~send_data( void_t ){}

            };
            so_typedef( send_data ) ;

            so_typedefs( so_std::vector<send_data_ptr_t>, sends ) ;
            sends_t _sends ;

        private:

            bool_t _released = true ;
            XLdriverConfig _config ;

        public:

            can_xl_system( void_t ) ;
            can_xl_system( this_cref_t ) = delete ;
            can_xl_system( this_rref_t ) ;
            ~can_xl_system( void_t ) ;

        public:

            so_net::result init( void_t ) ;
            so_net::result release( void_t ) ;

        public:

            struct recv_init_data
            {
                size_t channel = 0 ;

                so_thread::sync_object_ptr_t finish_sync_ptr = nullptr ;
            };
            so_typedef( recv_init_data ) ;

            so_net::result recv( recv_init_data_cref_t, so_net::so_can::can_recv_funk_t ) ;

        public:

            struct send_init_data
            {
                size_t channel = 1 ;
                so_thread::sync_object_ptr_t finish_sync_ptr = nullptr ;
            };
            so_typedef( send_init_data ) ;

            so_net::result send( send_init_data_cref_t, so_net::so_can::can_send_funk_t ) ;
            

        };
        so_typedef( can_xl_system ) ;
    }
}

#endif