//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "../socket_system_creator.h"

#include "win32_socket_system.h"

using namespace so_net ;

//**********************************************************************************
isocket_system_ptr_t socket_system_creator::create_os_system( void_t )
{
    return so_net::so_win32::win32_socket_system_t::create( 
        "[socket_system_creator::create_os_system] : win32_socket_system" ) ;
}