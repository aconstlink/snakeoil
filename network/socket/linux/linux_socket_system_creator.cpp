//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "../socket_system_creator.h"

#include "linux_socket_system.h"

using namespace so_net ;

//**********************************************************************************
isocket_system_ptr_t socket_system_creator::create_os_system( void_t )
{
    return so_net::so_linux::linux_socket_system_t::create( 
        "[socket_system_creator::create_os_system] : linux_socket_system" ) ;
}