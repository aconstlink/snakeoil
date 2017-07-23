//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "null_socket_system.h"

using namespace so_net ;

//**********************************************************************************
null_socket_system::null_socket_system( void_t )
{

}

//**********************************************************************************
null_socket_system::null_socket_system( this_rref_t )
{

}

//**********************************************************************************
null_socket_system::~null_socket_system( void_t )
{

}

//**********************************************************************************
null_socket_system::this_ptr_t null_socket_system::create( so_memory::purpose_cref_t p )
{
    return so_net::memory::alloc( this_t(), p ) ;
}

//**********************************************************************************
void_t null_socket_system::destroy( this_ptr_t ptr )
{
    so_net::memory::dealloc( ptr ) ;
}

//**********************************************************************************
so_net::result null_socket_system::send( ip4_address_cref_t, uint_t const, send_funk_t, so_thread::sync_object_ptr_t )
{
    return so_net::ok ;
}

//**********************************************************************************
so_net::result null_socket_system::recv( 
    ip4_binding_point_cref_t, recv_funk_t user_funk, so_thread::sync_object_ptr_t sobj )
{
    std::thread( [=]( void_t )
    {
        while( true )
        {
            so_net::sender_info_t si ;
            auto const res = user_funk( nullptr, 0, si  ) ;
            if( res == size_t( -1 ) ) break  ;
        }

        if( so_core::is_not_nullptr( sobj ) )
            sobj->set_and_signal() ;

    } ).detach() ;

    return so_net::ok ;
}

//**********************************************************************************
void_t null_socket_system::destroy( void_t )
{
    this_t::destroy( this ) ;
}

//**********************************************************************************