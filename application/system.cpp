//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "system.h"
#include <snakeoil/log/log.h>

using namespace so_app ;

so_app::system_ptr_t system::_system_ptr = nullptr ;

//***********************************************************************
so_app::result system::init( void_t )
{
    if( so_log::log::warning( _system_ptr != nullptr, 
        "[so_app::system::init] : system singleton already initialized." ) ) 
        return so_app::ok ;

    _system_ptr = so_app::memory::alloc( system(), "[so_app::system::init] : system" ) ;

    return so_app::ok ;
}

//***********************************************************************
so_app::result system::deinit( void_t ) 
{
    so_app::memory::dealloc( _system_ptr ) ;
    _system_ptr = nullptr ;

    return so_app::ok ;
}

//***********************************************************************

