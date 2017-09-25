//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "global.h"

#include "system/system.h"

using namespace so_log ;

so_thread::mutex_t global::_mtx ;
global::this_ptr_t global::_ptr = nullptr ;

//*************************************************************************************
global::global( void_t )
{
    __default_log_system = so_log::system_t::create() ;
}

//*************************************************************************************
global::global( this_rref_t rhv )
{
    so_move_member_ptr( __default_log_system, rhv ) ;
}

//*************************************************************************************
global::~global( void_t )
{
    so_log::system::destroy( __default_log_system ) ;
}

//*************************************************************************************
global::this_ptr_t global::init( void_t )
{
    if( so_core::is_not_nullptr( global_t::_ptr ) )
        return this_t::_ptr ;

    {
        so_thread::lock_guard_t lk( this_t::_mtx ) ;

        if( so_core::is_not_nullptr( global_t::_ptr ) )
            return this_t::_ptr ;

        // again, do not use snakeoil memory
        // memory is above log
        this_t::_ptr = new this_t() ;

        this_t::status( "[online] : snakeoil log" ) ;
    }

    return this_t::_ptr ;
}

//*************************************************************************************
void_t global::deinit( void_t )
{
    if( so_core::is_nullptr( global_t::_ptr ) )
        return ;

    delete global_t::_ptr ;
    this_t::_ptr = nullptr ;
}

//*************************************************************************************
global::this_ptr_t global::get( void_t )
{
    return this_t::init() ;
}

//*************************************************************************************
void_t global::message( log_level level, string_cref_t msg )
{
    this_t::get()->__default_log_system->log( level, msg ) ;
}

//*************************************************************************************
bool_t global::message( bool_t condition, log_level level, string_cref_t msg )
{
    if( condition ) so_log::global::message( level, msg ) ;
    return condition ;
}

//*************************************************************************************
void_t global::status( string_cref_t msg )
{
    so_log::global::message( so_log::log_level::status, msg ) ;
}

//*************************************************************************************
bool_t global::status( bool_t condition, string_cref_t msg )
{
    return so_log::global::message( condition, so_log::log_level::status, msg ) ;
}

//*************************************************************************************
void_t global::warning( string_cref_t msg )
{
    so_log::global::message( so_log::log_level::warning, msg ) ;
}

//*************************************************************************************
bool_t global::warning( bool_t condition, string_cref_t msg )
{
    return so_log::global::message( condition, so_log::log_level::warning, msg ) ;
}

//*************************************************************************************
void_t global::error( string_cref_t msg )
{
    so_log::global::message( so_log::log_level::error, msg ) ;
}

//*************************************************************************************
void_t global::error_and_exit( string_cref_t msg )
{
    so_log::global::message( so_log::log_level::error, msg ) ;
    so_core::break_if( true ) ;
}

//*************************************************************************************
void_t global::error_and_exit( bool_t condition, string_cref_t msg )
{
    so_core::break_if( so_log::global::message( condition, so_log::log_level::error, msg ) ) ;
}

//*************************************************************************************
bool_t global::error( bool_t condition, string_cref_t msg )
{
    return so_log::global::message( condition, so_log::log_level::error, msg ) ;
}


//*************************************************************************************
void_t global::critical( string_cref_t msg )
{
    so_log::global::message( so_log::log_level::critical, msg ) ;
}

//*************************************************************************************
bool_t global::critical( bool_t condition, string_cref_t msg )
{
    return so_log::global::message( condition, so_log::log_level::critical, msg ) ;
}
