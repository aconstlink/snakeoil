//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "system.h"
#include "../logger/std_cerr_logger.h"

#include <algorithm>

using namespace so_log ;

//*************************************************************************************

system::system( void_t )
{
    _this_default_logger = new std_cerr_logger( ) ;
    _default_logger = _this_default_logger ;
    this_t::add_logger( _default_logger ) ;
}

//*************************************************************************************
system::~system( void_t )
{
    so_assert( _default_logger == _this_default_logger ) ;
    delete _this_default_logger ;
}

//*************************************************************************************
result system::log( so_log::log_level ll, std::string const & msg )
{
    for( auto * logger : _loggers )
    {
        logger->log( ll, msg ) ;
    }
    return so_log::ok ;
}

//*************************************************************************************
result system::add_logger( logger_ptr_t lptr )
{
    if( so_core::is_nullptr( lptr ) )
    {
        return so_log::invalid_argument ;
    }

    auto iter = std::find( _loggers.begin(), _loggers.end(), lptr ) ;
    if( iter != _loggers.end() )
    {
        return so_log::ok ;
    }

    _loggers.push_back( lptr ) ;

    return lptr->log( so_log::log_level::status, "Logger online" ) ;
}

//*************************************************************************************
logger_ptr_t system::set_default_logger( logger_ptr_t lptr )
{
    logger_ptr_t ret_ptr = _default_logger ;
    _default_logger = lptr == nullptr ? _this_default_logger : lptr ;
    return ret_ptr == _this_default_logger ? nullptr : ret_ptr ;
}

