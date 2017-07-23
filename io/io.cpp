//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "io.h"

#include "system/system.h"

#include <snakeoil/log/log.h>
#include <snakeoil/core/break_if.hpp>

using namespace so_io ;

so_io::system_ptr_t so_io::io::_ios = nullptr ;

//*************************************************************************************
void_t io::init( void_t )
{
    if( so_core::is_not_nullptr( _ios ) ) return ;
    _ios = so_memory::memory::alloc( so_io::system_t(), "[io::init] : system" ) ;
}

//*************************************************************************************
void_t io::deinit( void_t )
{
    if( so_core::is_nullptr( _ios ) ) return ;
    so_memory::memory::dealloc( _ios ) ;
    _ios = nullptr ;
}

//*************************************************************************************
so_io::load_handle_t io::load( so_io::path_cref_t file_path )
{
    so_core::break_if( so_core::is_nullptr( _ios ) ) ;
    return _ios->load( file_path ) ;
}

//*************************************************************************************
so_io::store_handle_t io::store( so_io::path_cref_t file_path, char_cptr_t data, size_t sib )
{
    so_core::break_if( so_core::is_nullptr( _ios ) ) ;
    return _ios->store( file_path, data, sib ) ;
}