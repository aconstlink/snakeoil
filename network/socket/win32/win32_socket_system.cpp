//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "win32_socket_system.h"

#include <snakeoil/log/log.h>

#include <ws2tcpip.h>

using namespace so_net ;
using namespace so_net::so_win32 ;

//**********************************************************************************
win32_socket_system::win32_socket_system( void_t )
{
    WSAData data ;
    auto const res = WSAStartup( MAKEWORD( 2, 2 ), &data ) ;
    if( res != 0 )
    {
        so_log::log::error( "[win32_socket_system::initialize] : WSAStartup" ) ;
    }
    _is_init = true ;
}

//**********************************************************************************
win32_socket_system::win32_socket_system( this_rref_t rhv )
{
    _udps = std::move( rhv._udps ) ;
    _is_init = rhv._is_init ;
    rhv._is_init = false ;
}

//**********************************************************************************
win32_socket_system::~win32_socket_system( void_t )
{
    // @todo delete/release sockets

    for( auto & sd : _udps )
    {
        auto const res = closesocket( sd.second->s ) ;
        so_log::log::error( res == SOCKET_ERROR, 
            "[win32_socket_system::~win32_socket_system] : closesocket"  ) ;

        so_memory::memory::dealloc( sd.second ) ;
    }

    if( _is_init )
        WSACleanup() ;
}

//**********************************************************************************
win32_socket_system::this_ptr_t win32_socket_system::create( so_memory::purpose_cref_t p )
{
    return so_net::memory::alloc( this_t(), p ) ;
}

//**********************************************************************************
void_t win32_socket_system::destroy( this_ptr_t ptr )
{
    so_net::memory::dealloc( ptr ) ;
}

//**********************************************************************************
so_net::result win32_socket_system::send( ip4_address_cref_t addr, uint_t const port, 
    send_funk_t user_funk, so_thread::sync_object_ptr_t sop )
{
    socket_data_t sd ;

    {
        auto s = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP ) ;
        if( s == -1 )
        {
            so_log::log::error( "[win32_socket_system::send] : socket" ) ;
            so_log::log::error( "[win32_socket_system::send] : WSAGetLastError " +
                std::to_string( WSAGetLastError() ) ) ;
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
                "[win32_socket_system::send] : Id can not be stored mutliple times" ) ;

            _udps[ sid ] = sdp ;
        }
    }

    std::thread( [=]( void_t )
    {
        IN_ADDR ina ;

        inet_pton( AF_INET, addr.to_string().c_str(), ( void* )&ina ) ;

        sockaddr_in send_to_info ;
        send_to_info.sin_family = AF_INET;
        send_to_info.sin_addr.s_addr = ina.S_un.S_addr ;
        send_to_info.sin_port = htons( ushort_t( port ) );

        while( true )
        {
            so_net::send_array_t buffer ;
            size_t sib = 0 ;

            size_t const res = user_funk( buffer, sib ) ;

            auto const ires = sendto( sdp->s, ( char_cptr_t ) buffer.data(), int(sib), 0,
                ( SOCKADDR * ) & send_to_info, sizeof( send_to_info ) ) ;

            if( ires == SOCKET_ERROR )
            {
                so_log::log::error( "[win32_socket_system::send] : socket error" ) ;
                break ;
            }

            if( res == size_t( -1 ) ) break ;
        }

    } ).detach() ;

    return so_net::ok ;
}

//**********************************************************************************
so_net::result win32_socket_system::recv( 
    ip4_binding_point_cref_t bp, recv_funk_t user_funk, so_thread::sync_object_ptr_t sobj )
{    
    socket_data_t sd ;

    {
        auto s = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP ) ;
        if( s == -1 )
        {
            so_log::log::error( "[win32_socket_system::recv] : socket" ) ;
            so_log::log::error( "[win32_socket_system::recv] : WSAGetLastError " + 
                std::to_string( WSAGetLastError() ) ) ;
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
            so_log::log::error( "[win32_socket_system::recv] : bind" ) ;
            so_log::log::error( "[win32_socket_system::recv] : WSAGetLastError " + 
                std::to_string( WSAGetLastError() ) ) ;
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
void_t win32_socket_system::destroy( void_t )
{
    this_t::destroy( this ) ;
}

//**********************************************************************************
udp_socket_id_t win32_socket_system::create_udp_socket_id( void_t )
{
    so_thread::lock_guard_t lk( _mtx_udps ) ;
    return _udp_id++ ;
}