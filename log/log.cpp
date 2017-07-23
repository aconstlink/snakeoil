//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "log.h"

#include "system/system.h"

#include <snakeoil/core/break_if.hpp>

using namespace so_log ;

so_log::system * log::__default_log_system = nullptr ;


//*************************************************************************************
void_t log::init( void_t ) 
{
    if( __default_log_system == nullptr ) 
        __default_log_system = new so_log::system() ;
}

//*************************************************************************************
so_log::system * log::get_logging_system( void_t ) 
{
    return __default_log_system ;
}

//*************************************************************************************
void_t log::message( log_level level, string_cref_t msg )
{
    //so_log::log::message( level, module, msg.c_str() ) ;
    log::get_logging_system()->log( level, msg ) ;
}

//*************************************************************************************
bool_t log::message( bool_t condition, log_level level, string_cref_t msg )
{
    if( condition ) so_log::log::message( level, msg ) ;
    return condition ;
}

//*************************************************************************************
void_t log::status( string_cref_t msg )
{
    so_log::log::message( so_log::log_level::status, msg ) ;
}

//*************************************************************************************
bool_t log::status( bool_t condition, string_cref_t msg )
{
    return so_log::log::message( condition, so_log::log_level::status, msg ) ;
}

//*************************************************************************************
void_t log::warning( string_cref_t msg )
{
    so_log::log::message( so_log::log_level::warning, msg ) ;
}

//*************************************************************************************
bool_t log::warning( bool_t condition, string_cref_t msg )
{
    return so_log::log::message( condition, so_log::log_level::warning, msg ) ;
}

//*************************************************************************************
void_t log::error( string_cref_t msg )
{
    so_log::log::message( so_log::log_level::error, msg ) ;
}

//*************************************************************************************
void_t log::error_and_exit( string_cref_t msg )
{
    so_log::log::message( so_log::log_level::error, msg ) ;
    so_core::break_if( true ) ;
}

//*************************************************************************************
void_t log::error_and_exit( bool_t condition, string_cref_t msg )
{
    so_core::break_if( so_log::log::message( condition, so_log::log_level::error, msg ) ) ;
}

//*************************************************************************************
bool_t log::error( bool_t condition, string_cref_t msg )
{
    return so_log::log::message( condition, so_log::log_level::error, msg ) ;
}


//*************************************************************************************
void_t log::critical( string_cref_t msg )
{
    so_log::log::message( so_log::log_level::critical, msg ) ;
}

//*************************************************************************************
bool_t log::critical( bool_t condition, string_cref_t msg )
{
    return so_log::log::message( condition, so_log::log_level::critical, msg ) ;
}

