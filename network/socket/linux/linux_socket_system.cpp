//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "linux_socket_system.h"

#include <snakeoil/log/log.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

using namespace so_net ;
using namespace so_net::so_linux ;

//**********************************************************************************
linux_socket_system::linux_socket_system( void_t )
{
    _is_init = true ;
}

//**********************************************************************************
linux_socket_system::linux_socket_system( this_rref_t rhv )
{
    _udps = std::move( rhv._udps ) ;
    _is_init = rhv._is_init ;
    rhv._is_init = false ;
}

//**********************************************************************************
linux_socket_system::~linux_socket_system( void_t )
{
    // @todo delete/release sockets

    for( auto & sd : _udps )
    {
        auto const res = close( sd.second->s ) ;
        so_log::log::error( res == -1, 
            "[linux_socket_system::~linux_socket_system] : close"  ) ;

        so_memory::memory::dealloc( sd.second ) ;
    }
}

//**********************************************************************************
linux_socket_system::this_ptr_t linux_socket_system::create( so_memory::purpose_cref_t p )
{
    return so_net::memory::alloc( this_t(), p ) ;
}

//**********************************************************************************
void_t linux_socket_system::destroy( this_ptr_t ptr )
{
    so_net::memory::dealloc( ptr ) ;
}

//**********************************************************************************
so_net::result linux_socket_system::send( ip4_address_cref_t addr, uint_t const port, 
    send_funk_t user_funk, so_thread::sync_object_ptr_t )
{
    socket_data_t sd ;

    {
        auto s = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP ) ;
        if( s == -1 )
        {
            so_log::log::error( "[linux_socket_system::send] : socket" ) ;
            return so_net::failed ;
        }

        sd.s = s ;
    }

    socket_data_ptr_t sdp = nullptr ;
    udp_socket_id_t sid = udp_socket_id_t(-1) ;

    {
        sid = this_t::create_udp_socket_id() ;
        sdp = so_memory::memory::alloc( std::move( sd ), "" ) ;

        {
            so_thread::lock_guard_t lk( _mtx_udps ) ;

            auto const iter = _udps.find( sid ) ;
            so_log::log::error_and_exit( iter != _udps.end(),
                "[linux_socket_system::send] : Id can not be stored mutliple times" ) ;

            _udps[ sid ] = sdp ;
        }
    }

    std::thread( [=]( void_t )
    {
        in_addr ina ;

        inet_aton( addr.to_string().c_str(), &ina ) ;

        sockaddr_in send_to_info ;        
        send_to_info.sin_family = AF_INET;
        send_to_info.sin_addr.s_addr = ina.s_addr ;
        send_to_info.sin_port = htons( ushort_t( port ) );

        while( true )
        {
            so_net::send_array_t buffer ;
            size_t sib = 0 ;

            size_t const res = user_funk( buffer, sib ) ;

            size_t ires = sendto( sdp->s, ( char_cptr_t ) buffer.data(), int(sib), 0,
                ( sockaddr const * ) & send_to_info, sizeof( send_to_info ) ) ;

            if( ires == size_t(-1) )
            {
                so_log::log::error( "[linux_socket_system::send] : socket error" ) ;
                break ;
            }

            if( res == size_t( -1 ) ) break ;
        }

    } ).detach() ;

    return so_net::ok ;
}

//**********************************************************************************
so_net::result linux_socket_system::recv( 
    ip4_binding_point_cref_t bp, recv_funk_t user_funk, so_thread::sync_object_ptr_t sobj )
{    
    socket_data_t sd ;

    {
        auto s = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP ) ;
        if( s == -1 )
        {
            so_log::log::error( "[linux_socket_system::recv] : socket" ) ;
            return so_net::failed ;
        }

        sd.s = s ;
    }

    sockaddr_in server ;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( ushort_t( bp.port ) );

    {
        auto const res = bind( sd.s, ( struct sockaddr * )&server, sizeof( server ) ) ;
        if( res == -1 )
        {
            so_log::log::error( "[linux_socket_system::recv] : bind" ) ;
            return so_net::failed ;
        }
    }

    socket_data_ptr_t sdp = nullptr ;

    {
        udp_socket_id_t sid = this_t::create_udp_socket_id() ; 
        sdp = so_memory::memory::alloc( std::move( sd ), "" ) ;

        {
            so_thread::lock_guard_t lk( _mtx_udps ) ;
            
            auto const iter = _udps.find( sid ) ;
            so_log::log::error_and_exit( iter != _udps.end(), 
                "[win32_socket_system::recv] : Id can not be stored mutliple times" ) ;

            _udps[ sid ] = sdp ;
        }
    }

    std::thread( [=]( void_t )
    {
        while( true )
        {
            size_t const buflen = 2048 ;
            char_t buffer[ buflen ] ;
            memset( buffer, 0, buflen ) ;

            // @todo fill out data
            so_net::sender_info_t si ;

            auto const received = recvfrom( sdp->s, buffer, buflen, 0, NULL, NULL );
            
            // closed
            if( received == 0 ) break ;

            auto const res = user_funk( (byte_cptr_t)buffer, received, si ) ;
            if( res == size_t(-1) ) break  ;

            std::this_thread::sleep_for( std::chrono::milliseconds( res ) ) ;
        }

        if( so_core::is_not_nullptr( sobj ) )
            sobj->set_and_signal() ;

    } ).detach() ;

    return so_net::ok ;
}

//**********************************************************************************
void_t linux_socket_system::destroy( void_t )
{
    this_t::destroy( this ) ;
}

//**********************************************************************************
udp_socket_id_t linux_socket_system::create_udp_socket_id( void_t )
{
    so_thread::lock_guard_t lk( _mtx_udps ) ;
    return _udp_id++ ;
}