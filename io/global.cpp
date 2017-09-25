//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "global.h"

#include "system/system.h"

#include <snakeoil/log/global.h>

using namespace so_io ;

so_thread::mutex_t global::_mtx ;
global * global::_ptr = nullptr ;

//***********************************************************************
global::global( void_t )
{
    _ios = so_io::system_t::create( so_io::system_t(),
        "[global::global] : job_scheduler" ) ;
}

//***********************************************************************
global::global( this_rref_t rhv )
{
    so_move_member_ptr( _ios, rhv ) ;
}

//***********************************************************************
global::~global( void_t )
{
    so_io::system_t::destroy( _ios ) ;
}

//***********************************************************************
global::this_ptr_t global::create( this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_thread::memory::alloc( std::move( rhv ), p ) ;
}

//***********************************************************************
void_t global::destroy( this_ptr_t ptr )
{
    so_thread::memory::dealloc( ptr ) ;
}

//***********************************************************************
global::this_ptr_t global::init( void_t )
{
    if( so_core::is_not_nullptr( _ptr ) )
        return this_t::_ptr ;

    {
        so_thread::lock_guard_t lk( this_t::_mtx ) ;

        if( so_core::is_not_nullptr( _ptr ) )
            return this_t::_ptr ;

        this_t::_ptr = this_t::create( this_t(),
            "[so_io::global::init]" ) ;

        so_log::global::status( "[online] : snakeoil io" ) ;
    }

    return this_t::_ptr ;
}

//***********************************************************************
void_t global::deinit( void_t )
{
    if( so_core::is_nullptr( _ptr ) )
        return ;

    this_t::destroy( _ptr ) ;
    _ptr = nullptr ;
}

//***********************************************************************
global::this_ptr_t global::get( void_t )
{
    return this_t::init() ;
}

//***********************************************************************
so_io::system_ptr_t global::io_system( void_t )
{
    return this_t::get()->_ios ;
}

//***********************************************************************
so_io::load_handle_t global::load( so_io::path_cref_t file_path )
{
    return this_t::io_system()->load( file_path ) ;
}

//***********************************************************************
so_io::store_handle_t global::store( so_io::path_cref_t file_path, char_cptr_t data, size_t sib )
{
    return this_t::io_system()->store( file_path, data, sib ) ;
}